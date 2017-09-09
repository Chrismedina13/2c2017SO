/*
 ============================================================================
 Name        : FileSystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Headers/FileSystem.h"

int main(int argc, char *argv[]) {

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto FILESYSTEM : %i \n", config->puerto);

	//socketServer
	int FDServidor = socketServidor(config->puerto);

	printf("Se conecto un DataNode su FD es el  = %d\n",FDServidor);

	if(send(FDServidor,"Hola DataNode",13,0) != -1){

		puts("Mensaje a DataNode enviado correctamente");
	}
	else{
		puts("Error en el envio");
	}

	free(config);



	return EXIT_SUCCESS;
}
