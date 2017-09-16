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

#include "Headers/ComunicacionConYama.h"

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

	logInfo("Creando el hilo para comunicarme con Yama");

	ParametrosComunicacionYAMA* parametrosYAMA = setParametrosComunicacionYAMA(config->puertoYama,config->ipYama);

	pthread_t hiloYama;
	pthread_create(&hiloYama, NULL, (void*) comunicacionYama, parametrosYAMA);

	logInfo("SocketCliente Worker= %d \n", FDsocketClienteWorker);

	send(FDsocketClienteWorker, "Holaa", 5, 0);

	pthread_join(hiloYama,NULL);

	free(config);
	return EXIT_SUCCESS;
}
