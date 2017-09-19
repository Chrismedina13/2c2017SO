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

int lib_aceptarYRegistrarSocket(int socketServidor){

	struct sockaddr cliente;
	socklen_t longitudCliente = sizeof(cliente);
	int newSocketFileDescriptor;
	newSocketFileDescriptor = accept(socketServidor, &cliente,&longitudCliente);

	if (newSocketFileDescriptor == -1) {
		puts("Socket accept failed" );
		return 0;
	}

	return newSocketFileDescriptor;
}


int lib_SocketCliente(const char* ip, int port) {
	int FDCliente;
	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);
	memset(&(serv_addr.sin_zero),'\0',8);

	FDCliente= socket(PF_INET, SOCK_STREAM, 0);
	if (FDCliente == -1) {
		puts("Error en la creacion del socket Cliente");
		return -1;
	}

	if (connect(FDCliente, (struct sockaddr*) &serv_addr,sizeof(serv_addr)) == -1) {
		printf("Connect failed FD: %d address: %s port: %d\n",FDCliente, ip, port);
		return -1;
	}

	return FDCliente;
}

