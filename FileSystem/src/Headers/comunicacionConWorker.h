/*
 * comunicacionConWorker.h
 *
 *  Created on: 19/9/2017
 *      Author: utnso
 */
#include "comunicacionConDN.h"

#ifndef HEADERS_COMUNICACIONCONWORKER_H_
#define HEADERS_COMUNICACIONCONWORKER_H_

void comunicacionWorker(ParametrosComunicacion* parametros);

void mensajesRecibidosDeWorker(int codigo, int FD_DN);

#endif /* HEADERS_COMUNICACIONCONWORKER_H_ */
