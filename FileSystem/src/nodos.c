/*
 * nodos.c
 *
 *  Created on: 12/10/2017
 *      Author: utnso
 */
#include "Headers/nodos.h"
#include "Headers/FileSystem.h"
#include "SO_lib/estructuras.h"


int maxNodos= 20;
int crearRegistroArchivoNodos(int tamanio,int libres, int nodos, char * nodosPtr){ //puntero a la lista de struct bloques_nodos

	FILE * fp = fopen("yamafs/metadata/nodos.bin", "w");
	if (!fp) {
	  perror("Error al abrir el Archivo");
	  return (-1);
	}

	int count = 0;
	int cantNodos= sizeof(nodos);

	fscanf(fp, "TAMANIO=%d\n LIBRE=%d\n NODOS=[", tamanio, libres); //carga la info del archivo

	while(count <= cantNodos){
	//	fscanf(fp, "Nodo%d", nodos[count]);
		if (count<= cantNodos-1)fscanf(fp, ",");
		count++;
	}
	fscanf(fp,"]");

	int count2=0;
	int nodoTot = 0;
	int nodoLibre = 0;

	while (count2 < cantNodos){
		while(index <= 1){
	//		nodoTot = nodosPtr[index].bloquestotales;
	//		nodoLibre = nodosPtr[index].bloqueslibres;

			fscanf(fp, "Nodo%dTotal=%d\n Nodo%dLibre=%d\n", index, nodoTot, index, nodoLibre);
		//	index++;
		}
		count2++;
	}
	close(fp);

	return(1);

}


t_list* distribuirBloques(t_list* bloques, t_list* nodos){
	//RECORRO CADA ARCHIVO Y SE LO ASIGNO A DOS NODOS DISTINTOS
	int tamanioListaBloques = list_size(bloques);
	int indiceBloque = 0;
	t_list* listaUbicacionesBloquesArchivos = list_create();

	while(indiceBloque<tamanioListaBloques){
	char* bloque = list_get(bloques,indiceBloque);
	int indiceConOriginal = elegirNodo(nodos);
		Nodo* nodoConOriginal = list_remove(nodos,indiceConOriginal);
		int indiceConCopia = elegirNodo(nodos);
		Nodo* nodoConCopia = list_remove(nodos,indiceConCopia);
		int parteDelNodoDelOriginal = actualizarBitmapDelNodo(&nodoConOriginal);
		int parteDelNodoDeLaCopia = actualizarBitmapDelNodo(&nodoConCopia);
		list_add(nodos,nodoConOriginal);
		list_add(nodos,nodoConCopia);

		//CREO LA ESTRUCTURA UbicacionBloquesArchivo PARA ENVIARSELA A YAMA
		UbicacionBloquesArchivo* ubicacionBloquesArchivo;
		ubicacionBloquesArchivo->bytesOcupados = string_length(bloque);
		ubicacionBloquesArchivo->parteDelArchivo = indiceBloque;
		ubicacionBloquesArchivo->ubicacionCopia1.bloqueDelNodoDeLaCopia = parteDelNodoDelOriginal;
		ubicacionBloquesArchivo->ubicacionCopia1.nodo = nodoConOriginal->id_nodo;
		ubicacionBloquesArchivo->ubicacionCopia2.bloqueDelNodoDeLaCopia = parteDelNodoDeLaCopia;
		ubicacionBloquesArchivo->ubicacionCopia1.nodo = nodoConCopia->id_nodo;
		list_add(listaUbicacionesBloquesArchivos,ubicacionBloquesArchivo);
	}
	return listaUbicacionesBloquesArchivos;
	//GUARDO LAS ESTRUCTURAS PARA MANDARSELA A YAMA AL TERMINAR EL PROCESO DE DIVISION DE ARCHIVOS.
}

int elegirNodo(t_list* nodos){
	int i = 1;
	int indice=0;
	Nodo* nodoMasVacio = list_get(nodos,0);
	while(i<list_size(nodos)){
		if(bloquesLibres(list_get(nodos,i))>bloquesLibres(nodoMasVacio)){
			indice = i;
			i=list_size(nodos);
		}
	}
	return indice;
}

int bloquesLibres(Nodo* nodo){
	int i,libres=0;
	for(i=0;i<20;i++){
		if(nodo->bitmap[i] == 0) libres++;
	}
	return libres;
}

int actualizarBitmapDelNodo(Nodo* nodo){
	int i;
	int indiceBloqueAGuardar;
	for(i=0;i<20;i++){
		if(nodo->bitmap[i] == 0){
			indiceBloqueAGuardar = i;
			nodo->bitmap[i] = 1;
			i=20;
		}
	}
	return indiceBloqueAGuardar;
}

static Nodo* inicializarEstructuras(int id){
	Nodo* nuevoNodo = malloc(sizeof(Nodo));
	int i;
	for(i=0; i<20; i++) nuevoNodo->bitmap[i] = 0;
	nuevoNodo->id_nodo = id;
    return nuevoNodo;
}


