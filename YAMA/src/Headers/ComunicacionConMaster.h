/*
 * ComunicacionConMaster.h
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */

#ifndef HEADERS_COMUNICACIONCONMASTER_H_
#define HEADERS_COMUNICACIONCONMASTER_H_
#include "socketsYAMA.h"

typedef struct ParametrosComunicacionConMaster{
	int puerto;
} ParametrosComunicacionConMaster;

ParametrosComunicacionConMaster* setParametrosComunicacionConMaster(int puerto);
void comunicacionConMaster(ParametrosComunicacionConMaster* parametro);
#endif /* HEADERS_COMUNICACIONCONMASTER_H_ */
