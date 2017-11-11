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

static bloques_nodo* inicializarEstructuras(int id);

int elegirNodo(t_list* nodos);
int bloquesLibres(bloques_nodo* nodo);
int actualizarBitmapDelNodo(bloques_nodo* nodo);
t_list* distribuirBloques(t_list* bloques, t_list* nodos);


int crearRegistroArchivoNodos(tabla_nodos tablaNodos);
int actualizarBitMap(char* path);

#endif /* HEADERS_NODOS_H_ */
