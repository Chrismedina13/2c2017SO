/*
 * prubeas.c
 *
 *  Created on: 11/10/2017
 *      Author: utnso
 */

#include "Headers/pruebas.h"

int pruebammap (){

	void* file2;
	   FILE * fp;
	   fp = fopen ("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/FileSystem/archivoprueba.txt", "r");

	   while(!feof(fp)){

	   fputc(fgetc(fp), stdout);
	   }
	   file2 = mmap (NULL, sizeof(fp), PROT_READ, MAP_PRIVATE, fp,0);

	   printf("%s", file2);

	   fclose(fp);

	   return (0);

}
