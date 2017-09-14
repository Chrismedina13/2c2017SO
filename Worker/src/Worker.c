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
#include "Headers/socketsWorker.h"


int main(int argc, char *argv[]) {

	t_list* mastersConectados;

	//Archivo de logs
	crearLog("Worker.log","WORKER",1,log_level_from_string("INFO"));

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
	logInfo("Leo  %s \n",ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	printf("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	printf("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	printf("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	printf("Archivo de configuracion nombre nodo : %s \n", config->nombreNodo);
	printf("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);

	logInfo("Comunicacion con los Masters");

	//Creo esta funcion en donde se encarga la comunicacion con el master aca iria el select y los works
	comunicacionConMaster(config->puertoWorker,mastersConectados);

	free(config); // no estoy seguro si se libera asi
	return EXIT_SUCCESS;
}
