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

	/*EJEMPLO DE USO serializarInfoParaWorker
	logInfo("empieza el ejemplo serializar info para worker");

	char* rutaScript = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Script.sh";
	char* rutaArchivoTemporal = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/tmp.dat";
	char* punteroAlContenidoDelScript = obtenerPuntero(rutaScript);

	logInfo("empieza el ejemplo serializar info para worker2");
	script* scriptTransformacion = malloc(strlen(rutaScript) + strlen(punteroAlContenidoDelScript));
	scriptTransformacion->contenido=punteroAlContenidoDelScript;
	scriptTransformacion->nombre=rutaScript;
	logInfo("empieza el ejemplo serializar info para worker3");
	char* infoSerializada = serializarInfoParaWorker(1, 3, 900,rutaArchivoTemporal, scriptTransformacion);
	logInfo("Pasó serializar info para worker");

	infoParaWorker* infoDeserializada = deserializarInfoParaWorker(infoSerializada);
	logInfo("Pasó deserialziar info para worker");

	logInfo("\nNodo:%d\nBloque:%d\nBytes:%d\nNombre archivo temporal:%s\nNombreDelScript:%s\nContenido del Script\n:%s\n",
			infoDeserializada->nodo,infoDeserializada->bloque,infoDeserializada->archivoTemporal,infoDeserializada->scritpTransformacion->nombre,infoDeserializada->scritpTransformacion->contenido);

*/


	//Archivo de configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	ParametrosComunicacionConFileSystem* parametrosFileSystem = setParametrosComunicacionConFileSystem(
			config->puertoFileSystem, config->ipFileSystem,config->algoritmo_bal,config->disponibilidadBase);
	ParametrosComunicacionConMaster* parametrosMaster = setParametrosComunicacionConMaster(config->puertoYama);



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
