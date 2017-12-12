/*
 * Worker.h
 *
 *  Created on: 4/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_WORKER_H_
#define HEADERS_WORKER_H_

#include "comunicacionConFileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include "pthread.h"
#include "comunicacionConMaster.h"
#include "configuracionW.h"
#include "comunicacionConWorkerCliente.h"
#include "comunicacionConWorkerServidor.h"

int FD_Filesystem;
int id_nodo;
int FD_WorkerServidor;
t_list* listaDeArchivosTemporalesLocales;
int cantidadDeWorkersServidoresConectadosAlWorkerElegido;

#define ARCHIVO_CONFIGURACION "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/Nodo.conf"




#endif /* HEADERS_WORKER_H_ */
