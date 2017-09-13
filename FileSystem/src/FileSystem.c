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
			config->puerto1);
	printf("Archivo de configuracion puerto FILESYSTEM : %i \n",
			config->puerto2);


	//socketServer
	int FDServidorYAMA = socketServidor(config->puerto1);

	int FDServidorDN = socketServidor(config->puerto2);

	printf("Se conecto un Yama su FD es el  = %d\n", FDServidorYAMA);
	printf("Se conecto un DataNode su FD es el  = %d\n", FDServidorDN);

	logInfo("FD del DataNode : %i \n", FDServidorDN);
	logInfo("FD del Yama : %i \n", FDServidorYAMA);

	if (send(FDServidorDN, "Hola DataNode", 13, 0) != -1) {

		puts("Mensaje a DataNode enviado correctamente");

		logInfo("Comunicacion con DataNode establecida");
	} else {
		puts("Error en el envio a Data Node");

		logInfo("Error en la comunicacion con DataNode");
	}


	if (send(FDServidorYAMA, "Hola YAMA", 13, 0) != -1) {

			puts("Mensaje a DataNode enviado correctamente");

			logInfo("Comunicacion con YAMA establecida");
		} else {
			puts("Error en el envio a YAMA");

			logInfo("Error en la comunicacion con YAMA");

		}

	free(config);

	return EXIT_SUCCESS;
}
