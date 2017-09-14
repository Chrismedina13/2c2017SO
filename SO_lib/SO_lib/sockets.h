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

#define CONEXIONES_MAXIMAS 100

struct sockaddr_in my_addr;
struct sockaddr_in their_addr;

int lib_socketServidor(int puerto);


#endif /* SOCKETS_H_ */
