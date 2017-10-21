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

	listaDeWorkerTotales = list_create(); //Inicializo

	if (string_equals_ignore_case(algoritmo, "W-CLOCK")) {

		logInfo("PLanificando con W-CLOCK");

		return planificarConW_Clock(listaDeWorkersAPlanificar,
				disponibilidadBase);
	} else {

		logInfo("PLanificando con CLOCK");

		return planificarConClock(listaDeWorkersAPlanificar, disponibilidadBase);

	}

}

t_list* dev_nodos_a_planificar(void) {
	//busco de la lista de nodos a planificar aquellos que tienen partes de archivo
	t_list* nodosFinalesAPLanificar = list_create();

	int b = 0;
	int v_tamanio;
	int tamanioDeLaListaDeWorkersTotalesSinRemove = list_size(
			listaDeWorkerTotales);

	while (b < tamanioDeLaListaDeWorkersTotalesSinRemove) {

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, b);

		logInfo("Verifico si pertenece a la planificacion %i", nodo->nodo);

		v_tamanio = list_size(nodo->partesDelArchivo);

		if (v_tamanio != 0) {
			nodoParaPlanificar* nodoConBloques = list_remove(
					listaDeWorkerTotales, b);

			list_add(nodosFinalesAPLanificar, nodoConBloques);
			b--;
		}
		b++;
		tamanioDeLaListaDeWorkersTotalesSinRemove--;
	}
	return nodosFinalesAPLanificar;
}

t_list* planificarConW_Clock(t_list* listaDeWorkersAPlanificar,
		int disponibilidadBase) {

	int parte = 0;
	t_list* partesDelArchivo = list_create();
	int cantidadDePartesDelArchivo = list_size(listaDeWorkersAPlanificar);

	while (cantidadDePartesDelArchivo > 0) {

		list_add(partesDelArchivo, parte);
		cantidadDePartesDelArchivo--;
		parte++;
	}

	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,
			disponibilidadBase);

	//busco de la lista de nodos a planificar aquellos que tienen partes de archivo
	t_list* nodosFinalesAPLanificar = dev_nodos_a_planificar();
	//cm falta liberar nodosFinalesAPLanificar

	nodoParaPlanificar* punteroClock;
	nodoParaPlanificar* punteroClockAuxiliar;

	list_destroy(partesDelArchivo);

}

t_list* planificarConClock(t_list* listaDeWorkersAPlanificar,
		int disponibilidadBase) {

	int parte = 0;
	t_list* partesDelArchivo = list_create(); // me dice las partes que tiene el archivo
	int cantidadDePartesDelArchivo = list_size(listaDeWorkersAPlanificar);
	t_list* nodosFinalesAPLanificar;

	while (cantidadDePartesDelArchivo > 0) {

		list_add(partesDelArchivo, parte);
		cantidadDePartesDelArchivo--;
		parte++;
	}

	logInfo("El archivo esta dividido en %i", list_size(partesDelArchivo));

	logInfo("Actualizar lista de workers Totales");

	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,
			disponibilidadBase);

	logInfo("La lista de workers totales se actualizo con %i",
			list_size(listaDeWorkerTotales));

	nodosFinalesAPLanificar = dev_nodos_a_planificar();

	logInfo("La lista de workers a planificar son %i",
			list_size(nodosFinalesAPLanificar));

	nodoParaPlanificar* punteroClock;
	nodoParaPlanificar* punteroClockAuxiliar;

	nodoConMayorDisponibilidadClock(nodosFinalesAPLanificar); //me devulve la lista ordenada segun la dispo y la carga
	//Con los punteros ,Nodos finales a planificar y partes del archivo

	list_destroy(partesDelArchivo);
	list_destroy(nodosFinalesAPLanificar);

}

//asignar partes a planificar a nodos
void asignarpartseANodos(t_list* partesDelArchivo, t_list* nodos) {

	int i, j = 0;

	while (i < list_size(partesDelArchivo)) {
		int parte = list_get(partesDelArchivo, i);

		bool dev_si_nodo_contiene_parteArch(int valor) {
			return valor == parte;

		}

		while (j < list_size(nodos)) {
			nodoParaPlanificar* nodo = list_get(nodos, j);

			if (list_any_satisfy(nodo->partesDelArchivo,
					dev_si_nodo_contiene_parteArch)) {
				//fijarme si tengo dsponibillidad
				if (nodo->disponibilidad > 0) {
					//resta la disponibilidad
					nodo->disponibilidad--;
					//asignar la parte al nodo
					//cm:comentoporque rompelist_add(nodo->partesAplanificar, parte);

				} else {

				}

				nodo->disponibilidad

			}

		}

	}
}

void actualizarListaDeWorkersTotales(t_list* listaDeWorkersAPLanificar,
		int disponibilidadBase) {

	int nodo1;
	int nodo2;
	int a = 0;
	while (a < list_size(listaDeWorkersAPLanificar)) {

		UbicacionBloquesArchivo* bloque = list_get(listaDeWorkersAPLanificar,
				a);
		nodo1 = bloque->ubicacionCopia1.nodo;
		nodo2 = bloque->ubicacionCopia2.nodo;
		if (!estaNodorEnLaListaDeTotales(nodo1)) {

			nodoParaPlanificar* nodoA = crearNodoParaPlanificar(
					bloque->ubicacionCopia1.nodo, disponibilidadBase, 0,
					bloque->parteDelArchivo);
			logInfo("Se va agregar el nodo %i", nodo1);
			logInfo("Tamanio Lista de workersTotales antes de add %i",
					list_size(listaDeWorkerTotales));

			list_add(listaDeWorkerTotales, nodoA);
			logInfo("Tamanio Lista de workersTotales %i",
					list_size(listaDeWorkerTotales));
		} else {
			//si ya esta el nodo en la lista, agrega una parte de archivo
			agregarPartedeArchivoANodo(nodo1, bloque->parteDelArchivo);
		}
		if (!estaNodorEnLaListaDeTotales(nodo2)) {

			nodoParaPlanificar* nodoB = crearNodoParaPlanificar(
					bloque->ubicacionCopia2.nodo, disponibilidadBase, 0,
					bloque->parteDelArchivo);
			logInfo("Se va agregar el nodo %i", nodo2);

			logInfo("Tamanio Lista de workersTotales antes de add %i",
					list_size(listaDeWorkerTotales));
			list_add(listaDeWorkerTotales, nodoB);
			logInfo("Tamanio Lista de workersTotales %i",
					list_size(listaDeWorkerTotales));
		} else {
			//si ya esta el nodo en la lista, agrega una parte de archivo
			agregarPartedeArchivoANodo(nodo1, bloque->parteDelArchivo);
		}

		a++;
	}
}
void agregarPartedeArchivoANodo(int nodoBUscado, int bloque) {
	int i;
	nodoParaPlanificar* nodoaModificar;

	for (i = 0; i < list_size(listaDeWorkerTotales); i++) {
		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, i);

		if (nodo->nodo == nodoBUscado) {
			nodoaModificar = list_remove(listaDeWorkerTotales, i);
			list_add(nodoaModificar->partesDelArchivo, bloque);
			list_add_in_index(listaDeWorkerTotales, i, nodoaModificar);
		}
	}
}
bool estaNodorEnLaListaDeTotales(int nodo) {

	int i = 0;

	if (list_size(listaDeWorkerTotales) == 0) {

		return false;
	} else {
		while (i < list_size(listaDeWorkerTotales)) {

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
nodoParaPlanificar* dev_mayor_disponib(nodoParaPlanificar* nodo1,
		nodoParaPlanificar* nodo2) {
	if (nodo1->disponibilidad >= nodo2->disponibilidad) {

		if (nodo1->disponibilidad == nodo2->disponibilidad) {
			if (nodo1->carga >= nodo2->carga) {
				return nodo1;
			} else {
				return nodo2;
			}
		}
		return nodo1;

	} else {
		return nodo2;
	}
}
void nodoConMayorDisponibilidadClock(t_list* nodos) {
// ordenar la lista segun la disponibilidad y si esta fuera la misma segun la carga
	list_sort(nodos, dev_mayor_disponib);
}

int nodoConMayorDisponibilidadW_Clock(t_list* nodos) {

	return 0;

}
