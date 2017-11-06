/*
 * recuperacionFS.c
 *
 *  Created on: 22/10/2017
 *      Author: utnso
 */
#include "Headers/recuperacionFS.h"
#include "Headers/FileSystem.h"


int recuperarTablaDeNodos(){

		FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/yamafs/metadata/nodos.bin", "r");
		if (!fp) {
		  perror("Error al abrir el Archivo de directorios");
		  return (-1);
		}

		/*TRABAJO LAS PRIMERAS TRES LINEAS DEL ARCHIVO
			TAMANIO=300
			LIBRE=171
			NODOS=[Nodo1,​ ​ Nodo2,​ ​ Nodo3]
		*/
		char* tamanioS,libreS,nodosS;
		if(feof(fp)){
		  fscanf(fp, "%s %s %s", tamanioS, libreS, nodosS);
		}
		//TRABAJO EL TAMANIO
		tamanioS = string_substring(tamanioS,8,string_length(tamanioS));
		tabla_de_nodos.tamanio = atoi(tamanioS);

		//TRABAJO LA CANTIDAD LIBRE
		libreS = string_substring(libreS,6,string_length(libreS));
		tabla_de_nodos.bloqueslibres = atoi(libreS);

		//TRABAJO LA LISTA DE NODOS
		nodosS = string_substring(nodosS,7,string_length(nodosS));
		nodosS = string_reverse(nodosS);
		nodosS = string_substring(nodosS,1,string_length(nodosS));
		nodosS = string_reverse(nodosS);
		char** puntero = string_split(nodosS,",");
		int i = 0;
		int nodo;
		char* numeroDeNodo;
		while(puntero[i]!='\0'){
			numeroDeNodo = string_substring(puntero[i],4,sizeof(puntero[i]));
			list_add(&(tabla_de_nodos.listaNodos),atoi(numeroDeNodo));
			i++;
		}

		/*TRABAJO LAS SIGUIENTE LINEAS
		 	Nodo1Total=50
			Nodo1Libre=16
			Nodo2Total=100
			Nodo2Libre=80
			Nodo3Total=150
			Nodo3Libre=75
		 */
		char* nodototalS,nodolibreS,idNodoS;
		while(feof(fp)){
			fscanf(fp, "%s %s", nodototalS, nodolibreS);
			idNodoS = string_substring(nodototalS, 4, sizeof(nodototalS));
			idNodoS = string_substring_until(idNodoS,1);
			nodototalS = string_substring(nodototalS, 11, sizeof(nodototalS));
			nodolibreS = string_substring(nodolibreS, 11, sizeof(nodolibreS));

			bloques_nodo* nodo;
			nodo->nodo = atoi(idNodoS);
			nodo->bloquestotales = atoi(nodototalS);
			nodo->bloqueslibres = atoi(nodolibreS);

			list_add(&(tabla_de_nodos.listaCapacidadNodos),nodo);

		}
		return 0;
}



