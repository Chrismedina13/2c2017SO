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

	//PROBANDO SERIALIZAR Y DESERIALIZAR

			RespuestaTransformacionYAMA* nodo1 = setRespuestaTransformacionYAMA(11,21,"12.121",151,123451,"Documento1");
			RespuestaTransformacionYAMA* nodo2 = setRespuestaTransformacionYAMA(12,22,"12.122",152,123452,"Documento2");
			RespuestaTransformacionYAMA* nodo3 = setRespuestaTransformacionYAMA(13,23,"12.123",153,123453,"Documento3");
			t_list* lista = list_create();
			list_add(lista,nodo1);
			list_add(lista,nodo2);
			list_add(lista,nodo3);

			char* listaSErializada = serializarListaRespuestaTransf(lista);
			logInfo("Lista serializada");
			RespuestaTransformacionYAMA* respuestaDeserializada = deserializarListaRespuestaTransf(listaSErializada);
			logInfo("Respuesta lista deserialoizada");
	/*		logInfo("\nNodo: %i\nPuerto woeker: %d\nArchivo temporal: %s",n,nodo->puertoWorker,nodo->archivoTemporal);

			char* respuesta = serializarRespuestaTransformacionYAMA(nodo->nodo,nodo->puertoWorker, nodo->ipWorkwer, nodo->bloque,nodo->bytesOcupados, nodo->archivoTemporal);
			logInfo("Antes de deserializar. Tamanio de respuesta: %d ", strlen(respuesta));


			RespuestaTransformacionYAMA* nodoDeserializado = deserializarRespuestaTransformacionYAMA(respuesta);
			printf("\n\nArchivo temporal: %s \n\n",nodoDeserializado->archivoTemporal);
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
