/*
 * nodos.h
 *
 *  Created on: 12/10/2017
 *      Author: utnso
 */

#ifndef HEADERS_NODOS_H_
#define HEADERS_NODOS_H_

#include "commons/bitarray.h"

int bloquesLibres(t_bitarray bloques);

void funcion(t_bitarray bloques);

void inicializarEstructuras();

int crearRegistroArchivoNodos(int tamanio, int libres, int nodos, char* nodosPtr);
#endif /* HEADERS_NODOS_H_ */
