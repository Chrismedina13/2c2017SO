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
