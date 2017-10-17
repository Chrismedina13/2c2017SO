/*
 * estructurasFS.c
 *
 *  Created on: 30/9/2017
 *      Author: utnso
 */

#include "Headers/FileSystem.h"


int cargarDirectorios() {

	/*Carga los directorios de directorios.dat en una estructura tipo tabla_directorios.
	 *La estructura esta declarada global en FileSystem.h
	 */

  FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/directorios.dat", "r");
  if (!fp) {
	  perror("Error al abrir el Archivo de directorios");
	  return (-1);
  }

  int count = 0;

  while (count < 99) {
	  if(feof(fp)){
		  break;
	  }
	  fscanf(fp, "%d %s %d", &tabla_de_directorios[count].index, tabla_de_directorios[count].nombre,
			  &tabla_de_directorios[count].padre);

	  /*Prueba si se carga correctamente
	   printf("indice cargado: %d \n", tabla_de_directorios[count].index);
	   printf("nombre cargado: %s \n", tabla_de_directorios[count].nombre);
	   printf("padre cargado: %d \n", tabla_de_directorios[count].padre);
	   */

	  count++;
  }
  fclose(fp);
  logInfo("Tabla de Directorios cargada correctamente. \n");
  return (1);
}

int cantidadDirectorios(){

	/*devuelve el index del ultimo directorio.
	 *
	 */

	int count = 0;

	while(1){
		if(tabla_de_directorios[count].index == count) count++;
		else return(count-1);
	}

};

int existeDirectorio(char* nombre, int padre) { //no esta funcionando como deberia

	/* Recibe nombre del directorio a buscar y el index del padre
	 * devuelve 1 si existe, -1 si no existe.
	 */

	int count = 0;
	int cantDir = cantidadDirectorios();

	while (count <= cantDir) {
		if (tabla_de_directorios[count].nombre == nombre && tabla_de_directorios[count].padre == padre) { //hay un problema con el tipo de datos en padre
			return(1);
		}
		count++;
	}
	return(-1);
};

int crearDirectorio(char* nombre, int padre) {

	/* Recibe nombre del directorio a crear y el index del padre
	 * devuelve 1 si puede crearlo, -1 si ya existe o la cantidad de directorios no se lo permite.
	 */

	int cantDir;
	int existe;

	cantDir = cantidadDirectorios();
	existe = existeDirectorio(nombre, padre);

	if (cantDir<99 && existe == -1){
	strcpy(tabla_de_directorios[cantDir+1].index, cantDir+1);
	strcpy(tabla_de_directorios[cantDir+1].nombre, nombre);
	strcpy(tabla_de_directorios[cantDir+1].padre, padre);
	}

	else {
		logInfo("Directorio ya existe o cantidad maxima de directorios posibles alcanzada. \n");
		return(-1);
	}

	logInfo("Directorio creado correctamente. \n");
	return(1);

};
