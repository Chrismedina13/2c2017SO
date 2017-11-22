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
	logInfo("Leo  %s \n",ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	printf("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	printf("Archivo de configuracion puerto fileSystem para DATA NODE : %i \n", config->puertoFileSystemDN);
	printf("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	printf("Archivo de configuracion nombre nodo : %s \n", config->nombreNodo);
	printf("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);
	printf("Archivo de configuracion puerto fileSystem para WORKER : %i \n", config->puertoFileSystemW);

	logInfo("Comunicacion con los Masters");
	logInfo("Comunicacion con los FILESYSTEM");

	///pthread_t hiloMaster;

	ParametrosComunicacionConFileSystem* parametrosFileSystem = setParametrosComunicacionConFileSystem(config->puertoFileSystemW, config->ipFileSystem);
	//ParametrosComunicacionConMaster* parametrosMaster = setParametrosComunicacionConMaster(config->puertoWorker);

	//pthread_create(&hiloMaster, NULL, (void*) comunicacionConMaster, parametrosMaster);



	//Creo esta funcion en donde se encarga la comunicacion con el master aca iria el select y los works

	int terminoReduccionGlobal = 1;
	if(terminoReduccionGlobal){

		pthread_t hiloFS;
	pthread_create(&hiloFS, NULL, (void*) comunicacionConFileSystem, parametrosFileSystem);
	pthread_join(hiloFS, NULL);



	}

   //pthread_join(hiloMaster, NULL);

	free(config); // no estoy seguro si se libera asi
	return EXIT_SUCCESS;
}
