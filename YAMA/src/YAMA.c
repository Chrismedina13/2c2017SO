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

	t_list* mastersConectados;
	//Archivo de Logs
	crearLog("YAMA.log","YAMA",1,log_level_from_string("INFO"));

	//Archivo de configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
	logInfo("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	logInfo("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	logInfo("Archivo de configuracion Retardo de Planificacion : %i \n", config->retardo);
	logInfo("Archivo de configuracion Algoritmo de Balanceo : %s \n", config->algoritmo_bal);
	logInfo("Archivo de configuracion Puerto YAMA : %i \n", config->puertoYama);

	ParametrosComunicacionConFileSystem* parametrosFileSystem = setParametrosComunicacionConFileSystem(config->puertoFileSystem, config->ipFileSystem);
	ParametrosComunicacionConMaster* parametrosMaster = setParametrosComunicacionConMaster(config->puertoYama,mastersConectados);

	logInfo("Creando hilos para comunicacion con YAMA y FS");

	pthread_t hiloComunicacionConFileSystem;
	pthread_t hiloComunicacionConMaster;

	pthread_create(&hiloComunicacionConFileSystem,NULL,(void*) comunicacionConFileSystem, parametrosFileSystem);
	pthread_create(&hiloComunicacionConMaster,NULL,(void*) comunicacionConMaster, parametrosMaster);


	pthread_join(hiloComunicacionConMaster,NULL);
	pthread_join(hiloComunicacionConFileSystem,NULL);


	free(config);

	return EXIT_SUCCESS;
}
