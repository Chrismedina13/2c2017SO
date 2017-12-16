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
	int nodo;
	t_list *tareas;
}ParametrosComunicacionWoker;

void comunicacionReduccionLocalWorker(RespuestaReduccionLocal* rrl);
void comunicacionTransformacionWorker(ParametrosComunicacionWoker* parametros) ;

ParametrosComunicacionWoker* setParametrosComunicacionConWoker(int puerto,
		char* ip, int nodo, t_list* listaTareas);

void mensajesEnviadosAWorker(int codigo, int FDServidorWORKER, char* mensaje,int tamanio);
void mensajesRecibidosDeWorker(int codigo, int FDServidorWORKER,int cantidadDeBytes,int nodo,int bloque);

#endif
