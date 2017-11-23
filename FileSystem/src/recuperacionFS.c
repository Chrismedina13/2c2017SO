/*
 * recuperacionFS.c
 *
 *  Created on: 22/10/2017
 *      Author: utnso
 */
#include "Headers/recuperacionFS.h"
#include "Headers/FileSystem.h"

/*
 * 1 cargo los directorios desde directorios.dat (cargarDirectorios)
 * 2 recupero la Tabla de Nodos (recuperarTablaDeNodos)
 * 3 recupero la Tabla de Archivos (recuperarTablaDeArchivos)
 * 4 me fijo si tengo una copia completa de cada archivo en mis nodos existentes
 * 5a tengo todos los archivos con 2 copias, estado estable (fin)
 * 5b tengo todos los archivos pero no tienen 2 copias (en los nodos existentes)
 * 6 hago las copias que me falten, estado estable (fin)
 */

int recuperacionFileSystem(){

}

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
			nodo->idNodo = atoi(idNodoS);
			nodo->bloquesTotales = atoi(nodototalS);
			nodo->bloquesLibres = atoi(nodolibreS);

			list_add(&(tabla_de_nodos.listaCapacidadNodos),nodo);

		}
		return 0;
}

int recuperarTablaDeArchivos(){

	/*Usa los registros de archivos en /home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/yamafs/metadata/archivos/indice
	 * para cargar la tabla de archivos
	 *
	 * si puede abrir todos los direcotiros y cargar los datos de los archivos en la tabla, devuelve 1.
	 * si no puede abrir alguno de los directorios, devuelve -1.
	 * si no puede abrir alguno de los registros de archivo, devuelve -1.
	 */

    DIR* FD;
    struct dirent* in_file;
    FILE    *entry_file;
    char    buffer[BUFSIZ];

    /* Scanning the in directory */
    int cantDir =  cantidadDirectorios();
    char* directorio;
    int indice;
    strcpy(directorio,"/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/yamafs/metadata/archivos/");
    int count = 3;
    t_list* ubicaciones;
	list_create(ubicaciones);

	UbicacionBloquesArchivo* ubicacion;

    while(count<cantDir){

    	indice = tabla_de_directorios[count].index;
    	string_append_with_format(&directorio, "%s", indice);
    	if(NULL == (FD = opendir(directorio))) {
            fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));

            return -1;
        }

    	}

		while ((in_file = readdir(FD))){

			/* On linux/Unix we don't want current and parent directories
			 * On windows machine too, thanks Greg Hewgill
			 *
			 * dato
			 */

			if (!strcmp (in_file->d_name, "."))
				continue;
			if (!strcmp (in_file->d_name, ".."))
				continue;

			entry_file = fopen(in_file->d_name, "r");
			if (entry_file == NULL)
			{
				fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));

				return -1;
			}

			/* Doing some struf with entry_file : */
			/* 	TAMANIO=2056360 ok
				TIPO=texto ok
				BLOQUE0COPIA0=[Nodo1, 0]
				BLOQUE0COPIA1=[Nodo2, 0]
				BLOQUE0BYTES=1079829
				BLOQUE1COPIA0=[Nodo2, 1]
				BLOQUE1COPIA1=[Nodo1, 1]
				BLOQUE1BYTES=953658 */

			//nombre

			tabla_de_archivos[count].nombre = pathToFile(directorio);

			//directorio

			tabla_de_archivos[count].directorio = indice;

			//tamanio

			char* linea = fgets(buffer, BUFSIZ, entry_file);
			int tamanio =  atoi(string_substring_from(linea,8));

			tabla_de_archivos[count].tamanio = tamanio;

			//tipo

			linea = fgets(buffer, BUFSIZ, entry_file);
			char* tipo;
			tipo = string_substring_from(linea,5);

			strcpy(tabla_de_archivos[count].tipo,tipo);

			//partes del archivo y ubicaciones

			while ((linea =fgets(buffer, BUFSIZ, entry_file)) != NULL){

				int copia = obtenerCopia(linea); //si es linea de bytes, pone a copia en 3

				while(copia<2){

					//parte del archivo

					int bloque = obtenerBloque(linea);

					ubicacion->parteDelArchivo=bloque;

					//ubicacion copia 1


					int nodo = obtenerNodo(linea);
					int desplazamiento = obtenerDesplazamiento(linea);

					if(copia == 0){
						ubicacion->ubicacionCopia1.nodo= nodo;
						ubicacion->ubicacionCopia1.desplazamiento= desplazamiento;
					}
					if(copia ==1){
						ubicacion->ubicacionCopia2.nodo= nodo;
						ubicacion->ubicacionCopia2.desplazamiento= desplazamiento;
					}

					linea=fgets(buffer, BUFSIZ, entry_file);


				}

				//seteo tamanio en bytes

				int bytes = obtenerBytes(linea);

				ubicacion->bytesOcupados= bytes;

			}

			list_add(ubicaciones,ubicacion);

			/* When you finish with the file, close it */
			fclose(entry_file);
		}

	return 0;
}

int obtenerCopia(char* linea){

}

int obtenerBloque(char* linea){

}

int obtenerNodo(char* linea){

}

int obtenerDesplazamiento(char* linea){

}

int obtenerBytes(char* linea){

}
