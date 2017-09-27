/*
 * comunicacionConDN.h
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONDN_H_
#define HEADERS_COMUNICACIONCONDN_H_

#include <stdio.h>
#include <stdlib.h>
#include "socketsFileSystem.h"
#include "configuracion.h"
#include "logFileSystem.h"
#include "SO_lib/sockets.h"

typedef struct ParametrosComunicacion{
	int puertoFS_dn;
	int puertoFS_yama;
	int puertoFS_worker;
}ParametrosComunicacion;

void comunicacionDN(ParametrosComunicacion* parametros);
ParametrosComunicacion* setParametrosComunicacion(int puertoDN, int puertoYAMA, int puertoWorker);



#endif /* HEADERS_COMUNICACIONCONDN_H_ */
