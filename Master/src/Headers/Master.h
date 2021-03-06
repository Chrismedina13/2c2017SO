/*
 * Master.h
 *
 *  Created on: 7/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_MASTER_H_
#define HEADERS_MASTER_H_

#include <stdio.h>
#include <stdlib.h>
#include "configuracion.h"
#include "logMaster.h"
#include "pthread.h"
#include <SO_lib/sockets.h>
#include <SO_lib/Protocolo.h>
#include "ComunicacionConYama.h"
#include <semaphore.h>
#include "SO_lib/sem.h"

#define ARCHIVO_CONFIGURACION "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Master/master.conf"


int FD_YAMA;
int FD_WORKER;
int tamanioLista;
int nro_job;

char* ruta_transformador;
char* ruta_reduccion;

pthread_t * hilosTransformacion;
pthread_t * hilosReduccion;
#endif /* HEADERS_MASTER_H_ */
