/*
 * nodos.h
 *
 *  Created on: 12/10/2017
 *      Author: utnso
 */

#ifndef HEADERS_NODOS_H_
#define HEADERS_NODOS_H_

#include "FileSystem.h"

//static bloques_nodo* inicializarEstructuras(int id);

int elegirNodo(t_list* nodos);
int bloquesLibres(bloques_nodo* nodo);
int actualizarBitmapDelNodo(bloques_nodo* nodo);
t_list* distribuirBloques(t_list* bloques, t_list* nodos, int indiceArchivo);


int crearRegistroArchivoNodos(tabla_nodos tablaNodos);
int actualizarBitMap(int idNodo, int desplazamiento);
int ultimaCopia(int indiceArchivo,int parteArchivo);

#endif /* HEADERS_NODOS_H_ */
