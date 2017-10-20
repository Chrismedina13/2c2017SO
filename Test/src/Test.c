/*
 ============================================================================
 Name        : Test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Test.h"
#include <stdio.h>


void test1(void);




int main() {
  CU_initialize_registry();

  CU_pSuite prueba = CU_add_suite("Suite de prueba", NULL, NULL);

  CU_add_test(prueba, "uno", test1);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}


void test1() {

	UbicacionBloquesArchivo* ubi1 = crearUbicacionBloquesArchivos(0,0,1,12,2,22);
	UbicacionBloquesArchivo* ubi2 = crearUbicacionBloquesArchivos(1,0,3,12,1,22);
	UbicacionBloquesArchivo* ubi3 = crearUbicacionBloquesArchivos(2,0,2,12,3,22);

	listaDeWorkerTotales = list_create();
	t_list* listaDenodosAPlanificar = list_create();

	list_add(listaDenodosAPlanificar,ubi1);
	list_add(listaDenodosAPlanificar,ubi2);
	list_add(listaDenodosAPlanificar,ubi3);

	t_list* resultado = list_create();

	resultado = planificar(listaDenodosAPlanificar,"Clock",2);
	CU_ASSERT_EQUAL(list_size(resultado), 3);
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

t_list* dev_nodos_a_planificar() {
	//busco de la lista de nodos a planificar aquellos que tienen partes de archivo
	t_list* nodosFinalesAPLanificar = list_create();
	int b = 0;
	while (b < list_size(listaDeWorkerTotales)) {
		nodoParaPlanificar* nodo = list_get(listaDeWorkerTotales, b);
		if (!list_is_empty(nodo->partesDelArchivo)) {
			nodoParaPlanificar* nodoConBloques = list_remove(
					listaDeWorkerTotales, b);

			list_add(nodosFinalesAPLanificar, nodoConBloques);
		}
		b++;
	}
	return nodosFinalesAPLanificar;
}

t_list* planificarConW_Clock(t_list* listaDeWorkersAPlanificar,
		int disponibilidadBase) {

	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,
			disponibilidadBase);
	//busco de la lista de nodos a planificar aquellos que tienen partes de archivo
	t_list*nodosFinalesAPLanificar= dev_nodos_a_planificar();
	//cm falta liberar nodosFinalesAPLanificar

	return nodosFinalesAPLanificar;
}

t_list* planificarConClock(t_list* listaDeWorkersAPlanificar,
		int disponibilidadBase) {

	actualizarListaDeWorkersTotales(listaDeWorkersAPlanificar,
			disponibilidadBase);

	printf("Resultado %i ",list_size(listaDeWorkerTotales));


	t_list*nodosFinalesAPLanificar= dev_nodos_a_planificar();
	return nodosFinalesAPLanificar;
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
			list_add(listaDeWorkerTotales, nodoA);
		} else {
			//si ya esta el nodo en la lista, agrega una parte de archivo
			agregarPartedeArchivoANodo(nodo1, bloque->parteDelArchivo);
		}
		if (!estaNodorEnLaListaDeTotales(nodo2)) {

			nodoParaPlanificar* nodoB = crearNodoParaPlanificar(
					bloque->ubicacionCopia2.nodo, disponibilidadBase, 0,
					bloque->parteDelArchivo);
			list_add(listaDeWorkerTotales, nodoB);
		} else {
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
			list_add_in_index(listaDeWorkerTotales, nodoaModificar, i);
		}
	}
}
bool estaNodorEnLaListaDeTotales(int nodo) {

	int i = 0;

	if (list_size(listaDeWorkerTotales) == 0) {

		return false;
	} else {
		while (i < list_size(listaDeWorkerTotales)) { // modificar esto

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

