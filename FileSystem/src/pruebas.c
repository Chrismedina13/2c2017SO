/*
 * prubeas.c
 *
 *  Created on: 11/10/2017
 *      Author: utnso
 */

#include "Headers/pruebas.h"

int pasarAMemoria(const char * rutaDelArchivo){

        struct stat sb;
        off_t len;
        char* p;
        int fd;

        fd = open (rutaDelArchivo, O_RDONLY);
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
        //printf("%s\n",p);

        //MOSTRAMOS EL TAMAÑO DEL ARCHIVO
        printf("El tamaño del archivo es: %d\n",sb.st_size);

        //MOSTRAMOS LA CANTIDAD DE BLOQUES DE 1KB QUE NECESITA
        int cantidadBloques = cantidadDeBloques(sb.st_size);
        printf("Cantidad de bloques: %d\n\n",cantidadBloques);

        //PROBAMOS MOSTRAR UNO DE LOS BLOQUES
        mostrarBloque(p,0,cantidadBloques);

        //LIBERAMOS EL ESPACIO DE MEMORIA QUE OCUPABA EL ARCHIVO
        if (munmap (p, sb.st_size) == -1) {
                printf("Error al cerrar la proyeccion \n");
                return -1;
        }

        return 0;

}

// Devuelve la cantidad de bloques que se pueden hacer con 1KB
int cantidadDeBloques(int tamanio){
	int KB = 1024;
	int cantidad;
	if(tamanio%KB == 0) cantidad = tamanio/KB;
	else cantidad = (tamanio/KB)+1;
	return cantidad;
}

void mostrarBloque(char*puntero,int numeroDeBloque,int cantidadDeBloques){
	int KB = 1024;
	if(cantidadDeBloques <= numeroDeBloque) printf("Error, el numero de bloque supera a la cantidad total\n\n");
	else{
		char* bloque = string_substring(puntero, numeroDeBloque*KB, KB);
		printf("%s\n\n",bloque);
	}
}
