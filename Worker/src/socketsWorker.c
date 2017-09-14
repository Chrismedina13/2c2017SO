/*
 * socketsWorker.c
 *
 *  Created on: 6/9/2017
 *      Author: utnso
 */
#include "Headers/socketsWorker.h"
#define MAX 100

void comunicacionConMaster(int puertoWorker,t_list* mastersConectados){


	//Creo Servidor Principal
	int socketWorkerServidor;
	socketWorkerServidor = socketServidor(puertoWorker);

	fd_set master;
	fd_set read_fds;
	int fd_max;
	int i;
	int FD_Cliente;
	int bytesRecibidos;
	char buffer[5];

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

					if((FD_Cliente = aceptarYRegistrarSocket(socketWorkerServidor,mastersConectados)) == -1){

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

						logInfo("Recibi de Master: %s",buffer);
					}
				}

				}
			}

		}
	}

int socketServidor(int puerto){

	int FDSocket;
	int FDBind;
	int FDListen;
	int FDAccept; // El que se usa para enviar y recibir
	int size;

	FDSocket = socket(AF_INET,SOCK_STREAM,0);

	if(FDSocket == -1){
		puts("Error en la creacion del socket");
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(puerto);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero),'\0',8);

	FDBind = bind(FDSocket,(struct sockaddr *)&my_addr,sizeof(struct sockaddr));

	if(FDBind == -1){

		puts("Error en el bind");
	}

	FDListen = listen(FDSocket,CONEXIONES_MAXIMAS);

	if(FDListen == -1 ){

		puts("Error en el listen");
	}

	//crea El servidor , no la acepta

	return FDSocket;
}

int aceptarYRegistrarSocket(int socketServidor,t_list* conectados){

	struct sockaddr cliente;
	socklen_t longitudCliente = sizeof(cliente);
	int newSocketFileDescriptor;

	newSocketFileDescriptor = accept(socketServidor, &cliente,
			&longitudCliente);

	if (newSocketFileDescriptor == -1) {
		logError("Socket accept failed FD: %d\n", socketServidor);
		return 0;
	}

	list_add(conectados,newSocketFileDescriptor);

	return newSocketFileDescriptor;


}




