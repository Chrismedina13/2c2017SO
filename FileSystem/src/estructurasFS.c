/*
 * estructurasFS.c
 *
 *  Created on: 30/9/2017
 *      Author: utnso
 */

#include "Headers/FileSystem.h"

//manejo de directorios


//funciones

int cantidadDirectorios(){

	/*devuelve el index del ultimo directorio.
	 *
	 */

	int count = 1;

	  	while(1){
	  		if(!tabla_de_directorios[count].index) return(count);
	  		count++;
	  	}

};

int actualizarTablaDeDirectorios(){

	/*Actualiza el archivo directorios.dat
	 *
	 */

	int count = 0;
	int cantDir = cantidadDirectorios();

	FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/directorios.dat", "w");
	  if (!fp) {
		  perror("Error al abrir el Archivo de directorios");
		  return (-1);
	  }

	char* ruta;

	while(count <= cantDir){
		fprintf(fp, "%d /n", tabla_de_directorios[count].index);
		fprintf(fp, "%s /n", tabla_de_directorios[count].nombre);
		fprintf(fp, "%d /n", tabla_de_directorios[count].padre);

		ruta = string_from_format("yamafs/metadata/archivos/%d/%s", tabla_de_directorios[count].index, tabla_de_directorios[count].nombre);

	int status = mkdir(ruta, 0777);
		if (status == -1){
			//return(-1);
		}

		count++;
	}

	  return(1);

}

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

  while (count <= 99) {
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
  //actualizarTablaDeDirectorios();
  return (1);
}

int buscarIndice(){

	/* Find the smallest positive missing number in an array that contains
	 * all positive integers
	 */

	int i=0;

	while(i<100){
		tabla_de_directoriosAux[i].index = tabla_de_directorios[i].index;
		tabla_de_directoriosAux[i].padre = tabla_de_directorios[i].padre;
		strcpy(tabla_de_directoriosAux[i].nombre, tabla_de_directorios[i].nombre);
		i++;
	}

  int count;
  int cantDir = cantidadDirectorios();

  // Mark arr[i] as visited by making arr[arr[i] - 1] negative. Note that
  // 1 is subtracted because index start from 0 and positive numbers start from 1
  for(count = 0; count < cantDir; count++)
  {
    if(abs(tabla_de_directoriosAux[count].index) - 1 < cantDir && tabla_de_directoriosAux[abs(tabla_de_directoriosAux[count].index) - 1].index > 0)
    	tabla_de_directoriosAux[abs(tabla_de_directoriosAux[count].index) - 1].index = -tabla_de_directoriosAux[abs(tabla_de_directoriosAux[count].index) - 1].index;
  }

  // Return the first index value at which is positive
  for(count = 0; count < cantDir; count++)
    if (tabla_de_directoriosAux[count].index > 0)
      return count+1;  // 1 is added becuase indexes start from 0

  return cantDir+1;
}

int padreDirectorio(int index){

	/*Recibe index del directorio a buscar
	 *devuelve el index del padre. -2 si no lo encuentra.
	 */

	int count = 0;
	int cantDir = cantidadDirectorios();

	while (count <= cantDir) {
			if (tabla_de_directorios[count].index == index) {
				return(tabla_de_directorios[count].padre);
			}
			count++;
	}
	return(-2);


}

int existeDirectorio(char* nombre, int padre) {

	/* Recibe nombre del directorio a buscar y el index del padre
	 * devuelve el index si existe, -1 si no existe.
	 */

	int count = 0;
	int cantDir = cantidadDirectorios();

	while (count <= cantDir) {
		if ((strcmp(tabla_de_directorios[count].nombre,nombre) != 0) && tabla_de_directorios[count].padre == padre) {
			return(count);
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
	int indice;

	cantDir = cantidadDirectorios();
	existe = existeDirectorio(nombre, padre);

	if (cantDir<=100 && existe == -1){
	indice = buscarIndice();
	tabla_de_directorios[cantDir+1].index = indice;
	strcpy(tabla_de_directorios[cantDir+1].nombre, nombre);
	tabla_de_directorios[cantDir+1].padre = padre;

	}

	else {
		logInfo("Directorio ya existe o cantidad maxima de directorios posibles alcanzada. \n");
		return(-1);
	}

	actualizarTablaDeDirectorios();
	logInfo("Directorio creado correctamente. \n");
	return(1);

};

int eliminarDirectorio(int index){ //checkear si tiene archivos dentro

	/*Recibe el index de un directorio a borrar
	 *devuelve 1 si lo borra, -1 si no puede borrarlo (porque tiene cosas adentro).
	 */

	int count = 0;

	while(count <= 99){
		if(tabla_de_directorios[count].padre == index) return(-1); //si es padre de alguien no puede borrarlo
		//if (contieneArchivos()=1) return (-1);
		count++;
	}

	tabla_de_directorios[index].index = -2;
	strcpy(tabla_de_directorios[index].nombre,"deleted");
	tabla_de_directorios[index].padre = -2;

	count = 0;

	while(count <= 99){
		if(tabla_de_directorios[count].index == -2){
			tabla_de_directorios[count].index = tabla_de_directorios[count+1].index;//no deberiamos cambiar el index sino todos los que lo tengan como padre hay que cambiarlos tambien
			strcpy(tabla_de_directorios[count].nombre, tabla_de_directorios[count+1].nombre);
			tabla_de_directorios[count].padre = tabla_de_directorios[count+1].padre;
			tabla_de_directorios[count+1].index = -2;
			strcpy(tabla_de_directorios[count+1].nombre,"being deleted");
			tabla_de_directorios[count+1].padre = -2;
		}
		count++;
	}

	actualizarTablaDeDirectorios();
	return(1);
}

int mostrarTablaDeDirectorios(){

	/*Muestra con printf la comp del struct tabla_de_directorios
	 *
	 */

	int count = 0;

	printf("\n indice %d: %d \n", count, tabla_de_directorios[0].index);
	printf("nombre %d: %s \n", count, tabla_de_directorios[0].nombre);
	printf("padre %d: %d \n", count, tabla_de_directorios[0].padre);

	count++;

	while(count <= 99 && tabla_de_directorios[count].index != 0 ){
	   printf("indice %d: %d \n", count, tabla_de_directorios[count].index);
	   printf("nombre %d: %s \n", count, tabla_de_directorios[count].nombre);
	   printf("padre %d: %d \n", count, tabla_de_directorios[count].padre);
	   count++;
	}
	return(1);
}

int cambiarNombreDirectorio(int index, char* nombre){

	/*Cambia el nombre de un directorio, siempre y cuando este exista.
	 *recibe el index y nombre.
	 *devuelve 1 si lo puede cambiar, -1 si no existe o no puede cambiarlo.
	 */
	int padre = padreDirectorio(index);

	if(existeDirectorio(nombre,padre) != 1)return(-1);
	strcpy(tabla_de_directorios[index].nombre, nombre);
	actualizarTablaDeDirectorios();
	return(1);

}

int moverDirectorio(int index, int padre, int padreNew){

	tabla_de_directorios[index].padre = padreNew;
	actualizarTablaDeDirectorios();
	return(1);

}

int make_directory(const char* ruta) {

    mkdir(ruta, 777);
    return(1);
}

//manejo de archivos

//funciones


int crearRegistroArchivo(char* ruta, char* nombre, char tipo, int directorio){

	int tamanioBloque = 1024*1024;
	int indiceArchivo = 0;
	int tamArchivo;
	int maxArchivos = MAX;
	char* rutaLocal;
	UbicacionBloquesArchivo* bloquesPtr;
	tabla_archivos *archivosPtr = malloc (maxArchivos * (sizeof (tabla_archivos)));

	//abro mi archivo y cargo la info que necesito en el puntero a archivos

	FILE * fp = fopen(ruta, "r");
	if (!fp) {
	  perror("Error al abrir el Archivo");
	  return (-1);
	}

	tamArchivo = tamanioArchivo(fp);

	archivosPtr[indiceArchivo].tamanio = tamArchivo;
	archivosPtr[indiceArchivo].tipo = tipo;
	archivosPtr[indiceArchivo].directorio = directorio;
	archivosPtr[indiceArchivo].bloques = bloquesPtr;

	//creo mi registro de archivo local

	rutaLocal = string_from_format("yamafs/metadata/archivos/%d/%s", directorio, nombre);

	FILE * fp2 = fopen(rutaLocal, "w");
	if (!fp2){
	  perror("Error al crear el registro de Archivo");
	  return (-1);
	}

	fscanf(fp2, "TAMANIO=%d\n TIPO=%c\n DIRECTORIO=%d\n", &archivosPtr[indiceArchivo].tamanio, &archivosPtr[indiceArchivo].tipo, &archivosPtr[indiceArchivo].directorio); //carga la info del archivo

	int cantBloques;
	int count = 0;
	int copia = 0;
	int infoNodoCopia;
	int infoBloqueCopia;
	int infoBytesOcupados;
	//UbicacionBloquesArchivo* bloques;

	if ( tamArchivo % tamanioBloque  == 0 )
	{
	   cantBloques = (tamArchivo/tamanioBloque);
	}
	else cantBloques =  abs(tamArchivo/tamanioBloque)+1;

	while (count < cantBloques){
		while(copia <= 1){
			infoNodoCopia = archivosPtr[indiceArchivo].bloques[count].ubicacionCopia1.nodo;
			infoBloqueCopia = archivosPtr[indiceArchivo].bloques[count].ubicacionCopia1.bloqueDelNodoDeLaCopia;

			fscanf(fp2, "BLOQUE%dCOPIA%d=[Nodo%d, %d]\n", count, copia, infoNodoCopia, infoBloqueCopia);
			copia++;
		}

		infoBytesOcupados = archivosPtr[indiceArchivo].bloques[count].bytesOcupados;

		fscanf(fp2, "BLOQUE%dBYTES=%d\n", count, infoBytesOcupados);

		copia = 0;
		count++;
	}
	close(fp);
	close(fp2);
	return(1);

}

char* generarRutaLocal(char* nombre, int directorio){

	char* rutaLocal;

	strcpy(rutaLocal, string_from_format("yamafs/metadata/archivos/%d/%s", nombre, directorio));
	return(rutaLocal);

}

int tamanioArchivo(int fp){

	int tam;
	fseek(fp, 0L, SEEK_END);
	tam = ftell(fp);
	return(tam);

}

int cambiarNombreArchivo(char* rutaLocal, char* nombreNew, int directorio){

	//Cambia el nombre de un archivo, siempre y cuando este exista.
	//recibe el nombre del archivo y el index de su directorio.
	//devuleve 1 si lo puede cambiar, -1 si no existe o no puede cambiarlo.


	char* newRutaLocal = generarRutaLocal(nombreNew, directorio);

	if(strcmp(newRutaLocal,"error") == 0)return(-1);
	rename(rutaLocal, newRutaLocal);
	return(1);

}

int eliminarArchivo(char* rutaLocal){

	remove(rutaLocal);
	return (1);
}

int moverArchivo(char* rutaLocal, char* nombre, int directorio){

	/*Borra el registro del archivo en el directorio actual y lo mueve a un nuevo directorio
	 * retorna 1 si lo puede hacer, -1 si falla.
	 */


	char caracter;
	char* newRutaLocal = generarRutaLocal(nombre, directorio);

	FILE * fp = fopen(rutaLocal, "r");
		if (!fp) {
		  perror("Error al localizar el Archivo");
		  return (-1);
		}

	FILE * newfp = fopen(newRutaLocal, "w");
		if (!fp) {
			  perror("Error al crear el Archivo");
			  return (-1);
			}


	while(!feof(fp)){
		caracter = fgetc(fp);
		fputc(caracter, newfp);
	}


	fclose(fp);
	fclose(newfp);

	eliminarArchivo(rutaLocal);

	return(1);

}

int mostrarArchivo(char* rutaLocal){

	char caracter;

	FILE * fp = fopen(rutaLocal, "r");
			if (!fp) {
			  perror("Error al abrir el Archivo");
			  return (-1);
			}

	while(!feof(fp)){
		caracter = fgetc(fp);
		printf("%c", caracter);
	}

	fclose(fp);

	return(1);

}

//int archivosDirectorio(int directorio){




