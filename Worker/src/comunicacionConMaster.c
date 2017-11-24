/*
 * socketsWorker.c
 *
 *  Created on: 6/9/2017
 *      Author: utnso
 */
#include "Headers/comunicacionConMaster.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 100


void comunicacionConMaster(ParametrosComunicacionConMaster* parametrosMaster){


	//Creo Servidor Principal
	int socketWorkerServidor;
	socketWorkerServidor = lib_socketServidor(parametrosMaster->puertoWorker);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[4];

	FD_SET(socketWorkerServidor,&master);
	fd_max = socketWorkerServidor;

	for(;;){

		read_fds = master;
		if(select(fd_max+1, &read_fds,NULL,NULL,NULL) == -1){

			logInfo("Error en el select");
		}
		for(i = 0;i <= fd_max; i++){

			if(FD_ISSET(i,&read_fds)){

				if(i == socketWorkerServidor){

					if((FD_Cliente = lib_aceptarYRegistrarSocket(socketWorkerServidor)) == -1){

						logInfo("Error en el aceept despues del select");

					}else{
						FD_SET(FD_Cliente,&master);
						if(FD_Cliente > fd_max){
							fd_max = FD_Cliente;
						}
						logInfo("Nueva conexion del socket cliente Master de FD: %i",FD_Cliente);
					}
				}else{

					//Recibo datos de algun cliente
					if((bytesRecibidos = recv(i,buffer,5,0)) <= 0){
						if(bytesRecibidos == 0){
							logInfo("Conexion cerrada del FD : %i",i);

						}
						close(i);
						FD_CLR(i,&master);

					}else{
						int codigo = deserializarINT(buffer);
						logInfo("Recibi de Master: %s",buffer);
						mensajesRecibidosDeMaster(codigo, i);
						//hay que hacer forkkkkkk
					}
				}

				}
			}

		}
	}




ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto){

	ParametrosComunicacionConMaster* parametros = malloc(sizeof(ParametrosComunicacionConMaster));
	parametros->puertoWorker = puerto;
	return parametros;
}

void mensajesRecibidosDeMaster(int codigo, int FDMaster) {
//Worker recibe de master
	char pesoMensaje[4];
	int tamanio;
	char* mensaje;

	switch (codigo) {
	case TAREA_WORKER:
		recv(FDMaster, pesoMensaje, 4, 0);

		tamanio = deserializarINT(pesoMensaje);

		logInfo("Tamanio de lo que recibo %i", tamanio);

		mensaje = malloc(tamanio + 1);

		mensaje[tamanio] = '\0';

		if (recv(FDMaster, mensaje, tamanio, 0) == -1) {

			logInfo("Error en la recepcion de Info de Master.");

		} else {
//falta desereaalizar

			infoParaWorker * info = deserializarInfoParaWorker(mensaje);
		}
		break;
	case REDUCCION_TEMPORALES:
		break;
	case SOL_REDUCCION_GLOBAL:
	break;
	case SOL_ALMACENADO_FINAL:
	break;

	}
}
