/*
 * planificacionYAMA.h
 *
 *  Created on: 10/10/2017
 *      Author: utnso
 */

#ifndef HEADERS_PLANIFICACIONYAMA_H_
#define HEADERS_PLANIFICACIONYAMA_H_

#include "yama.h"
#include <semaphore.h>
#include "commons/collections/queue.h"
#include "SO_lib/estructuras.h"

pthread_mutex_t mutexJobsAPlanificar;
pthread_mutex_t mutexWorkerAPlanificar;

void agregarJObACola(char* job);
char* retirarJobDeLista();
void agregarWorkerALista(workerParaPlanificar* worker);




#endif /* HEADERS_PLANIFICACIONYAMA_H_ */
