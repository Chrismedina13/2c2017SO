/*
 * socketsWorker.h
 *
 *  Created on: 6/9/2017
 *      Author: utnso
 */

#ifndef SOCKETSWORKER_H_
#define SOCKETSWORKER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define  CONEXIONES_MAXIMAS 30 //PUDE MODIFICARSE SEGUN LO QUE PIDAN

struct sockaddr_in my_addr;
struct sockaddr_in their_addr;

int socketServidor(int puerto);
#endif /* SOCKETSWORKER_H_ */
