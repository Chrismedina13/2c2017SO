/*
 * sockets.h
 *
 *  Created on: 14/9/2017
 *      Author: utnso
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "commons/collections/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


#define CONEXIONES_MAXIMAS 100

struct sockaddr_in my_addr;
struct sockaddr_in their_addr;

int lib_socketServidor(int puerto);
int lib_aceptarYRegistrarSocket(int socketServidor,t_list* conectados);
int lib_SocketCliente(const char* ip, int port);


#endif /* SOCKETS_H_ */
