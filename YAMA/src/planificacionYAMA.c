/*
 * planificacionYAMA.c
 *
 *  Created on: 10/10/2017
 *      Author: utnso
 */

#include "Headers/planificacionYAMA.h"

void agregarWorkerALista(nodoParaPlanificar* worker) {

	pthread_mutex_lock(&mutexWorkerAPlanificar);
	list_add(listaDeWorkerTotales, worker);
	pthread_mutex_lock(&mutexWorkerAPlanificar);

}

void agregarJObACola(char* job) {

	pthread_mutex_lock(&mutexJobsAPlanificar);
	queue_push(jobsAPlanificar, job);
	pthread_mutex_unlock(&mutexJobsAPlanificar);

}

char* retirarJobDeLista() {

	pthread_mutex_lock(&mutexJobsAPlanificar);
	char* job = queue_pop(jobsAPlanificar);
	pthread_mutex_unlock(&mutexJobsAPlanificar);
	return job;
}

t_list* planificar(t_list* listaDeWorkersAPlanificar, char* algoritmo,
		int disponibilidadBase) {

	if (string_equals_ignore_case(algoritmo, "W-CLOCK")) {

		return planificarConW_Clock(listaDeWorkersAPlanificar,
				disponibilidadBase);
	} else {

		return planificarConClock(listaDeWorkersAPlanificar, disponibilidadBase);

	}

}

t_list* planificarConW_Clock(t_list* listaDeWorkersAPlanificar,
		int disponibilidadBase) {

	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,
			disponibilidadBase);

}

t_list* planificarConClock(t_list* listaDeWorkersAPlanificar,
		int disponibilidadBase) {

	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,
			disponibilidadBase);
}

void actualizarListaDeWorkersTotales(t_list* listaDeWorkersAPLanificar,
		int disponibilidadBase) {

	int nodo1;
	int nodo2;
	int a = 1;
	while (a <= list_size(listaDeWorkersAPLanificar)) {

		UbicacionBloquesArchivo* bloque = list_get(listaDeWorkersAPLanificar,
				a);
		nodo1 = bloque->ubicacionCopia1->nodo;
		nodo2 = bloque->ubicacionCopia2->nodo;
		if (!estaNodorEnLaListaDeTotales(nodo1)) {

			nodoParaPlanificar* nodoA = crearNodoParaPlanificar(
					bloque->ubicacionCopia1->nodo, disponibilidadBase, 0,
					bloque->parteDelArchivo);
			agregarWorkerALista(nodoA);
		} else {
			//si ya esta el nodo en la lista, agrega una parte de archivo
			agregarPartedeArchivoANodo(nodo1, bloque->parteDelArchivo);
		}
		if (!estaNodorEnLaListaDeTotales(nodo2)) {

			nodoParaPlanificar* nodoB = crearNodoParaPlanificar(
					bloque->ubicacionCopia2->nodo, disponibilidadBase, 0,
					bloque->parteDelArchivo);
			agregarWorkerALista(nodoB);
		}else {
			//si ya esta el nodo en la lista, agrega una parte de archivo
			agregarPartedeArchivoANodo(nodo1, bloque->parteDelArchivo);
		}

		a++;
	}
}
void agregarPartedeArchivoANodo(int nodoBUscado, int bloque) {
	int i;

	for (i = 1; i < list_size(listaDeWorkerTotales); i++) {
		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, i);

		if (nodo->nodo == nodoBUscado) {
			nodoParaPlanificar* nodoaModificar = list_remove(
					listaDeWorkerTotales, i);
			list_add(nodoaModificar->partesDelArchivo, bloque);
			list_add_in_index(listaDeWorkerTotales, nodoaModificar,i);
		}
	}
}
bool estaNodorEnLaListaDeTotales(int nodo) {

	int i = 1;

	if (list_size(listaDeWorkerTotales) == 0) {

		return false;
	} else {
		while (i <= list_size(listaDeWorkerTotales)) {

			nodoParaPlanificar* nodoAPlanificar = list_get(listaDeWorkerTotales,
					i);
			if (nodoAPlanificar->nodo == nodo) {
				return true;
			} else {

				i++;
			}

		}

		return false;
	}

}
