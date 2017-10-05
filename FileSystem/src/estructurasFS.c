/*
 * estructurasFS.c
 *
 *  Created on: 30/9/2017
 *      Author: utnso
 */

#include "Headers/FileSystem.h"

int cargarDirectorios() {

  FILE * fp = fopen("metadata/directorios.dat", "r");
  int count = 0;
  struct tabla_directorios tabla_de_directorios[100];

  while (count < 100 && !feof(fp)) {
  fscanf(fp, tabla_de_directorios[count].index, tabla_de_directorios[count].nombre, tabla_de_directorios[count].padre);
  }
  return 1;
}
