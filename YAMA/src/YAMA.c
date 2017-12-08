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

	//PROBANDO SERIALIZAR INFO REDUCCION GLOBAL DE MASTER PARA WORKER
	char* archivoG = "archivoTemporalGlobal";
	char* archivo1 = "archivotemplocal1";
	char* archivo2 = "archivotemporallocal2";
	char* archivo3 = "arclocal2";
	t_list* lista = list_create();

	list_add(lista,archivo1);
	list_add(lista,archivo2);
	list_add(lista,archivo3);

	/*char* listaS = serializarListaArchivos(lista);
	t_list* l = deserializarListaArchivos(listaS);
	int i =0;
	while(i<list_size(l)){
		char* contenido =list_get(l,i);
		printf("%s\n\n",contenido);
		i++;
	}*/


	/*
	char* rutaScript = "/home/utnso/Escritorio/Serializar";
	char* punteroAlContenidoDelScript = obtenerPuntero(rutaScript);
	script* script = malloc(strlen(rutaScript)+strlen(punteroAlContenidoDelScript));

	script->nombre = rutaScript;
	script->contenido = punteroAlContenidoDelScript;


 	infoReduccionGlobalDeMasterParaWorker* info = crearInfoReduccionGlobalDeMasterParaWorker(lista,archivoG,script);
	char* infoS =  serializarInfoReduccionGlobalDeMasterParaWorker(info);
	infoReduccionGlobalDeMasterParaWorker* i = deserializarInfoReduccionGlobalDeMasterParaWorker(infoS);
	t_list* l = i->listaInfoParaReduccionGlobal;
	int j =0;
		while(j<list_size(l)){
			char* contenido =list_get(l,j);
			printf("%s\n\n",contenido);
			j++;
		}
*/
	//PRUEBA DE INFOPARAREDUCCIONGLOBAL
/*
	infoParaReduccionGlobal* info = crearInfoParaReduccionGlobal("123.412.4.2",4040,"hola.txt");
	char* infoSerializada =  serializarInfoParaReduccionGlobal(info);
	infoParaReduccionGlobal* i = deserializarInfoParaReduccionGlobal(infoSerializada);
	printf("\nIP: %s\nPUERTO: %d\nARCHIVO: %s\n",i->ipWorker,i->puerto,i->archivoTemporalReduccionLocal);
*/
	/*PRUEBA DE LISTAUBICACIONESBLOQUESARCHIVOS

		UbicacionBloquesArchivo* u1 = malloc(sizeof(int)*6);
		u1->bytesOcupados=10;
		u1->parteDelArchivo=3;
		u1->ubicacionCopia1.desplazamiento=10;
		u1->ubicacionCopia1.nodo=1;
		u1->ubicacionCopia2.desplazamiento=20;
		u1->ubicacionCopia2.nodo=2;

		UbicacionBloquesArchivo* u2 = malloc(sizeof(int)*6);
		u2->bytesOcupados=900;
		u2->parteDelArchivo=1;
		u2->ubicacionCopia1.desplazamiento=4;
		u2->ubicacionCopia1.nodo=4;
		u2->ubicacionCopia2.desplazamiento=5;
		u2->ubicacionCopia2.nodo=5;

		UbicacionBloquesArchivo* u3 = malloc(sizeof(int)*6);
		u3->bytesOcupados=123;
		u3->parteDelArchivo=1;
		u3->ubicacionCopia1.desplazamiento=4;
		u3->ubicacionCopia1.nodo=4;
		u3->ubicacionCopia2.desplazamiento=5;
		u3->ubicacionCopia2.nodo=5;

		UbicacionBloquesArchivo* u4 = malloc(sizeof(int)*6);
			u4->bytesOcupados=456;
			u4->parteDelArchivo=1;
			u4->ubicacionCopia1.desplazamiento=477;
			u4->ubicacionCopia1.nodo=4;
			u4->ubicacionCopia2.desplazamiento=58888;
			u4->ubicacionCopia2.nodo=56789;
		t_list* lista = list_create();
		list_add(lista,u1);
		list_add(lista,u2);
		list_add(lista,u3);
		list_add(lista,u4);

		char* listaSerializada =serializarListaUbicacionBloquesArchivos(lista);
		t_list* listaDeserializada = deserializarUbicacionBloquesArchivos(listaSerializada);
		int i;
		for(i=0;i<list_size(listaDeserializada);i++){
			UbicacionBloquesArchivo* u = list_get(listaDeserializada,i);
			logInfo("\nBytes Ocupados: %i\nParte del archivo: %i\nCopia 1 nodo: %i\nCopia 1 desplazamiento: %i\n,Copia 2 nodo: %i\nCopia 2 desplazamiento: %i\n",
					u->bytesOcupados,u->parteDelArchivo,
					u->ubicacionCopia1.nodo,u->ubicacionCopia1.desplazamiento,
					u->ubicacionCopia2.nodo,u->ubicacionCopia2.desplazamiento);
		}

*/

	//EJEMPLO SERIALZIAR UBICACION BLOQUES ARCHIVOS
/*
	UbicacionBloquesArchivo* info = malloc (sizeof(int)*6);
		info->parteDelArchivo = 1500;
		info->bytesOcupados =120;
		info ->ubicacionCopia1.nodo = 17;
		info ->ubicacionCopia1.desplazamiento = 18;
		info->ubicacionCopia2.nodo = 20;
		info->ubicacionCopia2.desplazamiento = 21;

		char* infoSer = serializarUblicacionBloqueArchivo(info);
		logInfo("Pasó serializarUblicacionBloqueArchivo ");

		UbicacionBloquesArchivo* infoDes = deserializarUbicacionArchivo(infoSer);
			logInfo("Pasó deserializarUbicacionArchivo ");
			logInfo("Script deserializado\nNodo: %i\nDesplazamiento: %i",infoDes->ubicacionCopia1.nodo,infoDes->ubicacionCopia1.desplazamiento);
			logInfo("Script deserializado\nNODO: %i\nDesplazamiento %i",infoDes->ubicacionCopia2.nodo,infoDes->ubicacionCopia2.desplazamiento);
*/
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

