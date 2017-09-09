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

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto DataNode : %i \n", config->puertoDataNode);
	printf("Archivo de configuracion puerto worker : %i \n", config->puertoWorker);
	printf("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	printf("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	printf("Archivo de configuracion nombre nodo : %s \n", config->nombreNodo);
	printf("Archivo de configuracion ruta data.bin : %s \n", config->rutaDataBin);

	//socketClienteParaFileSystem

	int FDsocketClienteFileSystem;
	FDsocketClienteFileSystem = SocketCliente(config->ipFileSystem,config->puertoFileSystem);

	printf("SocketCliente = %d \n",FDsocketClienteFileSystem);

	char buffer[13];
	if(recv(FDsocketClienteFileSystem,buffer,13,0) != -1){
	printf("Se recibio: %s",buffer);
	}
	free(config);



	return EXIT_SUCCESS;
}
