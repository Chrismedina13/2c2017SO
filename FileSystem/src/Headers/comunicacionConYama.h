/*
 * comunicacionConYama.h
 *
 *  Created on: 16/9/2017
 *      Author: utnso
 */
#include "comunicacionConDN.h"
#ifndef HEADERS_COMUNICACIONCONYAMA_H_
#define HEADERS_COMUNICACIONCONYAMA_H_
#include "SO_lib/Protocolo.h"
void comunicacionYAMA(ParametrosComunicacion* parametros);
ParametrosComunicacion* setParametrosComunicacion(int puertoDN, int puertoYAMA,
		int puertoWorker);
void mensajesRecibidosDeYama(int codigo, int FDYama);


#endif /* HEADERS_COMUNICACIONCONYAMA_H_ */
