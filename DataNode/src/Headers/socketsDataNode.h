/*
 * socketsDataNode.h
 *
 *  Created on: 8/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_SOCKETSDATANODE_H_
#define HEADERS_SOCKETSDATANODE_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>


int SocketCliente(const char* ip, int port);


#endif /* HEADERS_SOCKETSDATANODE_H_ */
