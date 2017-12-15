#ifndef HEADERS_COMUNICACIONCONWORKER_H_
#define HEADERS_COMUNICACIONCONWOKER_H_

#include <stdio.h>
#include <stdlib.h>
#include "configuracion.h"
#include "logMaster.h"
#include "SO_lib/sockets.h"
#include "SO_lib/estructuras.h"
#include "SO_lib/Protocolo.h"
#include "SO_lib/FuncionesAuxiliares.h"
#include "SO_lib/serializacion.h"
#include "Master.h"

typedef struct ParametrosComunicacionWorker{

	int puertoWoker;
	char* ipWoker;
	int bloque;
	int bytesOcupados;
	char* archivoTemporal;
	int nodo;
}ParametrosComunicacionWoker;

typedef struct ParametroParaWorker{
	int nodo;
	int puerto;
	char* ip;
	t_list* transformaciones; //lista de estructura tareaTransformacion;
}ParametroParaWorker;

typedef struct tareaTransformacion{
	int bloque;
	int bytesOcupados;
	char* archivoTemporal;
}tareaTransformacion;



void comunicacionWorkers(ParametrosComunicacionWoker* parametros) ;

ParametrosComunicacionWoker* setParametrosComunicacionConWoker(int puerto, char* ip, int nodo, char* archivo, int bytesOcupados, int bloque);

void mensajesEnviadosAWorker(int codigo, int FDServidorWORKER, char* mensaje,int tamanio);
void mensajesRecibidosDeWorker(int codigo, int FDServidorWORKER);

#endif
