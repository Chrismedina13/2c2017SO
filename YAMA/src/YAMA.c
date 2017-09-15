/*
 ============================================================================
 Name        : YAMA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Headers/yama.h"


int main(int argc, char *argv[]) {

	//Archivo de Logs
	crearLog("YAMA.log","YAMA",1,log_level_from_string("INFO"));

	//Archivo de configuracion
	Configuracion *config = leerArchivoDeConfiguracion(ARCHIVO_CONFIGURACION);
	logInfo("Archivo de configuracion ip fileSystem : %s \n", config->ipFileSystem);
	logInfo("Archivo de configuracion puerto fileSystem : %i \n", config->puertoFileSystem);
	logInfo("Archivo de configuracion Retardo de Planificacion : %i \n", config->retardo);
	logInfo("Archivo de configuracion Algoritmo de Balanceo : %s \n", config->algoritmo_bal);
	logInfo("Archivo de configuracion Puerto YAMA : %i \n", config->puertoYama);




	//socketServer
		int FDServidor = socketServidor(config->puertoYama);

		logInfo("Se conecto un Master su FD es el  = %d\n",FDServidor);

		if(send(FDServidor,"Soy YAMA",8,0) != -1){

			logInfo("Mensaje a Master enviado correctamente");
		}
		else{
			logInfo("Error en el envio");
		}


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
