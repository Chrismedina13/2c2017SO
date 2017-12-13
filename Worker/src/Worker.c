/*
 ============================================================================
 Name        : Worker.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Headers/Worker.h"
#include "Headers/comunicacionConMaster.h"
#include "SO_lib/FuncionesAuxiliares.h"

int main(int argc, char *argv[]) {

	//Inicializo las variables globales
	FD_Filesystem = 0;
	id_nodo = 0;
	FD_WorkerServidor = 0;
	listaDeArchivosTemporalesLocales = list_create();

	//Archivo de logs
	crearLog("Worker.log","WORKER",1,log_level_from_string("INFO"));

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	ParametrosComunicacionConFileSystem* parametrosFileSystem = setParametrosComunicacionConFileSystem(config->puertoFileSystemW, config->ipFileSystem);
	ParametrosComunicacionConMaster* parametrosMaster = setParametrosComunicacionConMaster(config->puertoWorker);

	logInfo("Creando hilos para comunicacion con MASTER y FS");
	pthread_t hiloFS;
	pthread_t hiloMaster;

	pthread_create(&hiloMaster, NULL, (void*) comunicacionConMaster, parametrosMaster);
	pthread_create(&hiloFS, NULL, (void*) comunicacionConFileSystem, parametrosFileSystem);


	pthread_join(hiloFS, NULL);
	pthread_join(hiloMaster, NULL);

	free(config);
	return EXIT_SUCCESS;
}
