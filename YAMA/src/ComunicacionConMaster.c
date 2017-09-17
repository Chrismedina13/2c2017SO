/*
 * ComunicacionConMaster.c
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "Headers/ComunicacionConMaster.h"

void comunicacionConMaster(ParametrosComunicacionConMaster* parametros){

	//Creo Servidor Principal
	int socketYAMAServidor;
	socketYAMAServidor = lib_socketServidor(parametros->puerto);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[20];

	FD_SET(socketYAMAServidor,&master);
	fd_max = socketYAMAServidor;

	for(;;){

		read_fds = master;
		if(select(fd_max+1, &read_fds,NULL,NULL,NULL) == -1){

			logInfo("Error en el select");
		}
		else{

			logInfo("Select creado correctamente");
		}
		for(i = 0;i <= fd_max; i++){


			if(FD_ISSET(i,&read_fds)){

				if(i == socketYAMAServidor){

					if((FD_Cliente = lib_aceptarYRegistrarSocket(socketYAMAServidor,parametros->mastersConectados)) == -1){

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
					if((bytesRecibidos = recv(i,buffer,20,0)) <= 0){
						if(bytesRecibidos == 0){
							logInfo("Conexion cerrada del FD : %i",i);

						}
						close(i);
						FD_CLR(i,&master);

					}else{

						logInfo("Recibi de Master: %s",buffer);
					}
				}

				}
			}

		}
	}

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto,t_list* mastersConectados){
	ParametrosComunicacionConMaster* parametros = malloc(sizeof(ParametrosComunicacionConMaster));
	parametros->puerto = puerto;
	parametros->mastersConectados = mastersConectados;
	return parametros;
}



