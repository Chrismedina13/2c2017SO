/*
 * prubeas.c
 *
 *  Created on: 11/10/2017
 *      Author: utnso
 */

#include "Headers/pruebas.h"

int pasarAMemoriaBinario(const char * rutaDelArchivo){

		ArchivoEnMemoria archivoEnMemoria = damePunteroYTamanio(rutaDelArchivo);
		char* p = archivoEnMemoria.puntero;
		struct stat sb = archivoEnMemoria.sb;

		//MOSTRAMOS EL ARCHIVO COMPLETO
        //printf("%s\n",p);

        //MOSTRAMOS EL TAMAÑO DEL ARCHIVO
        printf("El tamaño del archivo es: %d\n",sb.st_size);

        //MOSTRAMOS LA CANTIDAD DE BLOQUES DE 1KB QUE NECESITA
        int cantidadBloques = cantidadDeBloques(sb.st_size);
        printf("Cantidad de bloques: %d\n\n",cantidadBloques);

        //divido el archivo en archivos de 1 KB
        t_list* lista = dividirArchivoBinario(p,cantidadBloques);

        //imprimo por pantalla uno de archivos de la lista para probar
        int salida = imprimirArchivo(lista);
        if(salida == 0) printf("Archivo impreso correctamente");
   		else printf("Archivo no impreso");


        //LIBERAMOS EL ESPACIO DE MEMORIA QUE OCUPABA EL ARCHIVO
        if (munmap (p, sb.st_size) == -1) {
                printf("Error al cerrar la proyeccion \n");
                return -1;
        }

        return 0;

}

int pasarAMemoriaTxt(const char * rutaDelArchivo){

		ArchivoEnMemoria archivoEnMemoria = damePunteroYTamanio(rutaDelArchivo);
		char* p = archivoEnMemoria.puntero;
		struct stat sb = archivoEnMemoria.sb;

		t_list* lista = dividirArchivoTxt(p);

		//imprimo por pantalla uno de archivos de la lista para probar
		int salida = imprimirArchivo(lista);
		if(salida == 0) printf("Archivo impreso correctamente");
		else printf("Archivo no impreso");

        //LIBERAMOS EL ESPACIO DE MEMORIA QUE OCUPABA EL ARCHIVO
        if (munmap (p, sb.st_size) == -1) {
                printf("Error al cerrar la proyeccion \n");
                return -1;
        }

        return 0;

}


// Devuelve una lista de archivos, los cuales se obtuvieron de dividir el archivo principal
t_list* dividirArchivoTxt(char* p){

	char** parrafos =  string_split(p, "\n");
	char* contenidoDelBloque = string_new(); //SE VA A IR GUARDANDO LOS PARRAFOS EN EL BLOQUE HASTA QUE PASE EL ESPACIO REQUERIDO
	int numeroDeArchivo = 0; //A LA HORA DE GUARDAR EN UN ARCHIVO SE INCREMENTA PARA EL SIGUIENTE ARCHIVO
	int i = 0; //NUMERO DE PARRAFO
	int k; // NUMERO DE PARRAFO SIGUIENTE (k = i+1);
	int KB = 1024; //TAMAÑO MAXIMO DEL BLOQUE
	int longitudParrafo; //PARA VER EL TAMAÑO DEL PARRAFO
	int totalEspacioLlenado = 0; //PARA VERIFICAR QUE AL AGREGAR LOS PARRAFOS NO SUPEREN EL ESPACIO REQUERIDO
	t_list* lista = list_create();
	char* parrafo;
	while(parrafos[i]!='\0'){
		k=i+1;
		parrafo = parrafos[i];
		longitudParrafo = string_length(parrafo);
		totalEspacioLlenado += longitudParrafo; //sirve para verificar que no sobrepase el KB
		if(totalEspacioLlenado<=KB && parrafos[k]!='\0'){ //verifico que no sobrepase el KB y que no sea el fin del archivo.
			string_append(&contenidoDelBloque, parrafo);//agrego el parrafo al bloque
			string_append(&contenidoDelBloque,"\n");
			i++;
		}
		else{
			if(parrafos[k]=='\0') {
				string_append(&contenidoDelBloque, parrafo); //agrego el parrafo faltante al bloque
				i++; //PARA QUE EL WHILE TERMINE
			}
			FILE* fd;
			char *nombreArchivo = string_new();
			char* numeroArchivo = string_itoa(numeroDeArchivo); //paso el numero de archivo de int a char*
			string_append(&nombreArchivo, "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/bloque");
			string_append(&nombreArchivo, numeroArchivo); //Le agrego el numero de archivo al nombre
			string_append(&nombreArchivo, ".txt"); //le agrego la extencion de archivo al nombre
			fd = fopen(nombreArchivo,"w");
			if (fd==NULL) {
				printf("Error al abrir el archivo.");
			}
			string_append(&contenidoDelBloque,"\n"); //le agrego el salto de linea final
			fputs(contenidoDelBloque,fd);
			list_add(lista,nombreArchivo);
			fclose(fd);
			*contenidoDelBloque = '\0'; //vacio el contenido del bloque para poder volver a meterle parrafos
			totalEspacioLlenado=0; //reseteo el espacio que ocupan los parrafos
			numeroDeArchivo++; //sirve para que me escriba otro archivo.
		}
	}

    return lista;
}

// Devuelve una lista de archivos, los cuales se obtuvieron de dividir el archivo principal
t_list* dividirArchivoBinario(char*puntero,int cantidadDeBloques){
	int i = 0,KB = 1024;
	t_list* lista = list_create();
	for(i=0;i<cantidadDeBloques;i++){
		char *nombreArchivo = string_new();
		FILE* fd;
		char* numeroDeArchivo = string_itoa(i);
		string_append(&nombreArchivo, "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/bloque");
		string_append(&nombreArchivo, numeroDeArchivo); //Le agrego el numero de archivo al nombre
		string_append(&nombreArchivo, ".txt"); //le agrego la extencion de archivo al nombre
		fd = fopen(nombreArchivo,"w");
		if (fd==NULL) {
			printf("Error al abrir el archivo.");

		}
		char* bloque = string_substring(puntero, i*KB, KB);
		fputs( bloque, fd );
		list_add(lista,nombreArchivo);
		fclose(fd);
	}
	return lista;
}

int imprimirArchivo(t_list* lista){
	char* ruta =(char*) list_get(lista,0);
	char* p;
	struct stat sb;
	int fd;

	fd = open (ruta, O_RDONLY);
	if (fd == -1) {
			printf("Error al abrir archivo\n");
			return -1;
	}

	if (fstat (fd, &sb) == -1) {
			printf("Error al hacer stat\n");
			return -1;
	}

	if (!S_ISREG (sb.st_mode)) {
			printf ("No es un archivo regular\n");
			return -1;
	}
	p = (char *)mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
			printf("Fallo el mapeo\n");
			return -1;
	}

	if (close (fd) == -1) {
			printf("Error al cerrar el archivo\n");
			return -1;
	}


	//MOSTRAMOS EL ARCHIVO COMPLETO
	printf("%s\n",p);

	//LIBERAMOS EL ESPACIO DE MEMORIA QUE OCUPABA EL ARCHIVO
	if (munmap (p, sb.st_size) == -1) {
			printf("Error al cerrar la proyeccion \n");
			return -1;
	}
	return 0;
}

ArchivoEnMemoria damePunteroYTamanio(const char* rutaArchivo){
	ArchivoEnMemoria archivoEnMemoria;

	struct stat sb;
	off_t len;
	char* p;
	int fd;

	fd = open (rutaArchivo, O_RDONLY);
	if (fd == -1) {
			printf("Error al abrir archivo\n");
			exit(-1);
	}

	if (fstat (fd, &sb) == -1) {
			printf("Error al hacer stat\n");
			exit(-1);
	}

	if (!S_ISREG (sb.st_mode)) {
			printf ("No es un archivo regular\n");
			exit(-1);
	}

	p = (char *)mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

	if (p == MAP_FAILED) {
			printf("Fallo el mapeo\n");
			exit(-1);
	}

	if (close (fd) == -1) {
			printf("Error al cerrar el archivo\n");
			exit(-1);
	}

	archivoEnMemoria.puntero = p;
	archivoEnMemoria.sb = sb;

	return archivoEnMemoria;
}

// Devuelve la cantidad de bloques que se pueden hacer con 1KB
int cantidadDeBloques(int tamanio){
	int KB = 1024;
	int cantidad;
	if(tamanio%KB == 0) cantidad = tamanio/KB;
	else cantidad = (tamanio/KB)+1;
	return cantidad;
}


