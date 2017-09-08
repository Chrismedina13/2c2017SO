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
	crearLog("Master.log", "YAMA", 1, log_level_from_string("INFO"));

	logInfo("HOLAAA"); //Probando logInfo
	logError("Muestra Error"); //Probando logError

	//Archivo de configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	logInfo("Archivo de configuracion IP YAMA : %s \n", config->ipYama);
	logInfo("Archivo de configuracion YAMA PUERTO : %i \n", config->puertoYama);

	//socketClienteParaWorker
	int FDsocketClienteWorker;
	FDsocketClienteWorker = SocketCliente("127.0.0.1", 5050); //127.0.0.1 es la ip local , 5050 puerto del worker

	printf("SocketCliente = %d \n", FDsocketClienteWorker);

	char buffer[11];
	recv(FDsocketClienteWorker, buffer, 11, 0);
	logInfo("Se recibio: %s", buffer);

	//socketClienteParaYAMA
	int FDsocketClienteYAMA;
	FDsocketClienteYAMA = SocketCliente("127.0.0.1", 5300); //127.0.0.1 es la ip local , 5050 puerto de YAMA

	logInfo("SocketCliente = %d \n", FDsocketClienteYAMA);

	char buffer2[11];
	recv(FDsocketClienteYAMA, buffer2, 11, 0);
	printf("Se recibio: %s", buffer2);

	free(config);
	return EXIT_SUCCESS;
}
