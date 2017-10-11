/*
 * ComunicacionConFileSystem.h
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */



#ifndef HEADERS_COMUNICACIONCONFILESYSTEM_H_
#define HEADERS_COMUNICACIONCONFILESYSTEM_H_
#include <SO_lib/sockets.h>
#include <pthread.h>
#include <commons/collections/queue.h>
#include "planificacionYAMA.h"

typedef struct ParametrosComunicacionConFileSystem{
	int puerto;
	int ip;
} ParametrosComunicacionConFileSystem;

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* parametros);
ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(int puerto,char* ip);

#endif /* HEADERS_COMUNICACIONCONFILESYSTEM_H_ */
