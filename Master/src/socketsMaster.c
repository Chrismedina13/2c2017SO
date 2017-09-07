/*
 * socketsMaster.c
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */
#include "Headers/socketsMaster.h"

int SocketCliente(const char* ip, int port) {
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
