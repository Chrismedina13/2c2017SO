/*
 * sockets.c

 *
 *  Created on: 14/9/2017
 *      Author: utnso
 */

#include "sockets.h"

int lib_socketServidor(int puerto){

	int FDSocket;
	int FDBind;
	int FDListen;

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

int lib_aceptarYRegistrarSocket(int socketServidor,t_list* conectados){

	struct sockaddr cliente;
	socklen_t longitudCliente = sizeof(cliente);
	int newSocketFileDescriptor;

	newSocketFileDescriptor = accept(socketServidor, &cliente,&longitudCliente);

	if (newSocketFileDescriptor == -1) {
		printf("Socket accept failed FD: %d\n", socketServidor);
		return 0;
	}

	list_add(conectados,newSocketFileDescriptor);

	return newSocketFileDescriptor;
}
