/*
 * estructurasFS.c
 *
 *  Created on: 30/9/2017
 *      Author: utnso
 */

#include "Headers/FileSystem.h"

//manejo de directorios


//funciones

int pathToIndex(char* path){

	/*Recibe un path y te devuelve el index del directorio.
	 *Devuleve el index del directorio, -1 si el directorio no existe.
	 */

	int tamanio;
	char* directorioString;
	int directorio;

	if(string_ends_with(path, "1") || string_ends_with(path, "2") || string_ends_with(path, "3") || string_ends_with(path, "4") || string_ends_with(path, "5") ||
			string_ends_with(path, "6") || string_ends_with(path, "7") || string_ends_with(path, "8") || string_ends_with(path, "9") || string_ends_with(path, "0")){

		tamanio = string_length(path);
		directorioString = string_substring(path, 26, tamanio - 26);
		tamanio = string_length(directorioString);
		directorioString = string_substring(directorioString, 0, tamanio -1);
		directorio = atoi(directorioString);
	}

	if(string_ends_with(path, ".txt") || string_ends_with(path, ".bin") || string_ends_with(path, ".csv")){

		char** secciones = string_split(path, "/");
		directorio = atoi(secciones[7]);

	}
	return(directorio);
}

int cantidadDirectorios(){ // en desuso, variable global cantDir

	/*devuelve el index del ultimo directorio.
	 *
	 */

	int count = 0;

	  	while(1){
	  		if(tabla_de_directorios[count].index) return(count);
	  		count++;
	  	}

};

int actualizarTablaDeDirectorios(){

	/*Actualiza el archivo directorios.dat
	 *
	 */

	int count = 0;
	char* ruta;

	FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/directorios.dat", "w+");
	  if (!fp) {
		  perror("Error al abrir el Archivo de directorios");
		  return (-1);
	  }

	while(count <= cantDir){

		if(tabla_de_directorios[count].index == 0 && tabla_de_directorios[count].padre == 0)break;

		fprintf(fp, "%d \n", tabla_de_directorios[count].index);
		fprintf(fp, "%s \n", tabla_de_directorios[count].nombre);
		fprintf(fp, "%d \n", tabla_de_directorios[count].padre);

		if(count >= 3){

			ruta = string_from_format("yamafs/metadata/archivos/%d", tabla_de_directorios[count].index);
			mkdir(ruta,0777);
		}

		count++;
	}

	  fclose(fp);
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
	  if(!strcmp(tabla_de_directorios[count].nombre,""))break;
	  //Prueba si se carga correctamente
	   printf("indice cargado: %d \n", tabla_de_directorios[count].index);
	   printf("nombre cargado: %s \n", tabla_de_directorios[count].nombre);
	   printf("padre cargado: %d \n", tabla_de_directorios[count].padre);
	   //

	  count++;
  }
  cantDir = count;
  fclose(fp);
  logInfo("Tabla de Directorios cargada correctamente. \n");
  actualizarTablaDeDirectorios();
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
	if(padre == -2)return(-1);
	while (count <= cantDir) {
		if ((strcmp(tabla_de_directorios[count].nombre,nombre) == 0) && tabla_de_directorios[count].padre == padre) {
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

	int existe;
	int indice;

	existe = existeDirectorio(nombre, padre);

	if (cantDir<=100 && existe == -1){
	indice = buscarIndice();
	tabla_de_directorios[cantDir].index = indice;
	strcpy(tabla_de_directorios[cantDir].nombre, nombre);
	tabla_de_directorios[cantDir].padre = padre;
	cantDir++;
	actualizarTablaDeDirectorios();
	}

	else {
		logInfo("Directorio ya existe o cantidad maxima de directorios posibles alcanzada. \n");
		return(-1);
	}

	logInfo("Directorio creado correctamente. \n");
	return(1);

};

int contieneArchivos(int index){

	/*Se fija si un directorio contiene archivos (files)
	 * Si contiene archivos devuelve 1, si esta vacio -1.
	 */
	char* ruta;
	ruta = string_from_format("yamafs/metadata/archivos/%d", index);
	  int n = 0;
	  struct dirent *d;
	  DIR *dir = opendir(ruta);
	  if (dir == NULL) //Not a directory or doesn't exist
		return 1;
	  while ((d = readdir(dir)) != NULL) {
		if(++n > 2)
		  break;
	  }
	  closedir(dir);
	  if (n <= 2) //Directory Empty
		return -1;
	  else
		return 1;

}

int eliminarDirectorio(int index){ //se puede hacer con una funcion ya hecha, rehacer

	/*Recibe el index de un directorio a borrar
	 *devuelve 1 si lo borra, -1 si no puede borrarlo (porque tiene cosas adentro).
	 */

	int count = 0;
	char* ruta;

	while(count <= 99){
		if(tabla_de_directorios[count].padre == index || contieneArchivos(index) == 1) return(-1); //si es padre de alguien no puede borrarlo
		//if (contieneArchivos()=1) return (-1);
		count++;
	}

	ruta = string_from_format("yamafs/metadata/archivos/%d", index);
	rmdir(ruta);

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

	cantDir--;
	actualizarTablaDeDirectorios();
	return(1);
}

int mostrarTablaDeDirectorios(){

	/*Muestra con printf la comp del struct tabla_de_directorios
	 *
	 */

	int count = 0;

	printf("\n indice %d: %d \n", count, tabla_de_directorios[count].index);
	printf("nombre %d: %s \n", count, tabla_de_directorios[count].nombre);
	printf("padre %d: %d \n", count, tabla_de_directorios[count].padre);

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

	if(existeDirectorio(nombre,padre) == -1){
		strcpy(tabla_de_directorios[index].nombre, nombre);
		actualizarTablaDeDirectorios();
		return(1);
	}
	else {
		return(1);
	}

}

int moverDirectorio(int index, int padreNew){

	tabla_de_directorios[index].padre = padreNew;
	actualizarTablaDeDirectorios();
	return(1);

}

int make_directory(const char* ruta) {

    mkdir(ruta, 777);
    return(1);
}

int cargarCasoBaseDirectorio(){

	FILE * fp = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/directorios.dat", "w");
	if (!fp) {
		perror("Error al abrir el Archivo de directorios");
		return (-1);
	}

	FILE * fp2 = fopen("/home/utnso/tp-2017-2c-s1st3m4s_0p3r4t1v0s/directorios2.dat", "r");
	if (!fp2) {
		perror("Error al abrir el Archivo de directorios2");
		return (-1);
	}

	int count = 0;

	while (feof(fp2)){

		fscanf(fp2, "%d %s %d", &tabla_de_directorios[count].index, tabla_de_directorios[count].nombre,
		  &tabla_de_directorios[count].padre);
		if(!strcmp(tabla_de_directorios[count].nombre,""))break;

		count++;
	}
	cantDir = count;
	fclose(fp);
	fclose(fp2);
	logInfo("Tabla de Directorios base cargada correctamente. \n");
	actualizarTablaDeDirectorios();
	return (1);


}

//manejo de archivos

//funciones

int newArchivo(){

	/*Devuelve el index del primer elemento vacio del vector
	 *
	 */

	int count = 0;
	while(count<100){

		if(strcmp(tabla_de_archivos[count].nombre,"deleted")==0){
			return(count);
		}
		if(tabla_de_archivos[count].directorio>2 && tabla_de_archivos[count].directorio<99){
			count++;
		}
		else return(count);
	}

	return(-1);

}

int crearRegistroArchivo(char* ruta, char* rutaLocal,t_list* ubicaciones, int indiceArchivo){

	int tamanioBloque = 1024*1024;
	int tamArchivo;
	//int maxArchivos = MAX;
	//UbicacionBloquesArchivo* bloquesPtr;
	//tabla_archivos *archivosPtr = malloc (maxArchivos * (sizeof (tabla_archivos)));

	//abro mi archivo y cargo la info que necesito en el puntero a archivos

	FILE * fp = fopen(ruta, "r");
	if (!fp) {
	  perror("Error al abrir el Archivo");
	  return (-1);
	}

	tamArchivo = tamanioArchivo(fp);

	char* tipo = pathToType(ruta);

	//termina de cargar el vector de archivos

	char* nombre = pathToFile(rutaLocal);

	tabla_de_archivos[indiceArchivo].nombre = nombre;
	tabla_de_archivos[indiceArchivo].tamanio = tamArchivo;
	tabla_de_archivos[indiceArchivo].tipo = tipo;
	tabla_de_archivos[indiceArchivo].directorio = pathToIndex(rutaLocal);
	tabla_de_archivos[indiceArchivo].bloques = ubicaciones;
	//tabla_de_archivos[indiceArchivo].ubicaciones =

	//creo mi registro de archivo local

	FILE * fp2 = fopen(rutaLocal, "w");
	if (!fp2){
	  perror("Error al crear el registro de Archivo");
	  return (-1);
	}

	fprintf(fp2, "TAMANIO=%d\nTIPO=%s\n", tabla_de_archivos[indiceArchivo].tamanio, tabla_de_archivos[indiceArchivo].tipo); //carga la info del archivo

	int cantBloques;
	int count = 0;
	int infoNodoCopia, infoNodoCopiaCopia;
	int infoBloqueCopia, infoBloqueCopiaCopia;
	int infoBytesOcupados;
	//UbicacionBloquesArchivo* bloques;

	if ( tamArchivo % tamanioBloque  == 0 )
	{
	   cantBloques = (tamArchivo/tamanioBloque);
	}
	else cantBloques =  abs(tamArchivo/tamanioBloque)+1;

	UbicacionBloquesArchivo* bloquesPtr = malloc(sizeof(UbicacionBloquesArchivo));

	while (count < cantBloques){

		//infoNodoCopia = archivosPtr[indiceArchivo].bloques[count].ubicacionCopia1.nodo;
		//infoBloqueCopia = archivosPtr[indiceArchivo].bloques[count].ubicacionCopia1.bloqueDelNodoDeLaCopia;

		bloquesPtr = list_get(ubicaciones,count);

		infoNodoCopia = bloquesPtr->ubicacionCopia1.nodo;
		infoBloqueCopia = bloquesPtr->ubicacionCopia1.desplazamiento;

		infoNodoCopiaCopia = bloquesPtr->ubicacionCopia2.nodo;
		infoBloqueCopiaCopia = bloquesPtr->ubicacionCopia2.desplazamiento;

		fprintf(fp2, "BLOQUE%dCOPIA0=[Nodo%d, %d]\n", count, infoNodoCopia, infoBloqueCopia);
		fprintf(fp2, "BLOQUE%dCOPIA1=[Nodo%d, %d]\n", count, infoNodoCopiaCopia, infoBloqueCopiaCopia);

		infoBytesOcupados = bloquesPtr->bytesOcupados;

		fprintf(fp2, "BLOQUE%dBYTES=%d\n", count, infoBytesOcupados);

		count++;
	}

	//free(archivosPtr);
	fclose(fp);
	fclose(fp2);

	cantArchivos++;
	return(1);

}

char* generarRutaLocal(char* nombre, int directorio){

	char* rutaLocal;

	strcpy(rutaLocal, string_from_format("yamafs/metadata/archivos/%d/%s", directorio, nombre));
	return(rutaLocal);

}

char* pathToType(char* path){

	/*Recibe un path y te devuelve el tipo del archivo.
	 *Devuleve el index del directorio, -1 si el directorio no existe.
	 */

	if(string_ends_with(path, "1") || string_ends_with(path, "2") || string_ends_with(path, "3") || string_ends_with(path, "4") || string_ends_with(path, "5") ||
			string_ends_with(path, "6") || string_ends_with(path, "7") || string_ends_with(path, "8") || string_ends_with(path, "9") || string_ends_with(path, "0")){

		return("directorio");
	}

	if(string_ends_with(path, ".txt"))	return("texto");
	if(string_ends_with(path, ".bin"))	return("binario");
	if(string_ends_with(path, ".csv"))	return("texto");
	return("error");

}

char* pathToFile(char* path){ //solo para yamafs

	/*Recibe un path de yamafs (/yamafs/metadata/archivos/3/ejemplo.txt)
	 * Devuelve el filename y su tipo (ejemplo.txt)
	 */

	int directorio = pathToIndex(path);
	int tamanioDir;

	char** secciones = string_split(path, "/");
	return(secciones[8]);
}

int pathToIndiceArchivo(char* path){

	/*recibe el path a un archivo
	 * devuelve su indice en el vector de archivos
	 * si no lo encuentra, devuelve -1
	 */

	int count = 0;
	char* nombre = pathToFile(path);
	int directorio = pathToIndex(path);

	while(count<100){

		if(strcmp(tabla_de_archivos[count].nombre,nombre)==0){
			return(count);
		}
		count++;
	}

	return(-1);
}

int tamanioArchivo(int fp){

	int tam;
	fseek(fp, 0L, SEEK_END);
	tam = ftell(fp);
	return(tam);

}

int cambiarNombreArchivo(char* rutaLocal, char* nombreNew){

	//Cambia el nombre de un archivo, siempre y cuando este exista.
	//recibe el nombre del archivo y el index de su directorio.
	//devuleve 1 si lo puede cambiar, -1 si no existe o no puede cambiarlo.

	int directorio;
	char* newRutaLocal;

	directorio = pathToIndex(rutaLocal);
	newRutaLocal = generarRutaLocal(nombreNew, directorio);


	int count = 0;
	char* nombre = pathToFile(rutaLocal);

	if(strcmp(newRutaLocal,"error") == 0)return(-1);
	rename(rutaLocal, newRutaLocal);

	while(count<100){

		if( strcmp(tabla_de_archivos[count].nombre,nombre)==0){
			strcpy(tabla_de_archivos[count].nombre,nombreNew);
			return(1);
		}
		count++;
	}


	return(-1);

}

int eliminarArchivo(char* rutaLocal){

	remove(rutaLocal);
	return (1);
}

int moverArchivo(char* rutaLocal, char* newRutaLocal){

	/*Borra el registro del archivo en el directorio actual y lo mueve a un nuevo directorio
	 * retorna 1 si lo puede hacer, -1 si falla.
	 */


	char caracter;

	FILE * fp = fopen(rutaLocal, "r");
		if (!fp) {
		  perror("Error al localizar el Archivo");
		  return (-1);
		}

	FILE * newfp = fopen(newRutaLocal, "w");
		if (!newfp) {
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

	int count = 0;
	char* nombre = pathToFile(rutaLocal);
	int directorio = pathToIndex(rutaLocal);

	while(count<100){

		if( strcmp(tabla_de_archivos[count].nombre,nombre)==0){
			tabla_de_archivos[count].directorio = directorio;
			return(1);
		}
		count++;
	}

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

t_list* nombreToUbicaciones(char* nombre){

	int count = 0;

	while(count<100){

			if( strcmp(tabla_de_archivos[count].nombre,nombre)){
				return(tabla_de_archivos[count].ubicaciones);

			}
			count++;
		}
	perror("error");
}

int inciarTablaDeArchivos(){

	/*typedef struct tabla_archivos {
	char* nombre;
	int tamanio; //en bytes
	char* tipo;
	int directorio; //index del directorio
	//UbicacionBloquesArchivo* bloques;
	t_list* bloques;//char *
	t_list* ubicaciones;//ubicacionBloquesArchivo
}tabla_archivos;
	 *
	 */
	int i;
	for(i=0; i<100; i++){
		tabla_de_archivos[i].nombre="deleted";
		tabla_de_archivos[i].tamanio=-1;
		tabla_de_archivos[i].tipo="";
		tabla_de_archivos[i].directorio=-1;
	}
	return(1);
}

//nodos

//funciones

int nodoToFD(int idNodo){

	int count=0;
	int retorno;


	while(count < list_size(tabla_de_nodos.listaCapacidadNodos)){

		bloques_nodo* nodo = list_get(tabla_de_nodos.listaCapacidadNodos, count);

		if(nodo->idNodo==idNodo){
			return(nodo->fileDescriptor);
		}

		count++;
	}
	return(-1);
}
