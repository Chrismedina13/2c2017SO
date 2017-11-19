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
