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

<<<<<<< HEAD
=======
	int puerto1= 5050;
	int puerto2= 5050;
	int puerto3= 5050;
	int puerto4= 5050;

	char* ip1 = "127.0.0.1";
	char* ip2 = "127.0.0.1";
	char* ip3 = "127.0.0.1";
	char* ip4 = "127.0.0.1";



	Info_Workers* info1 = malloc(sizeof(int) + strlen(ip1));
	Info_Workers* info2 = malloc(sizeof(int) + strlen(ip2));
	Info_Workers* info3 = malloc(sizeof(int) + strlen(ip3));
	Info_Workers* info4 = malloc(sizeof(int) + strlen(ip4));

	info1->puerto = puerto1;
	info1->ipWorker = ip1;
	info2->puerto = puerto2;
	info2->ipWorker = ip2;
	info3->puerto = puerto3;
	info3->ipWorker = ip3;
	info4->puerto = puerto4;
	info4->ipWorker = ip4;


	list_add(ipYPuertoWoerkers,info1);
	list_add(ipYPuertoWoerkers,info2);
	list_add(ipYPuertoWoerkers,info3);
	list_add(ipYPuertoWoerkers,info4);
>>>>>>> f333b0f8cd6c8bfee7b62899b1a0d3df282b615d

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

		int numero;
		char* algoritmo;
		printf("INGRESE EL NUEVO RETARDO DE PLANIFICACION: ");
		scanf("%d", &numero);

		printf("INGRESE W-CLOCK o CLOCK:");
		scanf("%s",&algoritmo);

		parametrosFileSystem->algoritmo = algoritmo;
		parametrosFileSystem->retardo = numero;
	}
}

