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

	//EJEMPLO DE SERIALIZAR Y DESERIALIZAR SCRIPT
/*
	char* rutaScript = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Script.sh";
	char* punteroAlContenidoDelScript = obtenerPuntero(rutaScript);
	script* script1 = malloc(strlen(rutaScript)+strlen(punteroAlContenidoDelScript));

	script1->nombre = rutaScript;
	script1->contenido = punteroAlContenidoDelScript;

	char* scriptSerializado = serializarScript(script1);
	logInfo("Script serializado");
	script* scriptDeserializado = deserilizarScript(scriptSerializado);
	logInfo("Script deserializado\n\nNombre: %s\nContenido: %s",scriptDeserializado->nombre,scriptDeserializado->contenido);
*/
/*
	//EJEMPLO DE USO serializarInfoParaWorker

	logInfo("empieza el ejemplo serializar info para worker");

	char* rutaArchivoTemporal = "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/tmp.dat";

	char* infoSerializada = serializarInfoParaWorker(1, 3, 900,rutaArchivoTemporal);
	logInfo("Pasó serializar info para worker");

	infoParaWorker* infoDeserializada = deserializarInfoParaWorker(infoSerializada);
	logInfo("Pasó deserialziar info para worker");

	logInfo("\nNodo:%d\nBloque:%d\nBytes:%d\nNombre archivo temporal:%s",
			infoDeserializada->nodo,infoDeserializada->bloque,infoDeserializada->bytesOcupados,infoDeserializada->archivoTemporal);

*/
	/*EJEMPLO DE SERIALIZAR LISTA INFO WORKER FUNCIONANDO*/
/*
	 	char* ipWorker1 = "43.2.2.4";
		int puerto1 = 2;
		Info_Workers* info1 = malloc(sizeof(ipWorker1)+ sizeof(int));
		char* ipWorker2 = "100.23.12.4";
		int puerto2 = 21;
		Info_Workers* info2 = malloc(sizeof(ipWorker2)+ sizeof(int));
		char* ipWorker3 = "167.23.2.1";
		int puerto3 = 100;
		Info_Workers* info3 = malloc(sizeof(ipWorker3)+ sizeof(int));

		info1->ipWorker = ipWorker1;
		info1->puerto = puerto1;
		info2->ipWorker = ipWorker2;
		info2->puerto = puerto2;
		info3->ipWorker = ipWorker3;
		info3->puerto = puerto3;

		t_list* lista = list_create();
		list_add(lista,info1);
		list_add(lista,info2);
		//list_add(lista,info3);

 		char* listaSerializada = serializarLista_info_workers(lista);
		logInfo("Lista Serializada");
		t_list* listaDeserializada = deserializarLista_info_workers(listaSerializada);
		logInfo("Lista Deserializada");
		Info_Workers* info4 = list_get(listaDeserializada,0);
		logInfo("\nPuerto: %d\nIP Worker: %s",info4->puerto,info4->ipWorker);
		Info_Workers* info5 = list_get(listaDeserializada,1);
		logInfo("\nPuerto: %d\nIP Worker: %s",info5->puerto,info5->ipWorker);
		//Info_Workers* info6 = list_get(listaDeserializada,2);
		//logInfo("\nPuerto: %d\nIP Worker: %s",info6->puerto,info6->ipWorker);

*/

	//Archivo de configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	parametrosFileSystem = setParametrosComunicacionConFileSystem(
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
