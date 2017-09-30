/*
 * ComunicacionConMaster.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/ComunicacionConMaster.h"

void comunicacionConMasters(ParametrosComunicacionConMaster* parametros){

	//Creo Servidor Principal
	int socketYAMAServidor;
	socketYAMAServidor = lib_socketServidor(parametros->puerto);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[4];

	FD_SET(socketYAMAServidor,&master);
	fd_max = socketYAMAServidor;

	for(;;){

		read_fds = master;
		if(select(fd_max+1, &read_fds,NULL,NULL,NULL) == -1){

			logInfo("Error en el select");
		}
		for(i = 0;i <= fd_max; i++){

			if(FD_ISSET(i,&read_fds)){

				if(i == socketYAMAServidor){

					if((FD_Cliente = lib_aceptarYRegistrarSocket(socketYAMAServidor)) == -1){

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
					if((bytesRecibidos = recv(i,buffer,4,0)) <= 0){
						if(bytesRecibidos == 0){
							logInfo("Conexion cerrada del FD : %i",i);

						}
						close(i);
						FD_CLR(i,&master);

					}else{

						int codigo = deserializarINT(buffer);
						logInfo("Recibi de Master: %i",codigo);
						mensajesRecibidosDeMaster(codigo,i);
					}
				}

				}
			}

		}
	}

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto){
	ParametrosComunicacionConMaster* parametros = malloc(sizeof(ParametrosComunicacionConMaster));
	parametros->puerto = puerto;
	return parametros;
}

void mensajesRecibidosDeMaster(int codigo, int FDMaster){

	char pesoMensaje[4];
	int tamanio;
	char* mensaje;

	switch (codigo) {
		case NOMBRE_ARCHIVO:
			recv(FDMaster,pesoMensaje,4,0);
			tamanio = deserializarINT(pesoMensaje);
			logInfo("tamanio de lo que recibo %i",tamanio);
			recv(FDMaster, mensaje, tamanio,0);
			logInfo("Se recibio el nombre del archivo:  %s", mensaje);
			break;
		default:
			break;
	}
}
