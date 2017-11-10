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
	while(count < cantNodos){
			fprintf(fp, "Nodo%d", list_get(tablaNodos.listaNodos,count));
			count++;
			if (count<cantNodos)fprintf(fp, ",");
	}
	fprintf(fp,"]\n");

	count = 0;
	bloques_nodo* bloque;
	while(count < cantNodos){
		bloque = list_get(tablaNodos.listaCapacidadNodos,count);
		fprintf(fp,"Nodo%dTotal=%d\nNodo%dLibre=%d\n",bloque->nodo,bloque->bloquestotales,bloque->nodo,bloque->bloqueslibres);
		count++;
	}

	fclose(fp);

	return 0;

}

t_list* distribuirBloques(t_list* bloques, t_list* nodos){
	//RECORRO CADA ARCHIVO Y SE LO ASIGNO A DOS NODOS DISTINTOS
	int tamanioListaBloques = list_size(bloques);
	int indiceBloque = 0;
	t_list* listaUbicacionesBloquesArchivos = list_create();

	while(indiceBloque<tamanioListaBloques){

		char* bloque = list_get(bloques,indiceBloque);
		int indiceOriginal = elegirNodo(nodos);
		bitMap* nodoConOriginal = list_remove(nodos,indiceOriginal);
		int indiceConCopia = elegirNodo(nodos);
		bitMap* nodoConCopia = list_remove(nodos,indiceConCopia);
		int parteDelNodoDelOriginal = actualizarBitmapDelNodo(&nodoConOriginal);
		int parteDelNodoDeLaCopia = actualizarBitmapDelNodo(&nodoConCopia);
		printf("Parte del nodo del original: %d\n",parteDelNodoDelOriginal);
		printf("Parte del nodo de la copia: %d\n", parteDelNodoDeLaCopia);
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

		indiceBloque++;
	}
	return listaUbicacionesBloquesArchivos;
	//GUARDO LAS ESTRUCTURAS PARA MANDARSELA A YAMA AL TERMINAR EL PROCESO DE DIVISION DE ARCHIVOS.
}

int elegirNodo(t_list* nodos){
	int i = 1;
	int indice=0;
	bitMap* nodoMasVacio = list_get(nodos,0);
	while(i<list_size(nodos)){
		if(bloquesLibres(list_get(nodos,i))>bloquesLibres(nodoMasVacio)){
			nodoMasVacio = list_get(nodos,i);
			indice = i;
		}
		i++;
	}
	return indice;
}

int bloquesLibres(bitMap* nodo){
	int i,libres=0;
	for(i=0;i<20;i++){
		if(nodo->bitmap[i] == 0) libres++;
	}
	return libres;
}

int actualizarBitmapDelNodo(bitMap* nodo){
	int i;
	for(i=0;i<20;i++){
		if(nodo->bitmap[i] == 0){
			nodo->bitmap[i] = 1;
			return i;
		}
	}
	return(-1);
}

t_list* tablaNodosToNodos(t_list* listaNodos){

	int count = 0;
	t_list* nodos = list_create();
	int tamLista = list_size(listaNodos);
	int id;
	int i;
	while(count<tamLista){
		bitMap* mapa = malloc(sizeof(bitMap)*tamLista);
		id = list_get(listaNodos,count);

		mapa->id_nodo = id;
		for( i=0; i<20;i++){
			mapa->bitmap[i]=0;
			printf("nodo:%d desplazamiento:%d, empty:%d\n",mapa->id_nodo,i, mapa->bitmap[i]);
		}
		list_add(nodos,mapa);
		count++;
	}

	return(nodos);
}

t_list* cargarNodos(){

		//EJEMPLO DE NODOS CARGADOS
		tabla_de_nodos.tamanio=40;
		tabla_de_nodos.bloqueslibres=15;
		tabla_de_nodos.listaNodos = list_create();
		tabla_de_nodos.listaCapacidadNodos = list_create();

		int* numero1 = 1;
		int* numero2 = 2;
		list_add(tabla_de_nodos.listaNodos,numero1);
		list_add(tabla_de_nodos.listaNodos,numero2);

		bloques_nodo* nodo1 = malloc(sizeof(bloques_nodo));
		nodo1->nodo=1;
		nodo1->bloquestotales=20;
		nodo1->bloqueslibres=10;

		list_add(tabla_de_nodos.listaCapacidadNodos,nodo1);

		bloques_nodo* nodo2 = malloc(sizeof(bloques_nodo));;
		nodo2->nodo=2;
		nodo2->bloquestotales=20;
		nodo2->bloqueslibres=5;

		list_add(tabla_de_nodos.listaCapacidadNodos,nodo2);

		int resp = crearRegistroArchivoNodos(tabla_de_nodos);
		if(resp==0) printf("\narchivo creado correctamente\n");
		else printf("\narchivo no creado.\n");

	//	int cantNodos = list_size(tabla_de_nodos.listaNodos);
	//	t_list* lista_nodos;

		//lista_nodos = tablaNodosToNodos(tabla_de_nodos.listaNodos);
		//FIN EJEMPLO NODOS CARGADOS

}

int showBitMap(t_list* Mapa_de_bits){

	int cuenta = 0,count = 0;
	printf("\n\n");
	bitMap* mapa;
	while(cuenta<list_size(Mapa_de_bits)){

		mapa = list_get(Mapa_de_bits,cuenta);
		int id = mapa->id_nodo;
		printf("%d\n",id);
		while(count<20){
			printf("nodo:%d desplazamiento:%d, empty:%d\n",mapa->id_nodo,count, mapa->bitmap[count]);
			count++;
		}
		count=0;
		cuenta++;
	}
	return(1);
}
