/*
 * nodos.c
 *
 *  Created on: 12/10/2017
 *      Author: utnso
 */
#include "Headers/nodos.h"
#include "SO_lib/estructuras.h"


int maxNodos= 20;
/*
 * SUPONIENDO QUE PREVIAMENTE YA SE ENCUENTRA INICIALIZADA LA SIGUIENTE ESTRUCTURA:

typedef struct tabla_nodos {
	int tamanio; //cantidad de bloques TOTAL ENTRE TODOS LOS NODOS
	int bloqueslibres; //cantidad de bloques libres entre todos los nodos
	t_list* listaNodos; //nodo1, nodo2, nodo3
	t_list* listaCapacidadNodos; //total por nodo y libre t_list* bloques_nodos;
}tabla_nodos;

 */
int crearRegistroArchivoNodos(tabla_nodos tablaNodos){

	FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/yamafs/metadata/nodos.txt", "w");
	if (!fp) {
	  perror("Error al abrir el Archivo");
	  return (-1);
	}

	int count = 0;
	int cantNodos= list_size(tablaNodos.listaNodos);

	fprintf(fp, "TAMANIO=%d\n LIBRE=%d\n NODOS=[", tablaNodos.tamanio, tablaNodos.bloqueslibres); //carga la info del archivo
	while(count <= cantNodos){
			fprintf(fp, "Nodo%d", list_get(tablaNodos.listaNodos,count));
			count++;
			if (count<=cantNodos)fprintf(fp, ",");
	}
	fprintf(fp,"]\n");

	count = 0;
	bloques_nodo* bloque;
	while(count <= cantNodos){
		bloque = list_get(tablaNodos.listaCapacidadNodos,count);
		fprintf("Nodo%dTotal=%d\nNodo%dLibre=%d\n",bloque->nodo,bloque->bloquestotales,bloque->nodo,bloque->bloqueslibres);
	}

	close(fp);

	return 0;;

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


