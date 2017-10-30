/*
 * Test.h
 *
 *  Created on: 16/10/2017
 *      Author: utnso
 */
/*
#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "SO_lib/estructuras.h"
#include "stdbool.h"
#include "commons/collections/list.h"

t_list* listaDeWorkerTotales;

void agregarWorkerALista(nodoParaPlanificar* worker);
t_list* planificar(t_list* listaDeWorkersAPlanificar,char* algoritmo, int disponibilidadBase);
t_list* planificarConW_Clock(t_list* listaDeWorkersAPlanificar,int disponibilidadBase);
t_list* planificarConClock(t_list* listaDeWorkersAPlanificar,int disponibilidadBase);
void agregarPartedeArchivoANodo(int nodoBUscado, int bloque);
void actualizarListaDeWorkersTotales(t_list* listaDeWorkersAPLanificar,int disponibilidadBase);
bool estaNodorEnLaListaDeTotales(int nodo);
UbicacionBloquesArchivo* crearUbicacionBloquesArchivos(int parteDelArchivo,int bytesOcupados,int copia1Nodo, int copia1Bloque
		,int copia2Nodo,int copia2Bloque);

#endif /* TEST_H_ */
