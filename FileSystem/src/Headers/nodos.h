/*
 * nodos.h
 *
 *  Created on: 12/10/2017
 *      Author: utnso
 */

#ifndef HEADERS_NODOS_H_
#define HEADERS_NODOS_H_

#include "commons/bitarray.h"
#include "commons/collections/list.h"
#include "FileSystem.h"

static bitMap* inicializarEstructuras(int id);

int elegirNodo(t_list* nodos);
int bloquesLibres(bitMap* nodo);
int actualizarBitmapDelNodo(bitMap* nodo);
t_list* distribuirBloques(t_list* bloques, t_list* nodos);


int crearRegistroArchivoNodos(tabla_nodos tablaNodos);

#endif /* HEADERS_NODOS_H_ */
