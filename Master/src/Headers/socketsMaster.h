/*
 * socketsMaster.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_SOCKETSMASTER_H_
#define HEADERS_SOCKETSMASTER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>


int SocketCliente(const char* ip, int port);



#endif /* HEADERS_SOCKETSMASTER_H_ */
