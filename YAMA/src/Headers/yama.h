/*
 * Yama.h
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_YAMA_H
#define HEADERS_YAMA_H

#include "configuracion.h"
#include "logYAMA.h"
#include <stdio.h>
#include <stdlib.h>
#include "ComunicacionConMaster.h"
#include "ComunicacionConFileSystem.h"
#include "pthread.h"
#include "commons/collections/queue.h"
#include "commons/string.h"
#include <semaphore.h>
#include "commons/bitarray.h"
#include "SO_lib/FuncionesAuxiliares.h"
#include "tabla_estados.h"
#include "SO_lib/estructuras.h"
#include "signal.h"
t_queue* jobsAPlanificar; // VARIABLE GLOBAL TIPO LISTA DONDE SE GUARDAN LOS JOBS A PLANIFICAR
sem_t semaforoYAMA; //Espera que llegue el job de MASTER para ejecutar la pate de comunicacion con FS
t_list* listaDeWorkerTotales;
t_list* listaDeJobs;
t_list* ipYPuertoWoerkers;
t_list* nodosCaidos;
int variableGlobalIdentificador;
int variableNombreTransformacion;
int VariableReduccionLocal;
int variableReduciionGlobal;
int FDsocketClienteFileSystem;
pthread_mutex_t mutexPlanificacion;
pthread_mutex_t mutexComunicacionConFS;

Job* crearJOB(int FDMAster,char* nombreDelJob);
void rutina(int senial);
int cantNodos;
int cantNodosConectados;

#define ARCHIVO_CONFIGURACION "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/YAMA/yama.conf"
#endif /* HEADERS_YAMA_H_ */
