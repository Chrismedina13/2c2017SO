/*
 * comunicacionConFileSystem.h
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONFILESYSTEM_H_
#define HEADERS_COMUNICACIONCONFILESYSTEM_H_


#include <SO_lib/sockets.h>

typedef struct ParametrosComunicacionConFileSystem{
	int puerto;
	int ip;
} ParametrosComunicacionConFileSystem;

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* parametros);
ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(int puerto,char* ip);


#endif /* HEADERS_COMUNICACIONCONFILESYSTEM_H_ */
