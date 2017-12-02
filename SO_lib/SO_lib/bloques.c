/*
 * bloques.c
 *
 *  Created on: 2/12/2017
 *      Author: utnso
 */

#include "bloques.h"
const int MB = 1024*1024;


char * get_bloque(int nro_bloque){

	/*Recibe un numero de bloque y devuelve un char* con su contenido
	 *
	 */

	const char * rutaDelArchivo= "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/data.bin";

	char* fp = obtenerPuntero(rutaDelArchivo);

	int tamanio = strlen(fp);
	char* bloque = string_substring(fp, nro_bloque* MB, MB);

	if (munmap (fp, tamanio) == -1) {
			printf("Error al cerrar la proyeccion \n");
	}

	/*
	FILE* fd;
	fd = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/nombre","w");
	if (fd==NULL){
		printf("Error al abrir el archivo.");
	}

	fputs( bloque, fd );

	*/
	return(bloque);


}

int set_bloque(char * contenido, int nro_bloque){

	const char * rutaDelArchivo= "/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/data.bin";

	FILE * fp = fopen(rutaDelArchivo, "r+");
	if (!fp) {
	  perror("Error al abrir el Archivo");
	   return(0);
	}

	if(fseek(fp, nro_bloque*MB, SEEK_SET)==0){
		logInfo("Me ubico en el bloque %i", nro_bloque);

		fwrite(contenido, sizeof(char),mystrlen(contenido),fp);

		fclose(fp);
		return(0);

	}else{
   logInfo("Fallo set bloque en el bloque %i", nro_bloque);
   fclose(fp);
	return(1);
	}

}




int mystrlen(char *s)
{
    int i =0;
    while (*s++) i++;
    return i;
}

int tamanioArchivo(int fp){

	int tam;
	fseek(fp, 0L, SEEK_END);
	tam = ftell(fp);
	return(tam);

}
