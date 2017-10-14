/*
 ============================================================================
 Name        : Test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

void test1(void);
void test2(void);
void test3(void);



int main() {
  CU_initialize_registry();

  CU_pSuite prueba = CU_add_suite("Suite de prueba", NULL, NULL);
  CU_add_test(prueba, "uno", test1);
  CU_add_test(prueba, "dos", test2);
  CU_add_test(prueba, "tres", test3);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}


void test1() {
	printf("Soy el test 1!, y pruebo que 2 sea igual a 1+1");
	CU_ASSERT_EQUAL(1+1, 2);
}

void test2() {
	printf("Soy el test 2!, y doy segmentation fault");
	char* ptr = NULL;
	*ptr = 9;
}

void test3() {
	printf("Soy el test 3!");
}

