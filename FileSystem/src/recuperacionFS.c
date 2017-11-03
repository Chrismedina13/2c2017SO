/*
 * recuperacionFS.c
 *
 *  Created on: 22/10/2017
 *      Author: utnso
 */
#include "Headers/recuperacionFS.h"
#include "Headers/FileSystem.h"

void recuperacionFS(){

	recuperarTablaDeDirectorios();

	//loginfo("LEVANTANDO TABLA DE ARCHIVOS");
	//recuperarTablasDeArchivos();


	//loginfo("LEVANTANDO TABLA DE NODOS");
	//recuperarTablaNodos();
}


void recuperarTablaDeDirectorios(){

	    char renglon1[LIMITE];
	    char renglon2[LIMITE];
	    char renglon3[LIMITE];
	    FILE* fd;
	    fd = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/directorios.dat", "rt");
	    fgets(renglon1,LIMITE,fd);

	    fclose(fd);

	    puts(renglon1);
	    puts(renglon2);
}
