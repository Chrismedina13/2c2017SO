/*
 * FuncionesAuxiliares.c
 *
 *  Created on: 3/11/2017
 *      Author: utnso
 */

#include "FuncionesAuxiliares.h"



int generarNumeroAleatorioNoRepetido(){

	srand(time(NULL));
	int test = rand() %100;
	printf ("El numero aleatorio del JOB vale %d\n", test);

	return test;

}

char* generarNombreArchivoTransformacion(int variableTransformacion){

	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/Transf-");
	string_append(&unaPalabra, string_itoa(variableTransformacion));
	return unaPalabra;
}

char* generarNombreArchivoReduccionLocal(int variableRL){

	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/RL-");
	string_append(&unaPalabra, string_itoa(variableRL));
	return unaPalabra;

}
char* generarNombreArchivoReduccionGlobal(int variableRG){


	char *unaPalabra = string_new();
	string_append(&unaPalabra, "/tmp/RG-");
	string_append(&unaPalabra, string_itoa(variableRG));
	return unaPalabra;
}


char* obtenerPuntero(const char* rutaArchivo){

	struct stat sb;
	off_t len;
	char* p;
	int fd;

	fd = open (rutaArchivo, O_RDONLY);
	//fd = fopen(rutaArchivo,"r");
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

	return p;
}

int tamanioEstructuraInfoWorker(Info_Workers* info){
	int tamanio = sizeof(int) + strlen(info->ipWorker);
	return tamanio;
}
int tamanioEstructurasListaWorkers(t_list* listaWorkers){
	int tamanioWorker;
	int tamanioListaTotal=0;
	int i;
	for(i=0;i<list_size(listaWorkers);i++){
		tamanioWorker = tamanioEstructuraInfoWorker(list_get(listaWorkers,i));
		tamanioListaTotal = tamanioListaTotal+tamanioWorker;
	}
	return tamanioListaTotal;
}
void setInfoWorker(Info_Workers* info, int puerto,char* ipWorker){
	info = malloc(sizeof(int) + strlen(ipWorker) * sizeof(char));
	info->puerto = puerto;
	info->ipWorker = ipWorker;
}

int tamanioEstructuraUbicacionBloquesArchivo(UbicacionBloquesArchivo* info){
	int tamanio = sizeof(int)*6;
	return tamanio;
}
int tamanioEstructurasListaUbicacionBloquesArchivo(t_list* listaUbicaciones){
	int tamanioUbicacion;
	int tamanioListaTotal=0;
	int i;
	for(i=0;i<list_size(listaUbicaciones);i++){
		tamanioUbicacion = tamanioEstructuraUbicacionBloquesArchivo(list_get(listaUbicaciones,i));
		tamanioListaTotal = tamanioListaTotal+tamanioUbicacion;
	}
	return tamanioListaTotal;
}
