/*
 ============================================================================
 Name        : Test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "Test.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void test1(void);




int main() {
  CU_initialize_registry();

  CU_pSuite prueba = CU_add_suite("Suite de prueba", NULL, NULL);

  CU_add_test(prueba, "uno", test1);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}


void test1() {



	int prueba = generarNumeroAleatorioNoRepetido();



	CU_ASSERT_EQUAL(prueba, 3);
}



int generarNumeroAleatorioNoRepetido(){

	srand(time(NULL)); //El mayordomo pone a girar la diana
	int test = rand() %100; //Primer disparo del robot
	printf ("El numero aleatorio de test vale %d\n", test);
	printf ("Otros numeros aleatorios son: %d, %d, %d\n",rand(),rand(),rand()) ;
	printf("La constante RAND_MAX vale %d\n", RAND_MAX);

	return test;

}

