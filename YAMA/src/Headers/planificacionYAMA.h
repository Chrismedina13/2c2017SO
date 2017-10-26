/*
 * planificacionYAMA.h
 *
 *  Created on: 10/10/2017
 *      Author: utnso
 */

#ifndef HEADERS_PLANIFICACIONYAMA_H_
#define HEADERS_PLANIFICACIONYAMA_H_

#include "yama.h"
#include <semaphore.h>
#include "commons/collections/queue.h"
#include "SO_lib/estructuras.h"

pthread_mutex_t mutexJobsAPlanificar;
pthread_mutex_t mutexWorkerAPlanificar;

void agregarJObACola(char* job);
char* retirarJobDeLista();
void agregarWorkerALista(nodoParaPlanificar* worker);
t_list* planificar(t_list* listaDeWorkersAPlanificar,char* algoritmo, int disponibilidadBase);
t_list* planificarConW_Clock(t_list* listaDeWorkersAPlanificar,int disponibilidadBase);
t_list* planificarConClock(t_list* listaDeWorkersAPlanificar,int disponibilidadBase);
void agregarPartedeArchivoANodo(int nodoBUscado, int bloque);
void actualizarListaDeWorkersTotales(t_list* listaDeWorkersAPLanificar,int disponibilidadBase);
bool estaNodorEnLaListaDeTotales(int nodo);
int nodoConMayorDisponibilidadW_Clock(t_list* nodos);
void nodoConMayorDisponibilidadClock(t_list* nodos);
t_list* dev_nodos_a_planificar(void);
int moverPunteroAuxiliar(nodoParaPlanificar* punteroClockAuxiliar,t_list* nodosFinalesAPLanificar, int desplazamientoPuntero, int parte);
void sumarDisponibilidadBaseAtodoslosNodos(t_list* nodosFinalesAPLanificar,int disponibilidadBase);
bool estaParteEnNodo(int parte, nodoParaPlanificar* nodo);
void asignarFuncionDeDisponibilidadW_ClockANodos(int disponibilidadBase);





#endif /* HEADERS_PLANIFICACIONYAMA_H_ */
