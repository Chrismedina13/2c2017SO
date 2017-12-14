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

int elegirNodo(int indexNodoAnterior);
void* sortListaNodos();
int bloquesLibres(bloques_nodo* nodo);
bool bloquesLibresSort(bloques_nodo* nodo, bloques_nodo* nodo2);
int ocuparBloqueBitMap(bloques_nodo* nodo);
void* distribuirBloques(int indiceArchivo);


int crearRegistroArchivoNodos(tabla_nodos tablaNodos);
int liberarBloqueBitMap(int idNodo, int desplazamiento);
int ultimaCopia(int indiceArchivo,int parteArchivo);

#endif /* HEADERS_NODOS_H_ */
