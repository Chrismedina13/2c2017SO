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

int receive_basic(int fd_dn, int tamanio_parcial)
{
    int size_recv , total_size= 0;
    char chunk[tamanio_parcial];

    //loop
    while(1)
    {
        memset(chunk ,0 , tamanio_parcial);  //clear the variable
        if((size_recv =  recv(fd_dn , chunk , tamanio_parcial , 0) ) < 0)
        {
            break;
        }
        else
        {
            total_size += size_recv;
            printf("%s" , chunk);
        }
    }

    return total_size;
}


char* recv_timeout(int FD_DN , int timeout, int CHUNK_SIZE)
{
	char* bloque=string_new();
    int size_recv , total_size= 0;
    struct timeval begin , now;
    char chunk[CHUNK_SIZE];
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
            string_append(&bloque,chunk);
            //reset beginning time
            gettimeofday(&begin , NULL);
        }
    }

    return bloque;
}
