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

void comunicacionConMaster(ParametrosComunicacionConMaster* parametrosMaster) {

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

	FD_SET(socketWorkerServidor, &master);
	fd_max = socketWorkerServidor;

	for (;;) {

		read_fds = master;
		if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1) {

			logInfo("Error en el select");
		}
		for (i = 0; i <= fd_max; i++) {

			if (FD_ISSET(i, &read_fds)) {

				if (i == socketWorkerServidor) {

					if ((FD_Cliente = lib_aceptarYRegistrarSocket(
							socketWorkerServidor)) == -1) {

						logInfo("Error en el aceept despues del select");

					} else {
						FD_SET(FD_Cliente, &master);
						if (FD_Cliente > fd_max) {
							fd_max = FD_Cliente;
						}
						logInfo(
								"Nueva conexion del socket cliente Master de FD: %i",
								FD_Cliente);
					}
				} else {

					//Recibo datos de algun cliente
					if ((bytesRecibidos = recv(i, buffer, 5, 0)) <= 0) {
						if (bytesRecibidos == 0) {
							logInfo("Conexion cerrada del FD : %i", i);

						}
						close(i);
						FD_CLR(i, &master);

					} else {
						int codigo = deserializarINT(buffer);
						logInfo(" Worker Recibe de Master: %s", codigo);


						char pesoMensaje[4];
						int tamanio;
						char* mensaje;
						 int FDMaster = i;

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
								infoParaWorker* info = deserializarInfoParaWorker(mensaje);
								logInfo("Nodo %i\nBloque %i\n,BytesOcupados %i\n,ArchivoTemporal %s",info->nodo,info->bloque,info->bytesOcupados,info->archivoTemporal);

							}
							break;
						case REDUCCION_TEMPORALES:
							break;
						case SOL_REDUCCION_GLOBAL:
							break;
						case SOL_ALMACENADO_FINAL:
							break;

						}



						//deberia divir la info 3 cosas:el cod a ejecutar,el origen de datos y el destino
					/*	int v_origen;
						char* v_destino;
						v_origen = info->bloque;
						v_destino = info->archivoTemporal;*/
						//crear_script(info->script);crear el archivo de transformacion
						/////////////////////////////////////////hay que hacer forkkkkkk

						/*int pid;
						 int i;
						 int estado;

						 pid = fork();

						 switch(pid)
						 {
						 case -1: // Si pid es -1 quiere decir que ha habido un error
						 perror("No se ha podido crear el proceso hijo\n");
						 break;
						 case 0: // Cuando pid es cero quiere decir que es el proceso hijo
						 for(i=0; i&lt;10; i++)
						 printf("Soy el hijo\n");
						 /*ejecuto el script de transformacion
						 crear_script(info->script);

						 ejecutar_script(v_origen,v_destino);
						 break;
						 default: // Cuando es distinto de cero es el padre
						 for(i=0; i&lt;10; i++)
						 printf("Soy el padre\n");
						 // La función wait detiene el proceso padre y se queda esperando hasta
						 // que termine el hijo
						 wait(estado);
						 printf("Mi proceso hijo ya ha terminado.\n");
						 break;
						 }*/

					}

					////////////////////////////////////////////////////////////
				}
			}

		}
	}

}


ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto) {

ParametrosComunicacionConMaster* parametros = malloc(
		sizeof(ParametrosComunicacionConMaster));
parametros->puertoWorker = puerto;
return parametros;
}



void mensajesEnviadosAMaster(int codigo, int FDMaster) {
//Worker recibe de master
char pesoMensaje[4];
int tamanio;
char* mensaje;

switch (codigo) {
case FIN_TRANSFORMACION:
	logInfo(
			"Woker envia señal de finalización de Transformación(EXITO o FRACASO)");
	Paquete* paqueteTranf = crearPaquete(FIN_TRANSFORMACION, tamanio, mensaje);

	if (enviarPaquete(FDMaster, paqueteTranf) == -1) {
		logInfo("Error en envio de respuesta de Transformacion.");
	}

	destruirPaquete(paqueteTranf);

	break;
case FIN_REDUCCION_LOCAL:
	logInfo(
			"Woker envia señal de finalización de Reducción Local(EXITO o FRACASO)");
	Paquete* paqueteRedLocal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
			mensaje);

	if (enviarPaquete(FDMaster, paqueteRedLocal) == -1) {
		logInfo("Error en envio de respuesta de Red.Local.");
	}

	destruirPaquete(paqueteRedLocal);

	break;

case FIN_REDUCCION_GLOBAL:
	logInfo(
			"Worker envia señal de finalización de Reducción Global(EXITO o FRACASO)");

	Paquete* paqueteRedGlobal = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
			mensaje);

	if (enviarPaquete(FDMaster, paqueteRedGlobal) == -1) {
		logInfo("Error en envio de respuesta de Red.Global.");
	}

	destruirPaquete(paqueteRedGlobal);

	break;
case ALMACENADO_FINAL:
	logInfo(
			"Worker envia señal de finalización de Almacenamiento Final(EXITO o FRACASO).");

	Paquete* paqueteAlmacenado = crearPaquete(FIN_REDUCCION_LOCAL, tamanio,
			mensaje);

	if (enviarPaquete(FDMaster, paqueteAlmacenado) == -1) {
		logInfo("Error en envio de respuesta de Almacenado Final");
	}

	destruirPaquete(paqueteAlmacenado);

	break;

default:
	break;
}
}
