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

char* receive_basic(int fd_dn, int tamanio_parcial);


//Receiving function
char* recv_timeout(int FD_DN , int timeout, int CHUNK_SIZE);
char* receive_correct(int FD_DN, int CHUNK_SIZE);

#endif /* SO_LIB_SENDANDRECEIVE_H_ */
