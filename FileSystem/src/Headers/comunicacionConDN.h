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

typedef struct ParametrosComunicacion{
	int puertoFS_dn;
	int puertoFS_yama;
}ParametrosComunicacion;

void comunicacionDN(ParametrosComunicacion* parametros);
ParametrosComunicacion* setParametrosComunicacion(int puertoDN, int puertoYAMA);



#endif /* HEADERS_COMUNICACIONCONDN_H_ */
