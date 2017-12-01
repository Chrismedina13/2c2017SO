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
#include "SO_lib/Protocolo.h"
#include "ComunicacionConMaster.h"


typedef struct ParametrosComunicacionConFileSystem {
	int puerto;
	int ip;
	char* algoritmo;
	int disponibilidadBase;
	int retardo;
} ParametrosComunicacionConFileSystem;

ParametrosComunicacionConFileSystem* parametrosFileSystem;

void comunicacionConFileSystem();
ParametrosComunicacionConFileSystem* setParametrosComunicacionConFileSystem(
		int puerto, char* ip,char* algoritmo,int disponiblidadBase, int retardo);
void mensajesRecibidosDeFS(int codigo, int FDsocketClienteFileSystem);

#endif /* HEADERS_COMUNICACIONCONFILESYSTEM_H_ */
