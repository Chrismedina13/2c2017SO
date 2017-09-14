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

	//Archivo de Logs
	crearLog("Master.log", "MASTER", 1, log_level_from_string("INFO"));

	//Archivo de configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo("Archivo de configuracion IP YAMA : %s \n", config->ipYama);
	logInfo("Archivo de configuracion YAMA PUERTO : %i \n", config->puertoYama);

	//socketsCliente
	int FDsocketClienteWorker;
	FDsocketClienteWorker = SocketCliente("127.0.0.1", 5050); //127.0.0.1 es la ip local , 5050 puerto del worker

	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = SocketCliente(config->ipYama, config->puertoYama);

	logInfo("SocketCliente Worker= %d \n", FDsocketClienteWorker);

	logInfo("SocketCliente YAMA = %d \n", FDsocketClienteYAMA);

	char buffer[11];
	send(FDsocketClienteWorker, "Holaa", 5, 0);

	recv(FDsocketClienteWorker,buffer,11,0);
	logInfo("Se recibio: %s", buffer);

	char buffer2[8];
	recv(FDsocketClienteYAMA, buffer2, 8, 0);
	logInfo("Se recibio: %s", buffer2);

	free(config);
	return EXIT_SUCCESS;
}
