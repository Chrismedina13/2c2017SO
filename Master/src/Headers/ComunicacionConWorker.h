#ifndef HEADERS_COMUNICACIONCONWORKER_H_
#define HEADERS_COMUNICACIONCONWOKER_H_

#include <stdio.h>
#include <stdlib.h>
#include "configuracion.h"
#include "logMaster.h"
#include "SO_lib/sockets.h"
#include "SO_lib/estructuras.h"
#include "SO_lib/Protocolo.h"

typedef struct ParametrosComunicacionWorker{

	int puertoWoker;
	char* ipWoker;

}ParametrosComunicacionWoker;

void comunicacionWorkers(ParametrosComunicacionWoker* parametros) ;

ParametrosComunicacionWoker* setParametrosComunicacionConWoker(int puerto,char* ip);



#endif
