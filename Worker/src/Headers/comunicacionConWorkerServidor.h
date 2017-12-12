/*
 * comunicacionConWorkerServidor.h
 *
 *  Created on: 11/12/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONWORKERSERVIDOR_H_
#define HEADERS_COMUNICACIONCONWORKERSERVIDOR_H_


#include <SO_lib/sockets.h>
#include <SO_lib/Protocolo.h>
#include <SO_lib/estructuras.h>
#include <SO_lib/serializacion.h>
#include "Worker.h"

typedef struct ParametrosComunicacionConWorkerServidor{
	int puerto;
	int ip;
} ParametrosComunicacionConWorkerServidor;

void comunicacionConWorkerServidor(infoParaReduccionGlobal* param);
ParametrosComunicacionConWorkerServidor* setParametrosComunicacionConWorkerServidor(int puerto,char* ip);
void mensajesRecibidosDeWorkerServidor(int codigo, int FDServidorWORKER);
void mensajesEnviadosAWorkerServidor(int codigo, int FD_FileSystem, char* mensaje, int tamanio);

#endif /* HEADERS_COMUNICACIONCONWORKERSERVIDOR_H_ */
