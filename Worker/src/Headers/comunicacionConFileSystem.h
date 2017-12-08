/*
 * comunicacionConFileSystem.h
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONFILESYSTEM_H_
#define HEADERS_COMUNICACIONCONFILESYSTEM_H_


#include <SO_lib/sockets.h>
#include <SO_lib/Protocolo.h>
#include <SO_lib/estructuras.h>
#include <SO_lib/serializacion.h>

typedef struct ParametrosComunicacionConFileSystem{
	int puerto;
	int ip;
} ParametrosComunicacionConFileSystem;

void comunicacionConFileSystem(ParametrosComunicacionConFileSystem* parametros);
ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(int puerto,char* ip);

void enviarAlmacenadoFinal(char * nombre_archivo , char* contenido, int FD_FS);

void mensajesEnviadosAFileSystem(int codigo, int FD_FileSystem, char* mensaje, int tamanio);
#endif /* HEADERS_COMUNICACIONCONFILESYSTEM_H_ */
