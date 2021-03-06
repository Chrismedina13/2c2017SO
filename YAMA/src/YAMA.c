/*
 ============================================================================
 Name        : YAMA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Headers/yama.h"


int main(int argc, char *argv[]) {

    jobsAPlanificar = queue_create();
    listaDeWorkerTotales = list_create();
    listaDeJobs = list_create();
    ipYPuertoWoerkers = list_create();
    variableGlobalIdentificador = 0;
    variableNombreTransformacion = 0;
    variableReduciionGlobal = 0;
    VariableReduccionLocal = 0;

    sem_init(&semaforoYAMA,0,0);
    tabla_estados = list_create();

    //Archivo de Logs
    crearLog("YAMA.log","YAMA",1,log_level_from_string("INFO"));


    //Archivo de configuracion
    Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

    parametrosFileSystem = setParametrosComunicacionConFileSystem(
            config->puertoFileSystem, config->ipFileSystem,config->algoritmo_bal,config->disponibilidadBase,config->retardo);
    ParametrosComunicacionConMaster* parametrosMaster = setParametrosComunicacionConMaster(config->puertoYama);

    signal(SIGUSR1,rutina);

    logInfo("Creando hilos para comunicacion con YAMA y FS");

    pthread_t hiloComunicacionConFileSystem;
    pthread_t hiloComunicacionConMaster;

    pthread_create(&hiloComunicacionConFileSystem,NULL,(void*) comunicacionConFileSystem, parametrosFileSystem);
    pthread_create(&hiloComunicacionConMaster,NULL,(void*) comunicacionConMasters, parametrosMaster);


    pthread_join(hiloComunicacionConMaster,NULL);
    pthread_join(hiloComunicacionConFileSystem,NULL);

    sem_destroy(&semaforoYAMA);
    free(config);
    queue_destroy_and_destroy_elements(jobsAPlanificar,free);
    list_destroy(listaDeWorkerTotales);
    return EXIT_SUCCESS;
}


Job* crearJOB(int FDMAster,char* nombreDelJob){

        Job* job = malloc((sizeof(int)*2)+strlen(nombreDelJob));
        job->master = FDMAster;
        job->nombreDelArchivo = nombreDelJob;
        job->identificadorJob = variableGlobalIdentificador++;
        return job;


}


void rutina(int senial){

    if(senial == SIGUSR1){

        printf("CAMBIANDO EL ALGORITMO DE PLANIFICACION \n");
        if(parametrosFileSystem->algoritmo == "W-CLOCK"){

            parametrosFileSystem->algoritmo = "CLOCK";
            printf("ALGORITMO CAMBIADO A CLOCK");
        }else{

            parametrosFileSystem->algoritmo = "W-CLOCK";
            printf("ALGORITMO CAMBIADO A CLOCK");
        }

        printf("AUMENTANDO EN 100 EL RETARDO");

        parametrosFileSystem->retardo += 100;


    }
}
