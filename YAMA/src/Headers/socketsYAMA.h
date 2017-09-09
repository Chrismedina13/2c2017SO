/*
 * socketsYAMA.h
 *
 *  Created on: 8/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_SOCKETSYAMA_H_
#define HEADERS_SOCKETSYAMA_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define  CONEXIONES_MAXIMAS 30 //PUDE MODIFICARSE SEGUN LO QUE PIDAN

struct sockaddr_in my_addr;
struct sockaddr_in their_addr;

int socketServidor(int puerto);

#endif /* HEADERS_SOCKETSYAMA_H_ */
