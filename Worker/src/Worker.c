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

int main(int argc, char *argv[]) {

	//Archivo de logs
	crearLog("Worker.log","WORKER",1,log_level_from_string("INFO"));

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);


	ParametrosComunicacionConFileSystem* parametrosFileSystem = setParametrosComunicacionConFileSystem(config->puertoFileSystemW, config->ipFileSystem);
	ParametrosComunicacionConMaster* parametrosMaster = setParametrosComunicacionConMaster(config->puertoWorker);

	pthread_t hiloMaster;
	pthread_create(&hiloMaster, NULL, (void*) comunicacionConMaster, parametrosMaster);
	pthread_join(hiloMaster, NULL);

	int terminoReduccionGlobal = 1;

	if(terminoReduccionGlobal){
		pthread_t hiloFS;
		pthread_create(&hiloFS, NULL, (void*) comunicacionConFileSystem, parametrosFileSystem);
		pthread_join(hiloFS, NULL);

		}
	free(config);
	return EXIT_SUCCESS;
}
