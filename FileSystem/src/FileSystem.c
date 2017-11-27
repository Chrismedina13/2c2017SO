#include "Headers/FileSystem.h"
#include "commons/string.h"
#include "Headers/consola.h"
#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include <stdbool.h>
#include <pthread.h>
#include "Headers/comunicacionConWorker.h"
#include "Headers/nodos.h"

int main(int argc, char *argv[]) {


	//Archivo de logs
	crearLog("FileSystem.log", "FS", 1, log_level_from_string("INFO"));

	//Recibir Nodos
	//cargarNodos();

	//proban2

//	recuperarTablaDeArchivos();

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	//semaforos
	int cantNodos= config->cant_nodos;
	sem_init(&cantNodosAux,0,0);
	sem_init(&semaforo_yama,0,0);


	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR DATA NODE : %i \n",
			config->puerto_dn);
	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR YAMA : %i \n",
			config->puerto_yama);


	if(config->estado_recuperacion==0){
		logInfo("FILE SYSTEM NO SE ENCUENTRA EN ESTADO DE RECUPERACION");
		logInfo("CREANDO ESTRUCTURAS ADMINISTRATIVAS");

		int status =  recuperacionFileSystem();
		if(status==-1){
			logInfo("FILE SYSTEM NO PUEDE RECUPERARSE");
		}
		if(status==1){
			logInfo("FILE SYSTEM SE RECUPERO CORRECTAMENTE");
		}
	}

	else{
		logInfo("FILE SYSTEM SE ENCUENTRA EN ESTADO DE RECUPERACION");
		logInfo("LEVANTANDO ESTRUCTURAS DEL ESTADO ANTERIOR...");

		//ACA ES DONDE DESDE NODOS.BIN, DIRECTORIOS.DAT Y ARCHIVOS.DAT LEVANTO MIS ESTRUCTURAS. --> me parece que levanto las estructuras cuando en la consola se pone format
		//HAY QUE VER SI ME FALTA INFO LE PREGUNTO A YAMA (?)
	}

	logInfo("Creando el hilo para comunicarme con Data Node");
	logInfo("Creando el hilo para comunicarme con YAMA");
	logInfo("Creando el hilo para comunicarme con WORKER");

	ParametrosComunicacion* parametros = setParametrosComunicacion(config->puerto_dn, config->puerto_yama,config->puerto_worker); // Hay que agregar el Puerto de Worker

	pthread_t hiloConsola, hiloDN, hiloYAMA,hiloWorker;

	pthread_create(&hiloDN, NULL, (void*) comunicacionDN, parametros);
	pthread_create(&hiloConsola, NULL, (void*) consolaFileSystem, NULL);
	pthread_create(&hiloYAMA, NULL, (void*) comunicacionYAMA, parametros);
	pthread_create(&hiloWorker, NULL, (void*) comunicacionWorker, parametros);



	pthread_join(hiloWorker, NULL);
	pthread_join(hiloYAMA, NULL);
	pthread_join(hiloConsola, NULL);
	pthread_join(hiloDN, NULL);

	//recibir datos de nodos (dataNode)

	//recibirNodos();
	//cargarNodos();//ejemplo hardcodeado de 2 nodos, para probar las funciones de consola

	//deja que se conecte yama, y se queda esperando

	sem_destroy(&cantNodosAux);
	sem_destroy(&semaforo_yama);
	free(config);
	return EXIT_SUCCESS;
}
