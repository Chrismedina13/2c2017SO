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


	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	//semaforos
	int cantNodos= config->cant_nodos;

	SEMAFORODN = make_semaphore(0);
	SEMAFOROYAMA = make_semaphore(0);

	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR DATA NODE : %i \n",
			config->puerto_dn);
	logInfo(
			"Archivo de configuracion PUERTO FILE SYSTEM PARA RECIBIR YAMA : %i \n",
			config->puerto_yama);



	//Recuperacion FileSystem

	if(config->estado_recuperacion==0){
		logInfo("FILE SYSTEM NO SE ENCUENTRA EN ESTADO DE RECUPERACION");
		//logInfo("CREANDO ESTRUCTURAS ADMINISTRATIVAS");
	}

	if(config->estado_recuperacion==1){
		logInfo("FILE SYSTEM SE ENCUENTRA EN ESTADO DE RECUPERACION");
		logInfo("LEVANTANDO ESTRUCTURAS DEL ESTADO ANTERIOR...");


		int status =  recuperacionFileSystem();
		if(status==-1){
			logInfo("FILE SYSTEM NO PUEDE RECUPERARSE");

			//hacer algo extra?
		}

		if(status==1){
			logInfo("FILE SYSTEM SE RECUPERO CORRECTAMENTE");
		}
	}

	logInfo("Creando el hilo para comunicarme con Data Node");
	logInfo("Creando el hilo para comunicarme con YAMA");
	logInfo("Creando el hilo para comunicarme con WORKER");

	ParametrosComunicacion* parametros = setParametrosComunicacion(config->puerto_dn, config->puerto_yama,config->puerto_worker); // Hay que agregar el Puerto de Worker

	//incian los hilos

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

	//sem_destroy(&cantNodosAux);
	//sem_destroy(&semaforo_yama);
	free(config);
	return EXIT_SUCCESS;
}
