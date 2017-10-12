/*
 * planificacionYAMA.c
 *
 *  Created on: 10/10/2017
 *      Author: utnso
 */

#include "Headers/planificacionYAMA.h"

void agregarWorkerALista(workerParaPlanificar* worker){

	pthread_mutex_lock(&mutexWorkerAPlanificar);
	list_add(listaDeWorkerTotales,worker);
	pthread_mutex_lock(&mutexWorkerAPlanificar);

}


void agregarJObACola(char* job){

	pthread_mutex_lock(&mutexJobsAPlanificar);
	queue_push(jobsAPlanificar,job);
	pthread_mutex_unlock(&mutexJobsAPlanificar);

}

char* retirarJobDeLista(){

	pthread_mutex_lock(&mutexJobsAPlanificar);
	char* job = queue_pop(jobsAPlanificar);
	pthread_mutex_unlock(&mutexJobsAPlanificar);
	return job;
}

t_list* planificacion(t_list* lista fr)

