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

pthread_mutex_t mutexJobsAPlanificar;
void agregarJObACola(char* job);
char* retirarJobDeLista();




#endif /* HEADERS_PLANIFICACIONYAMA_H_ */
