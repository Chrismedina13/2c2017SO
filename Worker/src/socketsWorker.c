/*
 * socketsWorker.c
 *
 *  Created on: 6/9/2017
 *      Author: utnso
 */
#include "Headers/socketsWorker.h"

void comunicacionConMaster(int puertoWorker){

	int socketWorker;

	socketWorker = socketServidor(puertoWorker);

	logInfo("SocketCliente = %d \n", socketWorker);

	logInfo("Se conecto un Master su FD es el  = %d\n",socketWorker);

	if(send(socketWorker,"Hola Master",11,0) != -1){

		puts("Mensaje a Master enviado correctamente");
	}
	else{
		puts("Error en el envio");
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

	size = sizeof(struct sockaddr_in);
	FDAccept = accept(FDSocket,(struct sockaddr *)&their_addr, &size);

	if(FDAccept == -1 ){

		puts("Error en el Accept");
	}

	return FDAccept;
}




