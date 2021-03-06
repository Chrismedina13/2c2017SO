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
	pthread_mutex_unlock(&mutexWorkerAPlanificar);

}

void agregarJObACola(Job* job) {

	pthread_mutex_lock(&mutexJobsAPlanificar);
	queue_push(jobsAPlanificar, job);
	pthread_mutex_unlock(&mutexJobsAPlanificar);

}

Job* retirarJobDeLista() {

	pthread_mutex_lock(&mutexJobsAPlanificar);
	Job* job = queue_pop(jobsAPlanificar);
	pthread_mutex_unlock(&mutexJobsAPlanificar);
	return job;
}

t_list* planificar(t_list* listaDeWorkersAPlanificar, char* algoritmo,
		int disponibilidadBase,char* jobAejecutar) {


	if (string_equals_ignore_case(algoritmo, "W-CLOCK")) {

		logInfo("PLanificando con W-CLOCK");

		usleep(parametrosFileSystem->retardo);
		logInfo("La Planificacion Tiene un retardo de %i", parametrosFileSystem->retardo);

		return planificarConW_Clock(listaDeWorkersAPlanificar,disponibilidadBase);

	} else {

		logInfo("PLanificando con CLOCK");
		usleep(parametrosFileSystem->retardo);
		logInfo("La Planificacion Tiene un retardo de %i", parametrosFileSystem->retardo);
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

t_list* planificarConW_Clock(t_list* listaDeWorkersAPlanificar,int disponibilidadBase){

	pthread_mutex_lock(&mutexPlanificacion);

	int parte = 0;
	t_list* partesDelArchivo = list_create();
	int cantidadDePartesDelArchivo = list_size(listaDeWorkersAPlanificar);

	while (cantidadDePartesDelArchivo > 0) {

		list_add(partesDelArchivo, parte);
		cantidadDePartesDelArchivo--;
		parte++;
	}

	logInfo("El archivo tiene %i", list_size(partesDelArchivo));

	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,disponibilidadBase);

	asignarFuncionDeDisponibilidadW_ClockANodos(disponibilidadBase);

	logInfo("Buscar y obtener solo aquellos nodos que tienen partes del archivo");
	t_list* nodosFinalesAPLanificar = dev_nodos_a_planificar();

	algoritmoPrincipal(nodosFinalesAPLanificar,partesDelArchivo,disponibilidadBase);

	logInfo("Actualizar carga de trabajo de los nodos planificados");
	actualizarCargaDeTrabajoDeWorkersPLanificados(nodosFinalesAPLanificar);

	logInfo("Armar estructura para mandar a Master");
	t_list* respuestaAMaster = armarRespuestaTransformacionYAMA(nodosFinalesAPLanificar,listaDeWorkersAPlanificar);

	logInfo("devolver nodos planificados a la estructura de listaDeWorkersTotales y limpiar las listas internas");

	int j = 0;
	while(j<list_size(nodosFinalesAPLanificar)){
		nodoParaPlanificar* nodofin = list_remove(nodosFinalesAPLanificar,j);
		list_clean(nodofin->partesAplanificar);
		list_clean(nodofin->partesDelArchivo);
		list_add_in_index(nodosFinalesAPLanificar,j,nodofin);
		j++;
	}


	list_add_all(listaDeWorkerTotales,nodosFinalesAPLanificar);

	pthread_mutex_unlock(&mutexPlanificacion);

	return respuestaAMaster;
}

t_list* planificarConClock(t_list* listaDeWorkersAPlanificar,int disponibilidadBase){

	pthread_mutex_lock(&mutexJobsAPlanificar);

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

	logInfo("Actualizar lista de workers Totales");//Arreglado
	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,disponibilidadBase);

	logInfo("La lista de workers totales se actualizo con %i",list_size(listaDeWorkerTotales));
	asignarFuncionDeDisponibilidadClockANodos(disponibilidadBase);

	nodosFinalesAPLanificar = dev_nodos_a_planificar();

	int l = 0;
	int m = 0;
	while(l < list_size(nodosFinalesAPLanificar)){

		nodoParaPlanificar* nodin = list_get(nodosFinalesAPLanificar,l);
		while(m < list_size(nodin->partesDelArchivo)){

			int parte = list_get(nodin->partesDelArchivo,m);

			logInfo("El nodo %i tiene a la parte %i", nodin->nodo,parte);

			m++;
		}
		m=0;

		l++;
	}

	logInfo("Aplico algoritmo principal");
	algoritmoPrincipal(nodosFinalesAPLanificar,partesDelArchivo,disponibilidadBase);

	int o = 0;
		int p = 0;
		while(o < list_size(nodosFinalesAPLanificar)){

			nodoParaPlanificar* nodin = list_get(nodosFinalesAPLanificar,o);
			while(p < list_size(nodin->partesAplanificar)){

				int parte = list_get(nodin->partesAplanificar,p);

				logInfo("El nodo %i tiene que planificar la parte %i", nodin->nodo,parte);

				p++;
			}
			p=0;

			o++;
		}

	logInfo("Actualizar carga de trabajo de los nodos planificados");
	actualizarCargaDeTrabajoDeWorkersPLanificados(nodosFinalesAPLanificar);

	logInfo("Armar estructura para mandar a Master");
	t_list* respuestaAMaster = armarRespuestaTransformacionYAMA(nodosFinalesAPLanificar,listaDeWorkersAPlanificar);

	logInfo("devolver nodos planificados a la estructura de listaDeWorkersTotales y limpiar las listas internas");

	int j = 0;
	while(j<list_size(nodosFinalesAPLanificar)){
		nodoParaPlanificar* nodofin = list_remove(nodosFinalesAPLanificar,j);
		list_clean(nodofin->partesAplanificar);
		list_clean(nodofin->partesDelArchivo);
		list_add_in_index(nodosFinalesAPLanificar,j,nodofin);
		j++;
	}

	list_add_all(listaDeWorkerTotales,nodosFinalesAPLanificar);

	pthread_mutex_unlock(&mutexJobsAPlanificar);

	list_destroy(partesDelArchivo);
	list_destroy(nodosFinalesAPLanificar);

	return respuestaAMaster;

}

//Ya Usa la nueva estructura
void actualizarListaDeWorkersTotales(t_list* listaDeWorkersAPLanificar,
		int disponibilidadBase) {

	int nodo1;
	int nodo2;
	int a = 0;
	while (a < list_size(listaDeWorkersAPLanificar)) {

		UbicacionBloquesArchivo2* bloque = list_get(listaDeWorkersAPLanificar,a);
		nodo1 = bloque->nodo1;
		nodo2 = bloque->nodo2;
		if (!estaNodorEnLaListaDeTotales(nodo1)) {

			nodoParaPlanificar* nodoA = crearNodoParaPlanificar(
					bloque->nodo1, disponibilidadBase, 0,
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
					bloque->nodo2, disponibilidadBase, 0,
					bloque->parteDelArchivo);
			logInfo("Se va agregar el nodo %i", nodo2);

			logInfo("Tamanio Lista de workersTotales antes de add %i",
					list_size(listaDeWorkerTotales));
			list_add(listaDeWorkerTotales, nodoB);
			logInfo("Tamanio Lista de workersTotales %i",
					list_size(listaDeWorkerTotales));
		} else {
			//si ya esta el nodo en la lista, agrega una parte de archivo
			agregarPartedeArchivoANodo(nodo2, bloque->parteDelArchivo);
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
bool dev_mayor_disponib(nodoParaPlanificar* nodo1,
		nodoParaPlanificar* nodo2) {
	if (nodo1->disponibilidad >= nodo2->disponibilidad) {

		if (nodo1->disponibilidad == nodo2->disponibilidad) {
			if (nodo1->carga <= nodo2->carga) {
				return true;
			} else {
				return false;
			}
		}
		return true;

	} else {
		return false;
	}
}


void nodoConMayorDisponibilidadClock(t_list* nodos) {
// ordenar la lista segun la disponibilidad y si esta fuera la misma segun la carga
	list_sort(nodos, dev_mayor_disponib);
}

void nodoConMayorDisponibilidadW_Clock(t_list* nodos) {

	list_sort(nodos, dev_mayor_disponib);
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
		t_list* nodosFinalesAPLanificar, int desplazamientoPuntero, int parte){

	int desplazamiento = desplazamientoPuntero + 1;
	while(desplazamiento != desplazamientoPuntero){
		if(desplazamiento == list_size(nodosFinalesAPLanificar)){

			desplazamiento = 0;
		}

		punteroClockAuxiliar = list_get(nodosFinalesAPLanificar,desplazamiento);
		if(estaParteEnNodo(parte,punteroClockAuxiliar)){

			return desplazamiento;
		}else{

			desplazamiento ++;
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

t_list* armarRespuestaTransformacionYAMA(t_list* nodosFinalesAPLanificar,t_list* listaDeWorkersAPlanificar){

	t_list* listaDeRespuestaTransformacionYAMA = list_create();

	int a = 0;
	int b = 0;
	int parte;
	int nodoDeLaParte;
	int bloqueDeLaParte;
	int bytesOcupadosDeLaParte;
	char* ip;
	int puerto;

	while(a < list_size(nodosFinalesAPLanificar)){

		nodoParaPlanificar* nodo = list_get(nodosFinalesAPLanificar,a);

		while(b< list_size(nodo->partesAplanificar)){

			char* archivoTemporal;
			parte = list_get(nodo->partesAplanificar,b);
			nodoDeLaParte = nodo->nodo;
			bloqueDeLaParte = bloqueOcupadoPorLaParteEnElNodo(parte,nodoDeLaParte,listaDeWorkersAPlanificar);
			bytesOcupadosDeLaParte = bytesocupadosPorLaParte(parte,listaDeWorkersAPlanificar);
			Info_Workers* worker = list_get(ipYPuertoWoerkers,(nodoDeLaParte-1));
			ip =worker->ipWorker;
			puerto = worker->puerto;
			variableNombreTransformacion++;
			archivoTemporal = generarNombreArchivoTransformacion(variableNombreTransformacion);

			RespuestaTransformacionYAMA* respuesta = setRespuestaTransformacionYAMA(nodoDeLaParte,puerto,
					ip,bloqueDeLaParte,bytesOcupadosDeLaParte,archivoTemporal);

			list_add(listaDeRespuestaTransformacionYAMA,respuesta);

			b++;

		}

		b=0;
		a++;

	}
	return listaDeRespuestaTransformacionYAMA;
}



int bytesocupadosPorLaParte(int parte,t_list* listaDeWorkersAPlanificar){

	int a = 0;
	while(a < list_size(listaDeWorkersAPlanificar)){

		UbicacionBloquesArchivo2* bloque = list_get(listaDeWorkersAPlanificar,a);

		if(bloque->parteDelArchivo == parte){

			return bloque->bytesOcupados;
		}else{

			a++;

		}

	}

	logInfo("no se encontro bytes ocupados del archivo");
	return -1;

}

int bloqueOcupadoPorLaParteEnElNodo(int parte,int nodo, t_list* listaDeWorkersAPlanificar){

	int a = 0;
	while(a < list_size(listaDeWorkersAPlanificar)){
		UbicacionBloquesArchivo2* ubi = list_get(listaDeWorkersAPlanificar,a);
		if(ubi->parteDelArchivo == parte){
			if(ubi->nodo1 == nodo){

				return ubi->desplazamiento1;
			}
			if(ubi->nodo2 == nodo){

				return ubi->desplazamiento2;
			}
		}

		a++;
	}

	logInfo("No se encontro el bloque donde se encuentra la parte %i", parte);
	return -1;

}

void algoritmoPrincipal(t_list* nodosFinalesAPLanificar,t_list* partesDelArchivo,int disponibilidadBase){

	nodoParaPlanificar* punteroClock;
	nodoParaPlanificar* punteroClockAuxiliar;

	logInfo("Ordenar lista de nodos a planificar por disponibilidad y carga");
	nodoConMayorDisponibilidadClock(nodosFinalesAPLanificar);

	int k = 0;
	while( k< list_size(nodosFinalesAPLanificar)){
		nodoParaPlanificar* nodos = list_get(nodosFinalesAPLanificar,k);
		logInfo("Ordenamiento : %i",nodos->nodo);
		k++;
	}

	int t = 0;
	int desplazamientoPuntero = 0;
	int indexParaActualizar;// me devuelve donde tengo que actualizar segun el punteroauxiliar

	while(t < list_size(partesDelArchivo)){

		int parte = list_get(partesDelArchivo, t);
		logInfo("Se planifica la parte %i",parte);

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
				if(desplazamientoPuntero == list_size(nodosFinalesAPLanificar)){

					desplazamientoPuntero = 0;

				}
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

				}else{
					nodoParaPlanificar* nodo = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodo->disponibilidad --;
					list_add(nodo->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodo);
					t ++;

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
				if(desplazamientoPuntero == list_size(nodosFinalesAPLanificar)){

					desplazamientoPuntero = 0;

				}
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
				}else{
					nodoParaPlanificar* nodo = list_remove(nodosFinalesAPLanificar,indexParaActualizar);
					nodo->disponibilidad --;
					list_add(nodo->partesAplanificar,parte);
					list_add_in_index(nodosFinalesAPLanificar,indexParaActualizar,nodo);
					t ++;

				}
			}

		}
	}

}


