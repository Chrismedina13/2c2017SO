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



/*
			RespuestaTransformacionYAMA* nodo1 = setRespuestaTransformacionYAMA(11,21,"12.121",151,123,"Documento1");
			RespuestaTransformacionYAMA* nodo2 = setRespuestaTransformacionYAMA(12,22,"12.122",152,123,"Documento2");
			RespuestaTransformacionYAMA* nodo3 = setRespuestaTransformacionYAMA(13,23,"12.123",153,123,"Documento3");
			t_list* lista1 = list_create();
			list_add(lista1,nodo1);
			list_add(lista1,nodo2);
			list_add(lista1,nodo3);
			int i = 0;
			char* ListaSerializada = malloc(tamanioRespuestaTransformacionYAMA(lista1) + sizeof(int)*list_size(lista1)+sizeof(int));
			int desplazamiento;
			int cantidadDeElementos = list_size(lista1);
			//SERIALIZAR
			desplazamiento = 0;
			serializarDato(ListaSerializada, &cantidadDeElementos, sizeof(int), &desplazamiento);
			for(i=0;i<list_size(lista1);i++){
				RespuestaTransformacionYAMA* nodo = list_get(lista1,i);

				char* respuestaAUnNodo = serializarRespuestaTransformacionYAMA(nodo->nodo,
							nodo->puertoWorker, nodo->ipWorkwer, nodo->bloque,
							nodo->bytesOcupados, nodo->archivoTemporal);
				//int tamanio = tamanioEstructuraRespuestaTransf(nodo); //ACA PROBE CON MANDARLE EL TAMAÑO DE LA ESTRUCTURA SIN SERIALZIAR PERO NO FUNCIONÓ
					//			logInfo("EL TAMANIO DE LA ESTRUCTURA %d ES: %d",i,tamanio);

				int tamanio = strlen(respuestaAUnNodo); //ACA PROBE MANDARLE LA LONGITUD DE LA RESPUESTA SERIALIZADA
				logInfo("EL TAMANIO DE LA ESTRUCTURA SERIALIZADA %d ES: %d",i,tamanio);

				serializarDato(ListaSerializada, &tamanio, sizeof(int), &desplazamiento);
				serializarDato(ListaSerializada, respuestaAUnNodo,
							tamanio, &desplazamiento);

			}

			//DESERIALIZAR
			desplazamiento=0;
			i=0;
			t_list* lista = list_create();
			int tamanioEstructura;
			int longitudDeLista;
			char* respuesta;
			deserializarDato(&(longitudDeLista),
							ListaSerializada, sizeof(int), &desplazamiento);
			for(i=0;i<longitudDeLista;i++){
				deserializarDato(&(tamanioEstructura),ListaSerializada, sizeof(int), &desplazamiento);
				respuesta = string_substring(ListaSerializada,desplazamiento,tamanioEstructura);
				desplazamiento= desplazamiento+tamanioEstructura;
				RespuestaTransformacionYAMA* respuestaDeserializada = deserializarRespuestaTransformacionYAMA(respuesta);
				list_add(lista,respuestaDeserializada);
			}
			RespuestaTransformacionYAMA* respuestaDeserializada = list_get(lista,0);
			logInfo("\nNODO: %d\nPUERTO WORKER: %d\nIP WORKER: %s\nBLOQUE: %d\nBYTESOCUPADOS: %d\nARCHIVO TEMPORAL: %s\n",respuestaDeserializada->nodo,
							respuestaDeserializada->puertoWorker, respuestaDeserializada->ipWorkwer, respuestaDeserializada->bloque,
							respuestaDeserializada->bytesOcupados, respuestaDeserializada->archivoTemporal);

*/

			/*char* listaSErializada = serializarListaRespuestaTransf(lista);
			logInfo("Lista serializada");
			t_list* listaDeserializada = deserializarListaRespuestaTransf(listaSErializada);
			logInfo("Respuesta lista deserialoizada");

			//RespuestaTransformacionYAMA* nodo = list_get(listaDeserializada,0);
			//printf("\n\nArchivo temporal: %s \n\n",nodo->archivoTemporal);

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
