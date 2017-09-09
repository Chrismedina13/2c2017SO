/*
 ============================================================================
 Name        : DataNode.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Headers/DataNode.h"

int main(void) {
	//Archivo de Logs

	crearLog("DataNode.log", "YAMA", 1, log_level_from_string("INFO"));

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	logInfo("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	logInfo("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	logInfo("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	logInfo("Archivo de configuracion nombre nodo : %s \n", config->nombreNodo);
	logInfo("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);

	//socketClienteParaFileSystem

	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = SocketCliente(config->ipFileSystem,config->puertoFileSystem);

	logInfo("SocketCliente = %d \n",FDsocketClienteFileSystem);

	char buffer[13];
	if(recv(FDsocketClienteFileSystem,buffer,13,0) != -1){
		logInfo("Se recibio: %s",buffer);
	}
	free(config);



	return EXIT_SUCCESS;
}
