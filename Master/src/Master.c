/*
 ============================================================================
 Name        : Master.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Headers/Master.h"


int main(int argc, char *argv[]) {

	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion IP YAMA : %s \n", config->ipYama);
	printf("Archivo de configuracion YAMA PUERTO : %i \n", config->puertoYama);

	int FDsocketCliente;

	FDsocketCliente = SocketCliente("127.0.0.1",5050); //127.0.0.1 es la ip local , 5050 puerto del worker

	printf("SocketCliente = %d \n",FDsocketCliente);

	char buffer[11];
	recv(FDsocketCliente,buffer,11,0);
	printf("Se recibio: %s",buffer);

	free(config);
	return EXIT_SUCCESS;
}
