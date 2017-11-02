/*
 * nodos.c
 *
 *  Created on: 12/10/2017
 *      Author: utnso
 */
#include "Headers/nodos.h"
#include "Headers/FileSystem.h"



int crearRegistroArchivo(int tamanio,int libres, int nodos, char * nodosPtr){ //puntero a la lista de struct bloques_nodos

	FILE * fp = fopen("yamafs/metadata/nodos.bin", "w");
	if (!fp) {
	  perror("Error al abrir el Archivo");
	  return (-1);
	}

	int count = 0;
	int cantNodos= sizeof(nodos);

	fscanf(fp, "TAMANIO=%d\n LIBRE=%d\n NODOS=[", tamanio, libres); //carga la info del archivo

	while(count <= cantNodos){
		fscanf(fp, "Nodo%d", nodos[count]);
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
			index++;
		}
		count2++;
	}
	close(fp);

	return(1);

}



