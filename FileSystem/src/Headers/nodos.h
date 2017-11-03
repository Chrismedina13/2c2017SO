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

typedef struct Nodo{
	int bitmap[20];
	int id_nodo;
}Nodo;

static Nodo* inicializarEstructuras(int id);

int elegirNodo(t_list* nodos);
int bloquesLibres(Nodo* nodo);
int actualizarBitmapDelNodo(Nodo* nodo);
t_list* distribuirBloques(t_list* bloques, t_list* nodos);


int crearRegistroArchivoNodos(int tamanio, int libres, int nodos, char* nodosPtr);

#endif /* HEADERS_NODOS_H_ */
