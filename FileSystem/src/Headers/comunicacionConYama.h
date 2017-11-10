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
#include "SO_lib/serializacion.h"

void comunicacionYAMA(ParametrosComunicacion* parametros);
ParametrosComunicacion* setParametrosComunicacion(int puertoDN, int puertoYAMA,
		int puertoWorker);
void mensajesRecibidosDeYama(int codigo, int FDYama);

void mensajesEnviadosAYama(int codigo,int FD_YAMA, char* mensaje,int tamanio);

char* setearUbicacionBloque(int nodo1, int bloquenodo1, int nodo2, int bloquenodo2, int parteDelArchivo,int bytesOcupados);
#endif /* HEADERS_COMUNICACIONCONYAMA_H_ */
