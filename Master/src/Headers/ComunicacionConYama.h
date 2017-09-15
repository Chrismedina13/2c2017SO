#ifndef HEADERS_COMUNICACIONCONYAMA_H_
#define HEADERS_COMUNICACIONCONYAMA_H_

#include <stdio.h>
#include <stdlib.h>
#include "socketsMaster.h"
#include "configuracion.h"
#include "logMaster.h"

typedef struct ParametrosComunicacionYAMA{

	int puertoYAMA;
	char* ipYAMA;

}ParametrosComunicacionYAMA;

void comunicacionYama(ParametrosComunicacionYAMA* parametros);
ParametrosComunicacionYAMA* setParametrosComunicacionYAMA(int ip,char* puerto);


#endif /* HEADERS_MASTER_H_ */
