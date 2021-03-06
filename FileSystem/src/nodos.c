/*
 * nodos.c
 *
 *  Created on: 12/10/2017
 *      Author: utnso
 */
#include "Headers/nodos.h"
#include <stdbool.h>

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

	FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/yamafs/metadata/nodos.bin", "w");
	if (!fp) {
	  perror("Error al abrir el Archivo");
	  return (-1);
	}

	int count = 0;
	int cantNodos= list_size(tablaNodos.listaNodos);

	fprintf(fp, "TAMANIO=%d\nLIBRE=%d\nNODOS=[", tablaNodos.tamanio, tablaNodos.bloqueslibres); //carga la info del archivo
	while(count < cantNodos){
			fprintf(fp, "Nodo%d", list_get(tablaNodos.listaNodos,count)); //tira warning, pero los elementos de la lista son int, y funciona? asi que va
			count++;
			if (count<cantNodos)fprintf(fp, ",");
	}
	fprintf(fp,"]\n");

	count = 0;
	bloques_nodo* bloque;
	while(count < cantNodos){
		bloque = list_get(tablaNodos.listaCapacidadNodos,count);
		fprintf(fp,"Nodo%dTotal=%d\nNodo%dLibre=%d\n",bloque->idNodo,bloque->bloquesTotales,bloque->idNodo,bloque->bloquesLibres);
		count++;
	}

	free(bloque);
	fclose(fp);

	return 0;

}

void * distribuirBloques(int indiceArchivo){

	/*Recibe una lista de char* con bloques de texto (bloques) y una lista de bitMap (nodos)
	 * Devuelve una lista de ubicacionBloquesArchivo
	 */

	//RECORRO CADA ARCHIVO Y SE LO ASIGNO A DOS NODOS DISTINTOS

	int cantBloques = list_size(tabla_de_archivos[indiceArchivo].bloques);
	int indiceBloque = 0;
	tabla_de_archivos[indiceArchivo].ubicaciones = list_create();
	char* bloque;
	bloques_nodo* bitMapNodo1;
	bloques_nodo* bitMapNodo2;
	int index1, indexList1, index2, indexList2;
	int desplazamiento1, desplazamiento2;

	while(indiceBloque<cantBloques){

		UbicacionBloquesArchivo2* bloquesPtr = malloc(sizeof(int)*6);
		bloque = list_get(tabla_de_archivos[indiceArchivo].bloques,indiceBloque);

		//elije los 2 nodos mas vacios

int asd;
		indexList1 = elegirNodo(asd);
		bitMapNodo1 = list_get(tabla_de_nodos.listaCapacidadNodos,indexList1);
		index1 = bitMapNodo1->idNodo;
		desplazamiento1 = buscarBloqueVacio(bitMapNodo1); //busca el vacio, devuelve eso y a su vez ya lo actualiza

		if(desplazamiento1==-1){
			logInfo("Nodo lleno."); //Si distribuye bien y checkea bien no deberia entrar aca
		}
		logInfo("Ubico la copia 1 del bloque %d, en el nodo %d, desplazamiento %d",indiceBloque, index1,desplazamiento1);
		list_remove(tabla_de_nodos.listaCapacidadNodos,indexList1);

		//printf("Nodo para guardar el bloque:%d Desplazamiento:%d \n", index1,desplazamiento1);

		indexList2 = elegirNodo(asd);
		bitMapNodo2 = list_get(tabla_de_nodos.listaCapacidadNodos,indexList2);
		index2 = bitMapNodo2->idNodo;
		desplazamiento2 = buscarBloqueVacio(bitMapNodo2); //lo devuleve y lo pone en ocupado

		if(desplazamiento2==-1){
			logInfo("Nodo lleno."); //Si distribuye bien y checkea bien no deberia entrar aca
		}
		logInfo("Ubico la copia 2 del bloque %d, en el nodo %d, desplazamiento %d",indiceBloque, index2,desplazamiento2);

		list_add(tabla_de_nodos.listaCapacidadNodos,bitMapNodo1);

		//cargo los datos de los bloques en tabla_de_archivos

		bloquesPtr->parteDelArchivo = indiceBloque;
		bloquesPtr->desplazamiento1 = desplazamiento1;
		bloquesPtr->nodo1 = index1;
		bloquesPtr->desplazamiento2 = desplazamiento2;
		bloquesPtr->nodo2 = index2;
		bloquesPtr->bytesOcupados = string_length(bloque);

		logInfo("tam:%d parteNum:%d\nNodo:%d, Desplazamiento:%d\nNodo:%d, Desplazamiento:%d",
				bloquesPtr->bytesOcupados,
				bloquesPtr->parteDelArchivo,
				bloquesPtr->nodo1,
				bloquesPtr->desplazamiento1,
				bloquesPtr->nodo2,
				bloquesPtr->desplazamiento2);

		list_add(tabla_de_archivos[indiceArchivo].ubicaciones,bloquesPtr);

	//	free(bloquesPtr);
	//	free(bitMapNodo1);
	//	free(bitMapNodo2);

		indiceBloque++;
	}
	//GUARDO LAS ESTRUCTURAS PARA MANDARSELA A YAMA AL TERMINAR EL PROCESO DE DIVISION DE ARCHIVOS.
}

void * distribuirYEnviarBloques(int indiceArchivo){

	/*Recibe una lista de char con bloques de texto (bloques) y una lista de bitMap (nodos)
	 * Devuelve una lista de ubicacionBloquesArchivo
	 */


	//para distribuir
	int cantBloques = list_size(tabla_de_archivos[indiceArchivo].bloques);
	int indiceBloque = 0;
	tabla_de_archivos[indiceArchivo].ubicaciones = list_create();

	int desplazamiento1;
	int desplazamiento2;


	//RECORRO CADA ARCHIVO Y SE LO ASIGNO A DOS NODOS DISTINTOS


	while(indiceBloque<cantBloques){


		UbicacionBloquesArchivo2* bloquesPtr = malloc(sizeof(int)*6);
		char* bloque = list_get(tabla_de_archivos[indiceArchivo].bloques,indiceBloque);
		int tamanioSetBloque = strlen(bloque);

		sortListaNodos();

		//elije los 2 nodos mas vacios y los envia

		//nodo 1

		bloques_nodo* bitMapNodo = list_remove(tabla_de_nodos.listaCapacidadNodos,0);
		bloques_nodo* nodo1Memoria = malloc(sizeof(int)*165);

		memcpy(nodo1Memoria, bitMapNodo, sizeof(int)*165);

		desplazamiento1 = buscarBloqueVacio(nodo1Memoria); //busca el vacio, devuelve eso y a su vez ya lo actualiza

		if(desplazamiento1==-1){
			logInfo("Nodo lleno."); //Si distribuye bien y checkea bien no deberia entrar aca
		}

		//copia 1

		logInfo("voy a mandar a este FileDescriptor %d un mensaje de tamaño %d",
				nodo1Memoria->fileDescriptor,
				tamanioSetBloque);

		mensajesEnviadosADataNode(SET_BLOQUE,nodo1Memoria->fileDescriptor, bloque , tamanioSetBloque);

		char* desplazamiento1Serializado = serializeInt(desplazamiento1);

		send(nodo1Memoria->fileDescriptor, desplazamiento1Serializado, sizeof(int), 0);

		logInfo("Copia1 del bloque %d, esta en dataNode%d:desplazamiento%d",
				indiceBloque,
				nodo1Memoria->idNodo,
				desplazamiento1);

		//nodo 2

		bitMapNodo = list_remove(tabla_de_nodos.listaCapacidadNodos,0);
		bloques_nodo* nodo2Memoria= malloc(sizeof(int)*165);

		memcpy(nodo2Memoria, bitMapNodo, sizeof(int)*165);

		desplazamiento2 = buscarBloqueVacio(nodo2Memoria); //lo devuleve y lo pone en ocupado

		if(desplazamiento2==-1){
			logInfo("Nodo lleno."); //Si distribuye bien y checkea bien no deberia entrar aca
		}

		//copia 2

		logInfo("voy a mandar a este FileDescriptor %d un mensaje de tamaño %d",
				nodo2Memoria->fileDescriptor,
				tamanioSetBloque);

		mensajesEnviadosADataNode(SET_BLOQUE,nodo2Memoria->fileDescriptor, bloque,tamanioSetBloque);

		char* desplazamiento2Serializado = serializeInt(desplazamiento2);

		send(nodo2Memoria->fileDescriptor, desplazamiento2Serializado, sizeof(int), 0);

		logInfo("Copia2 del bloque %d, esta en dataNode%d:desplazamiento%d",
				indiceBloque,
				nodo2Memoria->idNodo,
				desplazamiento2);


		//cargo los datos de los bloques en tabla_de_archivos

		bloquesPtr->parteDelArchivo = indiceBloque;
		bloquesPtr->nodo1 = nodo1Memoria->idNodo;
		bloquesPtr->desplazamiento1 = desplazamiento1;
		bloquesPtr->nodo2 = nodo2Memoria->idNodo;
		bloquesPtr->desplazamiento2 = desplazamiento2;
		bloquesPtr->bytesOcupados = string_length(bloque);

		logInfo("tam:%d parteNum:%d\nNodo:%d, Desplazamiento:%d\nNodo:%d, Desplazamiento:%d",bloquesPtr->bytesOcupados,
				bloquesPtr->parteDelArchivo,bloquesPtr->nodo1,
				bloquesPtr->desplazamiento1,bloquesPtr->nodo2, bloquesPtr->desplazamiento2);

		list_add(tabla_de_archivos[indiceArchivo].ubicaciones,bloquesPtr);


		list_add(tabla_de_nodos.listaCapacidadNodos, nodo1Memoria);
		list_add(tabla_de_nodos.listaCapacidadNodos, nodo2Memoria);
		//free(bitMapNodo);
		indiceBloque++;

	}
	//GUARDO LAS ESTRUCTURAS PARA MANDARSELA A YAMA AL TERMINAR EL PROCESO DE DIVISION DE ARCHIVOS.
}
/*
t_list* distribuirBloques2(t_list* bloques, t_list* mapa_de_bits_original, int indiceArchivo){

	Recibe una lista de char* con bloques de texto (bloques) y una lista de bitMap (nodos)
	 * Devuelve una lista de ubicacionBloquesArchivo


	//RECORRO CADA ARCHIVO Y SE LO ASIGNO A DOS NODOS DISTINTOS

	t_list* mapa_de_bits = copyList(mapa_de_bits_original);

	int cantBloques = list_size(bloques);
	int indiceBloque = 0;
	//t_list* listaUbicacionesBloquesArchivos = list_create();
	tabla_de_archivos[indiceArchivo].ubicaciones = list_create();

	while(indiceBloque<cantBloques){

		//carga el bloque a guardar (char*)

		char* bloque = list_get(bloques,indiceBloque);
		int nodosLlenos=0;

		//elije los 2 nodos mas vacios

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


		int count = 0;
		int index1, indexList1, index2, indexList2;
		int desplazamiento1, desplazamiento2;
		bloques_nodo* bitMapNodo1 = malloc(sizeof(bloques_nodo));
		bloques_nodo* bitMapNodo2 = malloc(sizeof(bloques_nodo));

		indexList1 = elegirNodo(mapa_de_bits);
		bitMapNodo1 = list_get(mapa_de_bits,indexList1);
		index1 = bitMapNodo1->idNodo;
		desplazamiento1 = buscarBloqueVacio2(bitMapNodo1); //busca el vacio, devuelve eso y a su vez ya lo actualiza
		if(desplazamiento1==-1){
			nodosLlenos=-1;
		}


		list_remove(mapa_de_bits,indexList1);

		//printf("Nodo para guardar el bloque:%d Desplazamiento:%d \n", index1,desplazamiento1);

		indexList2 = elegirNodo(mapa_de_bits);
		bitMapNodo2 = list_get(mapa_de_bits,indexList2);
		index2 = bitMapNodo2->idNodo;
		desplazamiento2 = buscarBloqueVacio2(bitMapNodo2); //lo devuleve y lo pone en ocupado
		if(desplazamiento2==-1){
			nodosLlenos=-1;
		}


		//printf("Nodo para guardar el bloque:%d Desplazamiento:%d \n", index2,desplazamiento2);

		list_add(mapa_de_bits,bitMapNodo1);

		//cargo los datos de los bloques en tabla_de_archivos

		UbicacionBloquesArchivo* ubicacionBloquesArchivo = malloc(sizeof(UbicacionBloquesArchivo));
		ubicacionBloquesArchivo->bytesOcupados = string_length(bloque);
		ubicacionBloquesArchivo->parteDelArchivo = indiceBloque;
		ubicacionBloquesArchivo->ubicacionCopia1.desplazamiento = desplazamiento1;
		ubicacionBloquesArchivo->ubicacionCopia1.nodo = index1;
		ubicacionBloquesArchivo->ubicacionCopia2.desplazamiento = desplazamiento2;
		ubicacionBloquesArchivo->ubicacionCopia2.nodo = index2;
		//printf("tam:%d parteNum:%d\nNodo:%d, Desplazamiento:%d\nNodo:%d, Desplazamiento:%d",ubicacionBloquesArchivo->bytesOcupados, indiceBloque,index1,
		//		desplazamiento1,index2, desplazamiento2);
		//list_add(tabla_de_nodos.listaCapacidadNodos,ubicacionBloquesArchivo);
		list_add(tabla_de_archivos[indiceArchivo].ubicaciones,ubicacionBloquesArchivo);//empieza a cargar el vector de archivos

	//	free(ubicacionBloquesArchivo);
	//	free(bitMapNodo1);
	//	free(bitMapNodo2);

		indiceBloque++;
	}
	return (tabla_de_archivos[indiceArchivo].ubicaciones);
	//GUARDO LAS ESTRUCTURAS PARA MANDARSELA A YAMA AL TERMINAR EL PROCESO DE DIVISION DE ARCHIVOS.
}

t_list* copyList(t_list* mapa_de_bits_original){

	int count=0;

	t_list* listaCopia = list_create();
	bloques_nodo infoNodoOriginal = list_get(mapa_de_bits_original,0);

	while(infoNodoOriginal!=NULL)
	{
		bloques_nodo infoNodoOriginal = list_get(mapa_de_bits_original,count);
	    bloques_nodo *infoNodo = (bloques_nodo *) malloc (sizeof(bloques_nodo));
	    infoNodo->idNodo=infoNodoOriginal->idNodo;
	    infoNodo->bloquesLibres =infoNodoOriginal->bloquesLibres;
	    infoNodo->bloquesTotales =infoNodoOriginal->bloquesTotales;
	    while(count<infoNodo){
	    	infoNodo->bitmap[count]=infoNodoOriginal->bitmap[count];
	    	count++;
	    }
	}

	return(listaCopia);


}
*/

int tamChar (char *cadena)
{
    int j=0;
    while (cadena[j] != '\0') {j++;}
    return j;
}

int elegirNodo(int indexNodoAnterior){

	int count = 0;
	int flag=0;
	int extraCount = 0;

	bloques_nodo* nodoMasVacio;
	bloques_nodo* nodoAComparar;

	if(count==indexNodoAnterior)count++;
	while(flag==0){

		nodoMasVacio= list_get(tabla_de_nodos.listaCapacidadNodos,count);
		if(nodoMasVacio->estado==1 && count!=indexNodoAnterior){
			flag=1;
			if(list_size(tabla_de_nodos.listaCapacidadNodos)==1){
				return(count);
			}
		}

		count++;
	}

	count = 0;



	while(count<list_size(tabla_de_nodos.listaCapacidadNodos)){

		if(count==indexNodoAnterior){
			count++;
		}
		if(count>=list_size(tabla_de_nodos.listaCapacidadNodos)){
			return (extraCount);
		}
		nodoAComparar = list_get(tabla_de_nodos.listaCapacidadNodos,count);

		if((bloquesLibres(nodoAComparar)>=bloquesLibres(nodoMasVacio)) && nodoAComparar->estado==1){
			nodoMasVacio = list_get(tabla_de_nodos.listaCapacidadNodos,count);
			extraCount = count;
		}

		count++;
	}

	return extraCount;
}

void* sortListaNodos(){

	list_sort(tabla_de_nodos.listaCapacidadNodos,bloquesLibresSort);
}

int bloquesLibres(bloques_nodo* nodo){
	/*int i,libres=0;
	for(i=0;i<(nodo->bloquesTotales);i++){
		if(nodo->bitmap[i] == 0) libres++;
	}
	return libres;
	*/
	int count=0;
	int libres=0;
	while(count<(nodo->bloquesTotales)){
		if(nodo->bitmap[count]==0)libres++;
		count++;
	}
	return libres;
}

bool bloquesLibresSort(bloques_nodo* nodo, bloques_nodo* nodo2){

	int count=0;
	int libres=0;
	while(count<(nodo->bloquesTotales)){
		if(nodo->bitmap[count]==0)libres++;
		count++;
	}

	int count2=0;
	int libres2=0;
	while(count2<(nodo2->bloquesTotales)){
			if(nodo2->bitmap[count2]==0)libres2++;
			count2++;
		}

	if(libres>libres2){
		return true;
	}
	return false;
}

int ocuparBloqueBitMap(bloques_nodo* nodo){ // en desuso, reemplazada por buscarBloqueVacio
	int i;
	for(i=0;i<nodo->bloquesTotales;i++){
		if(nodo->bitmap[i] == 0){
			nodo->bitmap[i] = 1;
			return i;
		}
	}
	return(-1);
}

int buscarBloqueVacio(bloques_nodo* nodo){

	/*Recibe un bloques_nodo
	 * Devuelve el desplazamiento (para anotarlo en donde corresponda) y setea el desplazamiento del bitmap como ocupado.
	 * Si el nodo esta lleno devuelve -1.
	 */

	int count = 0;
	while(count<(nodo->bloquesTotales)){
		if(nodo->bitmap[count]==0){
			nodo->bitmap[count]=1;
			nodo->bloquesLibres--;
			return(count);
		}
	count++;
	}
	return(-1);
}

/*
int buscarBloqueVacio2(bloques_nodo* nodo){

	Recibe un bloques_nodo
	 * Devuelve el desplazamiento (para anotarlo en donde corresponda).
	 * Si el nodo esta lleno devuelve -1.


	int count = 0;
	while(count<nodo->bloquesTotales){
		if(nodo->bitmap[count]==0){
			return(count);
		}
	count++;
	}
	return(-1);
}
*/

t_list* inicializarBitMapNodos(t_list* listaNodos){

	/*agarra la lista ListaNodos de tabla_de_nodos
	 * y con eso crea el bitmap para los nodos
	 * Devuelve una lista con los bitMaps (estrucutra bitMap)
	 */

	int count = 0;
	t_list* bitMapNodos = list_create();
	int cantNodos = list_size(listaNodos);
	int idNodo;
	int desplazamiento;
	bloques_nodo* mapa = malloc(sizeof(int)*165);
	while(count<cantNodos){

		idNodo = list_get(listaNodos,count);
		mapa->idNodo = idNodo;
		for( desplazamiento=0; desplazamiento<160;desplazamiento++){
			mapa->bitmap[desplazamiento]=0;
		//	printf("nodo:%d desplazamiento:%d, empty:%d\n",mapa->id_nodo,i, mapa->bitmap[i]);
		}
		list_add(bitMapNodos,mapa);
		count++;
	}

	//free(mapa);
	return(bitMapNodos);
}

/*
void cargarNodos(){ //en desuso

		//EJEMPLO DE NODOS CARGADOS
		tabla_de_nodos.tamanio=40;
		tabla_de_nodos.bloqueslibres=40;
		tabla_de_nodos.listaNodos = list_create();
		tabla_de_nodos.listaCapacidadNodos = list_create();

		int* numero1 = 1;
		int* numero2 = 2;
		list_add(tabla_de_nodos.listaNodos,numero1);
		list_add(tabla_de_nodos.listaNodos,numero2);

		bloques_nodo* nodo1 = malloc(sizeof(bloques_nodo));
		nodo1->idNodo=1;
		nodo1->bloquesTotales=20;
		nodo1->bloquesLibres=20;
		int i=0;
		while(i<20){
		nodo1->bitmap[i]=0;
		i++;
		}

		list_add(tabla_de_nodos.listaCapacidadNodos,nodo1);

		bloques_nodo* nodo2 = malloc(sizeof(bloques_nodo));;
		nodo2->idNodo=2;
		nodo2->bloquesTotales=20;
		nodo2->bloquesLibres=20;
		int j=0;
		while(j<20){
		nodo2->bitmap[j]=0;
		j++;
		}

		list_add(tabla_de_nodos.listaCapacidadNodos,nodo2);


		int resp = crearRegistroArchivoNodos(tabla_de_nodos);
		if(resp==0) printf("\nRegistro de Nodo cargado correctamente.\n");
		else printf("\nRegistro de Nodo no pudo ser cargado.\n");


	//	int cantNodos = list_size(tabla_de_nodos.listaNodos);
	//	t_list* lista_nodos;

		//lista_nodos = tablaNodosToNodos(tabla_de_nodos.listaNodos);
		//FIN EJEMPLO NODOS CARGADOS

}
*/

int showBitMap(t_list* Mapa_de_bits){

	int cuenta = 0,count = 0, id;
	printf("\n\n");
	bloques_nodo* mapa;
	while(cuenta<list_size(Mapa_de_bits)){

		mapa = list_get(Mapa_de_bits,cuenta);

		id = mapa->idNodo;
		printf("%d\n",id);
		while(count<mapa->bloquesTotales){
			printf("nodo:%d desplazamiento:%d, empty:%d\n",mapa->idNodo,count, mapa->bitmap[count]);
			count++;
		}
		count=0;
		cuenta++;
	}
	return(1);
}

int liberarBloqueBitMap(nodo, desplazamiento){

	/*Recibe un nodo y un bloque dentro del nodo
	* devuleve 1 si lo pone en 0
	* -1 si no puedo
	*/

	int count = 0;
	bloques_nodo* bloquesNodo;
	bloquesNodo = list_get(tabla_de_nodos.listaCapacidadNodos,count);
	int cantNodos=list_size(tabla_de_nodos.listaNodos);

	while(count<cantNodos){
		if(bloquesNodo->idNodo==nodo){
		bloquesNodo->bitmap[desplazamiento]=0;
		bloquesNodo->bloquesLibres=bloquesNodo->bloquesLibres+1;
		return(1);
		}
	count++;
	}

return(-1);
}

int ultimaCopia(int indiceArchivo,int parteArchivo){

	/*recibe indice del archivo en tabla de archivos y la partde del archivo
	 * devuelve 1 si tiene mas de 1 copia, -1 si no.
	 */

	int count = 0;
	int cantPartes;
	UbicacionBloquesArchivo2* ubicaciones;

	cantPartes = list_size(tabla_de_archivos[indiceArchivo].bloques);

	while(count<cantPartes){

		ubicaciones = list_get(tabla_de_archivos[indiceArchivo].ubicaciones,count);

		if(ubicaciones->parteDelArchivo == parteArchivo){
			break;
		}

		count++;
	}

	if(ubicaciones->nodo1 == -1 || ubicaciones->nodo2 == -1){
		return(-1);
	}

	return(1);
}

void * distribuirBloque(char* bloque, int indiceArchivo, int cantBloques){

	/*Recibe una lista de char* con bloques de texto (bloques) y una lista de bitMap (nodos)
	 * Devuelve una lista de ubicacionBloquesArchivo
	 */

	//RECORRO CADA ARCHIVO Y SE LO ASIGNO A DOS NODOS DISTINTOS

	int indiceBloque = cantBloques;
	tabla_de_archivos[indiceArchivo].ubicaciones = list_create();
	bloques_nodo* bitMapNodo1;
	bloques_nodo* bitMapNodo2;
	UbicacionBloquesArchivo2* ubicacionBloquesArchivo = malloc(sizeof(int)*6);
	int index1, indexList1, index2, indexList2;
	int desplazamiento1, desplazamiento2;

int asd;
	indexList1 = elegirNodo(asd);
	bitMapNodo1 = list_get(tabla_de_nodos.listaCapacidadNodos,indexList1);
	index1 = bitMapNodo1->idNodo;
	desplazamiento1 = buscarBloqueVacio(bitMapNodo1); //busca el vacio, devuelve eso y a su vez ya lo actualiza

	if(desplazamiento1==-1){
		logInfo("Nodo lleno."); //Si distribuye bien y checkea bien no deberia entrar aca
	}
	logInfo("Ubico la copia 1 del bloque %d, en el nodo %d, desplazamiento %d",indiceBloque, index1,desplazamiento1);
	list_remove(tabla_de_nodos.listaCapacidadNodos,indexList1);

	//printf("Nodo para guardar el bloque:%d Desplazamiento:%d \n", index1,desplazamiento1);

	indexList2 = elegirNodo(asd);
	bitMapNodo2 = list_get(tabla_de_nodos.listaCapacidadNodos,indexList2);
	index2 = bitMapNodo2->idNodo;
	desplazamiento2 = buscarBloqueVacio(bitMapNodo2); //lo devuleve y lo pone en ocupado

	if(desplazamiento2==-1){
		logInfo("Nodo lleno."); //Si distribuye bien y checkea bien no deberia entrar aca
	}
	logInfo("Ubico la copia 2 del bloque %d, en el nodo %d, desplazamiento %d",indiceBloque, index2,desplazamiento2);

	list_add(tabla_de_nodos.listaCapacidadNodos,bitMapNodo1);

	//cargo los datos de los bloques en tabla_de_archivos

	ubicacionBloquesArchivo->parteDelArchivo = indiceBloque;
	ubicacionBloquesArchivo->desplazamiento1 = desplazamiento1;
	ubicacionBloquesArchivo->nodo1 = index1;
	ubicacionBloquesArchivo->desplazamiento2 = desplazamiento2;
	ubicacionBloquesArchivo->nodo2 = index2;
	ubicacionBloquesArchivo->bytesOcupados = string_length(bloque);

	logInfo("tam:%d parteNum:%d\nNodo:%d, Desplazamiento:%d\nNodo:%d, Desplazamiento:%d",ubicacionBloquesArchivo->bytesOcupados,
			ubicacionBloquesArchivo->parteDelArchivo,ubicacionBloquesArchivo->nodo1,
			ubicacionBloquesArchivo->desplazamiento1,ubicacionBloquesArchivo->nodo2, ubicacionBloquesArchivo->desplazamiento2);

	list_add(tabla_de_archivos[indiceArchivo].ubicaciones,ubicacionBloquesArchivo);

}
