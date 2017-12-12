/*
 * sendAndReceive.c
 *
 *  Created on: 1/12/2017
 *      Author: utnso
 */

#include "sendAndReceive.h"
//#define CHUNK_SIZE 1024

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>

//alternativa a recv (en bloques mas pequenios), es lento ya que si el tamanio total no es multiplo de tamanio_parcial, el ultimo recv toma 30 segundos en completar

char* receive_basic(int fd_dn, int tamanio)
{
    int size_recv , total_size= 0;
    char chunk[tamanio];
    char* bloque;

    if((size_recv =  recv(fd_dn , bloque , tamanio , MSG_WAITALL))>0){
    	return bloque;
    }
    else{
    	return ("error");
    }
}


char* recv_timeout(int FD_DN , int timeout, int CHUNK_SIZE)
{
	char* bloque=NULL;
    int size_recv , total_size= 0;
    struct timeval begin , now;
    char chunk[CHUNK_SIZE]; //tamanio de los bloques que va recibiendo
    double timediff;

    //make socket non blocking
    fcntl(FD_DN, F_SETFL, O_NONBLOCK);

    //beginning time
    gettimeofday(&begin , NULL);

    while(1)
    {
        gettimeofday(&now , NULL);

        //time elapsed in seconds
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);

        //if you got some data, then break after timeout
        if( total_size > 0 && timediff > timeout )
        {
            break;
        }

        //if you got no data at all, wait a little longer, twice the timeout
        else if( timediff > timeout*2)
        {
            break;
        }

        memset(chunk ,0 , CHUNK_SIZE);  //clear the variable
        if((size_recv =  recv(FD_DN , chunk , CHUNK_SIZE , 0) ) < 0)
        {
            //if nothing was received then we want to wait a little before trying again, 0.1 seconds
            usleep(100000);
        }
        else
        {
            total_size += size_recv;
          	realloc(bloque, total_size);
          	memcpy(bloque+total_size-size_recv, &chunk, size_recv);
            //reset beginning time
            gettimeofday(&begin , NULL);
        }
    }

    return bloque;
}
