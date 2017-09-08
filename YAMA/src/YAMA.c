/*
 ============================================================================
 Name        : YAMA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Headers/yama.h"
#include "Headers/socketsYAMA.h"


int main(int argc, char *argv[]) {

	//Archivo de Logs
	crearLog("YAMA.log","YAMA",1,log_level_from_string("INFO"));

	logInfo("Prueba de logInfo");
	logError("Prueba de logError");



	//Archivo de configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
	printf("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	printf("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	printf("Archivo de configuracion Retardo de Planificacion : %i \n", config->retardo);
	printf("Archivo de configuracion Algoritmo de Balanceo : %s \n", config->algoritmo_bal);
	printf("Archivo de configuracion Algoritmo de Balanceo : %i \n", config->puertoYama);

	logInfo("Archivo de configuracion IP FileSystem : %s \n", config->ipFileSystem);
	logInfo("Archivo de configuracion PUERTO FileSystem : %i \n", config->puertoFileSystem);


	//socketServer
		int FDServidor = socketServidor(config->puertoYama);

		printf("Se conecto un Master su FD es el  = %d\n",FDServidor);

		if(send(FDServidor,"Hola Master",11,0) != -1){

			puts("Mensaje a Master enviado correctamente");
		}
		else{
			puts("Error en el envio");
		}



	return EXIT_SUCCESS;
}
