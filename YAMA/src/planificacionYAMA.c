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
		int disponibilidadBase,char* jobAejecutar) {

	//Inicializo

	if (string_equals_ignore_case(algoritmo, "W-CLOCK")) {

		logInfo("PLanificando con W-CLOCK");

		t_list* listaConLaPlanificacion = planificarConW_Clock(listaDeWorkersAPlanificar,
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

	asignarFuncionDeDisponibilidadW_ClockANodos(disponibilidadBase);

	//busco de la lista de nodos a planificar aquellos que tienen partes de archivo
	t_list* nodosFinalesAPLanificar = dev_nodos_a_planificar();

	//cm falta liberar nodosFinalesAPLanificar

	nodoParaPlanificar* punteroClock;
	nodoParaPlanificar* punteroClockAuxiliar;

	nodoConMayorDisponibilidadClock(nodosFinalesAPLanificar); //me devulve la lista ordenada segun la dispo y la carga


	int t,desplazamientoPuntero = 0;
	int indexParaActualizar;// me devuelve donde tengo que actualizar segun el punteroauxiliar


	logInfo("Asignando partes a planificar a los nodos");

	while (t < list_size(partesDelArchivo)) {

		int parte = list_get(partesDelArchivo, t);
		punteroClock = list_get(nodosFinalesAPLanificar, desplazamientoPuntero);
		if (punteroClock->disponibilidad != 0) {
			if (estaParteEnNodo(parte, punteroClock)) {
				nodoParaPlanificar* nodoParaActuaizar = list_remove(
						nodosFinalesAPLanificar, desplazamientoPuntero);
				nodoParaActuaizar->disponibilidad--;
				list_add(nodoParaActuaizar->partesAplanificar, parte);
				list_add_in_index(nodosFinalesAPLanificar,
						desplazamientoPuntero, nodoParaActuaizar);
				t++; // se analiza la otra parte
				desplazamientoPuntero++; // avanza el puntero a la otra posicion de los nodos
			} else {

				indexParaActualizar = moverPunteroAuxiliar(punteroClockAuxiliar,
						nodosFinalesAPLanificar, desplazamientoPuntero,parte); // me devuelve el index del lugar donde tiene la parte
				if (indexParaActualizar == -1) { //entra aca si da una vuelta y todas las disponibilidades estan en 0
					sumarDisponibilidadBaseAtodoslosNodos(nodosFinalesAPLanificar,disponibilidadBase);
					indexParaActualizar = moverPunteroAuxiliar(
							punteroClockAuxiliar, nodosFinalesAPLanificar,
							desplazamientoPuntero,parte);
					nodoParaPlanificar* nodoAActualizar = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodoAActualizar->disponibilidad --;
					list_add(nodoAActualizar->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodoAActualizar);
					t ++;
					desplazamientoPuntero ++;

				}else{
					nodoParaPlanificar* nodo = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodo->disponibilidad --;
					list_add(nodo->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodo);
					t ++;
					desplazamientoPuntero ++;

				}
			}

		} else {
			punteroClock->disponibilidad = disponibilidadBase;

			if (estaParteEnNodo(parte, punteroClock)) {
				nodoParaPlanificar* nodoParaActuaizar = list_remove(
						nodosFinalesAPLanificar, desplazamientoPuntero);
				nodoParaActuaizar->disponibilidad--;
				list_add(nodoParaActuaizar->partesAplanificar, parte);
				list_add_in_index(nodosFinalesAPLanificar,
						desplazamientoPuntero, nodoParaActuaizar);
				t++;
				desplazamientoPuntero++;
			} else {

				indexParaActualizar = moverPunteroAuxiliar(punteroClockAuxiliar,
						nodosFinalesAPLanificar, desplazamientoPuntero,parte); // me devuelve el index del lugar donde tiene la parte
				if (indexParaActualizar == -1) { //entra aca si da una vuelta y todas las disponibilidades estan en 0
					sumarDisponibilidadBaseAtodoslosNodos(nodosFinalesAPLanificar,disponibilidadBase);
					indexParaActualizar = moverPunteroAuxiliar(
							punteroClockAuxiliar, nodosFinalesAPLanificar,
							desplazamientoPuntero,parte);
					nodoParaPlanificar* nodoAActualizar = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodoAActualizar->disponibilidad --;
					list_add(nodoAActualizar->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodoAActualizar);
					t ++;
					desplazamientoPuntero ++;

				}else{
					nodoParaPlanificar* nodo = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodo->disponibilidad --;
					list_add(nodo->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodo);
					t ++;
					desplazamientoPuntero ++;

				}
			}

		}
	}

	logInfo("Actualizar carga de trabajo de los nodos planificados");
	actualizarCargaDeTrabajoDeWorkersPLanificados(nodosFinalesAPLanificar);

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

	asignarFuncionDeDisponibilidadClockANodos(disponibilidadBase);

	nodosFinalesAPLanificar = dev_nodos_a_planificar();

	logInfo("La lista de workers a planificar son %i",
			list_size(nodosFinalesAPLanificar));

	nodoParaPlanificar* punteroClock;
	nodoParaPlanificar* punteroClockAuxiliar;

	nodoConMayorDisponibilidadClock(nodosFinalesAPLanificar); //me devulve la lista ordenada segun la dispo y la carga
	//Con los punteros ,Nodos finales a planificar y partes del archivo

	int t,desplazamientoPuntero = 0;
	int indexParaActualizar;// me devuelve donde tengo que actualizar segun el punteroauxiliar

	logInfo("Asignando partes a planificar a los nodos");

	while (t < list_size(partesDelArchivo)) {

		int parte = list_get(partesDelArchivo, t);
		punteroClock = list_get(nodosFinalesAPLanificar, desplazamientoPuntero);
		if (punteroClock->disponibilidad != 0) {
			if (estaParteEnNodo(parte, punteroClock)) {
				nodoParaPlanificar* nodoParaActuaizar = list_remove(
						nodosFinalesAPLanificar, desplazamientoPuntero);
				nodoParaActuaizar->disponibilidad--;
				list_add(nodoParaActuaizar->partesAplanificar, parte);
				list_add_in_index(nodosFinalesAPLanificar,
						desplazamientoPuntero, nodoParaActuaizar);
				t++; // se analiza la otra parte
				desplazamientoPuntero++; // avanza el puntero a la otra posicion de los nodos
			} else {

				indexParaActualizar = moverPunteroAuxiliar(punteroClockAuxiliar,
						nodosFinalesAPLanificar, desplazamientoPuntero,parte); // me devuelve el index del lugar donde tiene la parte
				if (indexParaActualizar == -1) { //entra aca si da una vuelta y todas las disponibilidades estan en 0
					sumarDisponibilidadBaseAtodoslosNodos(nodosFinalesAPLanificar,disponibilidadBase);
					indexParaActualizar = moverPunteroAuxiliar(
							punteroClockAuxiliar, nodosFinalesAPLanificar,
							desplazamientoPuntero,parte);
					nodoParaPlanificar* nodoAActualizar = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodoAActualizar->disponibilidad --;
					list_add(nodoAActualizar->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodoAActualizar);
					t ++;
					desplazamientoPuntero ++;

				}else{
					nodoParaPlanificar* nodo = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodo->disponibilidad --;
					list_add(nodo->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodo);
					t ++;
					desplazamientoPuntero ++;

				}
			}

		} else {
			punteroClock->disponibilidad = disponibilidadBase;

			if (estaParteEnNodo(parte, punteroClock)) {
				nodoParaPlanificar* nodoParaActuaizar = list_remove(
						nodosFinalesAPLanificar, desplazamientoPuntero);
				nodoParaActuaizar->disponibilidad--;
				list_add(nodoParaActuaizar->partesAplanificar, parte);
				list_add_in_index(nodosFinalesAPLanificar,
						desplazamientoPuntero, nodoParaActuaizar);
				t++;
				desplazamientoPuntero++;
			} else {

				indexParaActualizar = moverPunteroAuxiliar(punteroClockAuxiliar,
						nodosFinalesAPLanificar, desplazamientoPuntero,parte); // me devuelve el index del lugar donde tiene la parte
				if (indexParaActualizar == -1) { //entra aca si da una vuelta y todas las disponibilidades estan en 0
					sumarDisponibilidadBaseAtodoslosNodos(nodosFinalesAPLanificar,disponibilidadBase);
					indexParaActualizar = moverPunteroAuxiliar(
							punteroClockAuxiliar, nodosFinalesAPLanificar,
							desplazamientoPuntero,parte);
					nodoParaPlanificar* nodoAActualizar = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodoAActualizar->disponibilidad --;
					list_add(nodoAActualizar->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodoAActualizar);
					t ++;
					desplazamientoPuntero ++;

				}else{
					nodoParaPlanificar* nodo = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodo->disponibilidad --;
					list_add(nodo->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodo);
					t ++;
					desplazamientoPuntero ++;

				}
			}

		}
	}

	logInfo("Actualizar carga de trabajo de los nodos planificados");
	actualizarCargaDeTrabajoDeWorkersPLanificados(nodosFinalesAPLanificar);

	logInfo("Armar estructura para mandar a Master");


	logInfo("Actualizar tabla global de Yama");


	logInfo("devolver nodos planificados a la estructura de listaDeWorkersTotales");

	list_destroy(partesDelArchivo);
	list_destroy(nodosFinalesAPLanificar);

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
			//si ya esta el nodo en la lista, agre ga una parte de archivo
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
			if (nodo1->carga <= nodo2->carga) {
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

bool estaParteEnNodo(int parte, nodoParaPlanificar* nodo){

	int posicionPartesDelArchivo = 0;
	while(posicionPartesDelArchivo < list_size(nodo->partesDelArchivo)){

		if (parte == list_get(nodo->partesDelArchivo,posicionPartesDelArchivo)){

			return true;
		}else{

			posicionPartesDelArchivo ++;
		}


	}

	return false;
}

int moverPunteroAuxiliar(nodoParaPlanificar* punteroClockAuxiliar,
		t_list* nodosFinalesAPLanificar, int desplazamientoPuntero, int parte) {

	int desplazamiento = desplazamientoPuntero + 1;

	while (desplazamiento < list_size(nodosFinalesAPLanificar)
			&& desplazamiento != desplazamientoPuntero) {

		punteroClockAuxiliar = list_get(nodosFinalesAPLanificar,
				desplazamiento);
		if (estaParteEnNodo(parte, punteroClockAuxiliar)) {

			return desplazamiento;
		} else {

			if (desplazamiento == (list_size(nodosFinalesAPLanificar) - 1)) {

				desplazamiento = 0;

			} else {

				desplazamiento++;
			}
		}

	}
	return -1;

}

void sumarDisponibilidadBaseAtodoslosNodos(t_list* nodosFinalesAPLanificar,int disponibilidadBase){

	int i = 0;
	while(i < list_size(nodosFinalesAPLanificar)){

		nodoParaPlanificar* nodo =list_remove(nodosFinalesAPLanificar,i);
		nodo->disponibilidad = disponibilidadBase;
		list_add_in_index(nodosFinalesAPLanificar,i,nodo);

		i++;
	}

}

void asignarFuncionDeDisponibilidadW_ClockANodos(int disponibilidadBase){

	int a,b = 0;
	int mayorCargaDeTrabajo;

	while(a < list_size(listaDeWorkerTotales)){

		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales,a);
		if (nodo->carga >= mayorCargaDeTrabajo){

			mayorCargaDeTrabajo = nodo->carga;
		}

		a ++;
	}

	logInfo("La mayor carga de trabajo es %i", mayorCargaDeTrabajo);

	logInfo("Asignado funcion de disponibilidad w_clock a nodos");

	while(b < list_size(listaDeWorkerTotales)){

		nodoParaPlanificar* nodoAActualizar = list_remove(listaDeWorkerTotales,b);
		nodoAActualizar->disponibilidad = (disponibilidadBase + mayorCargaDeTrabajo) - nodoAActualizar->carga;
		list_add_in_index(listaDeWorkerTotales,b,nodoAActualizar);

		b++;
	}

}


void asignarFuncionDeDisponibilidadClockANodos(int disponibilidadBase){

	int b = 0;

	logInfo("Asignado funcion de disponibilidad de algoritmo clock a nodos");

	while(b < list_size(listaDeWorkerTotales)){

		nodoParaPlanificar* nodoAActualizar = list_remove(listaDeWorkerTotales,b);
		nodoAActualizar->disponibilidad = disponibilidadBase;
		list_add_in_index(listaDeWorkerTotales,b,nodoAActualizar);

		b++;
	}

}

void actualizarCargaDeTrabajoDeWorkersPLanificados(t_list* nodosFinalesAPLanificar){

	int a = 0;
	while(a < list_size(nodosFinalesAPLanificar)){

		nodoParaPlanificar* nodo = list_remove(nodosFinalesAPLanificar,a);
		nodo->carga += list_size(nodo->partesAplanificar);
		list_add_in_index(nodosFinalesAPLanificar,a,nodo);

		a++;
	}
}




