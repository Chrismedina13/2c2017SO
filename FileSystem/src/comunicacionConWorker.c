/*
 * comunicacionConWorker.c
 *
 *  Created on: 19/9/2017
 *      Author: utnso
 */

#include "Headers/comunicacionConYama.h"
#include "Headers/comunicacionConDN.h"
#include "Headers/comunicacionConWorker.h"


void comunicacionWorker(ParametrosComunicacion* parametros){


		int socketFSServidor;
		socketFSServidor = lib_socketServidor(parametros->puertoFS_worker);




		//Creo Servidor Principal

		fd_set master;
		fd_set read_fds;
		int fd_max;
		int i;
		int FD_Cliente;
		int bytesRecibidos;
		char buffer[4];

		FD_SET(socketFSServidor, &master);
		fd_max = socketFSServidor;

		for (;;) {

			read_fds = master;
			if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1) {

				logInfo("Error en el select");
			}
			for (i = 0; i <= fd_max; i++) {

				if (FD_ISSET(i, &read_fds)) {

					if (i == socketFSServidor) {

						if ((FD_Cliente = lib_aceptarYRegistrarSocket(
								socketFSServidor)) == -1) {

							logInfo("Error en el acept despues del select");

						} else {
							FD_SET(FD_Cliente, &master);
							if (FD_Cliente > fd_max) {
								fd_max = FD_Cliente;
							}
							logInfo(
									"Nueva conexion del socket cliente WORKER de FD: %i",
									FD_Cliente);
						}
					} else {

						//Recibo datos de algun cliente
						if ((bytesRecibidos = recv(i, buffer, 4, 0)) <= 0) {
							if (bytesRecibidos == 0) {

								logInfo("Conexion cerrada del FD : %i", i);//VER QUE HACER ACA
							}
							close(i);
							FD_CLR(i, &master);

						} else {

							int codigo = deserializarINT(buffer);
							logInfo("Recibi de WORKER: %i", codigo);
							mensajesRecibidosDeWorker(codigo, i);

						}
					}

				}
			}

		}
	}





void mensajesRecibidosDeWorker(int codigo, int FD_DN) {

	char pesoMensaje[8];
	int tamanio;
	char * mensaje;
	script * ArchivoFinal;
	switch (codigo) {
	case ALMACENADO_FINAL:

		                        recv(FD_DN,pesoMensaje,4,0);
								tamanio= deserializarINT(pesoMensaje);
								logInfo("tamanio de lo que recibo %i", tamanio);
								mensaje = malloc(tamanio + 1);
								mensaje[tamanio] = '\0';
								recv(FD_DN, mensaje,tamanio,0);
								ArchivoFinal=deserilizarScript(mensaje);
								logInfo("Se reibio el archivo final: %s",  ArchivoFinal->nombre);
                                free(mensaje);

		break;
	case SALUDO:

			                        recv(FD_DN,pesoMensaje,4,0);
									tamanio= deserializarINT(pesoMensaje);
									logInfo("tamanio de lo que recibo %i", tamanio);
									mensaje = malloc(tamanio + 1);
									mensaje[tamanio] = '\0';
									recv(FD_DN, mensaje,tamanio,0);
								    logInfo("Se reibio: %s",  mensaje);
	                                free(mensaje);

			break;


	    default:

						logInfo("Llego un mensaje que FS no sabe traducir.");
						break;
	}
}

