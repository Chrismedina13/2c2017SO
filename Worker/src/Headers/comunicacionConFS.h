/*
 * comunicacionConFS.h
 *
 *  Created on: 19/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONFS_H_
#define HEADERS_COMUNICACIONCONFS_H_


#include <SO_lib/sockets.h>

typedef struct ParametrosComunicacionConFileSystem{
	int puerto;
	int ip;
} ParametrosComunicacionConFileSystem;

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* parametros);
ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(int puerto,char* ip);


#endif /* HEADERS_COMUNICACIONCONFS_H_ */
