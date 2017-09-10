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

	//Archivo de logs
	crearLog("FileSystem.log", "FS", 1, log_level_from_string("INFO"));

	//Configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);

	printf("Archivo de configuracion puerto FILESYSTEM : %i \n",
			config->puerto);

	//socketServer
	int FDServidor = socketServidor(config->puerto);

	printf("Se conecto un DataNode su FD es el  = %d\n", FDServidor);

	logInfo("FD del DataNode : %i \n", FDServidor);

	if (send(FDServidor, "Hola DataNode", 13, 0) != -1) {

		puts("Mensaje a DataNode enviado correctamente");

		logInfo("Comunicacion con DataNode establecida");
	} else {
		puts("Error en el envio");

		logInfo("Error en la comunicacion con DataNode");

	}

	free(config);

	return EXIT_SUCCESS;
}
