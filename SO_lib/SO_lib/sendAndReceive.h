/*
 * sendAndReceive.h
 *
 *  Created on: 1/12/2017
 *      Author: utnso
 */

#ifndef SO_LIB_SENDANDRECEIVE_H_
#define SO_LIB_SENDANDRECEIVE_H_

//#define CHUNK_SIZE 1024

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>

//Receiving funcion without timeout

int receive_basic(int fd_dn, int tamanio_parcial);


//Receiving function
int recv_timeout(int s, int timeout);

int receive_correct(int FD_DN);

#endif /* SO_LIB_SENDANDRECEIVE_H_ */
