/*
 * estructurasFS.c
 *
 *  Created on: 30/9/2017
 *      Author: utnso
 */

#include "Headers/FileSystem.h"

int cargarDirectorios() {

  FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/directorios.txt", "r");
  if (!fp) perror("Error al abrir el Archivo de directorios");
  else printf("Archivo de directorios abierto");
  int count = 0;
  struct tabla_directorios tabla_de_directorios[100];

  while (count < 100 && !feof(fp)) {
  fscanf(fp, tabla_de_directorios[count].index, tabla_de_directorios[count].nombre,
		  	  tabla_de_directorios[count].padre);
  }
  logInfo("Tabla de Directorios cargada correctamente. \n");
  return 1;
}
