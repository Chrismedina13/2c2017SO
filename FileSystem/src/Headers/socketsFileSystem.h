/*
 * socketsFileSystem.h
 *
 *  Created on: 8/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_SOCKETSFILESYSTEM_H_
#define HEADERS_SOCKETSFILESYSTEM_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define  CONEXIONES_MAXIMAS 30 //PUDE MODIFICARSE SEGUN LO QUE PIDAN

struct sockaddr_in my_addr;
struct sockaddr_in their_addr;

int socketServidor(int puerto);
#endif /* HEADERS_SOCKETSFILESYSTEM_H_ */

