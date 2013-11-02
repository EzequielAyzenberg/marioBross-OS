#include "CUnit/Basic.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

//que debe hacer antes de cada prueba, abrir un archivo un socket o que 
int inicializar() {
	
	return 0;
}

//que debe hacer despues de cada prueba. depende de lo que hicimos antes 
int limpiar() {
	
	return 0;
}

// un tipo de test, hay de muchos tipo especifico en comentarios los que restan 
void test_para_ver_que_esto_funciona() {
	inicializar();
	int cantidad = -1;
	CU_ASSERT_EQUAL(cantidad, -1);
}

void test_para_ver_que_agregue_uno_distinto() {
	inicializar();
	int cantidad = prueba();
	CU_ASSERT_EQUAL(cantidad, 7);
}

/*
CU_ASSERT_TRUE(value): Verifica que una expresión sea verdadera
CU_ASSERT_FALSE(value): Verifica que una expresión sea falsa
CU_ASSERT_EQUAL(actual, expected): Verifica que actual == expected
CU_ASSERT_NOT_EQUAL(actual, expected)
CU_ASSERT_STRING_EQUAL(actual, expected): Verifica que dos strings sean equivalentes
CU_ASSERT_STRING_NOT_EQUAL(actual, expected)
CU_ASSERT_PTR_EQUAL(actual, expected): Verifica que los punteros sean equivalentes
CU_ASSERT_PTR_NOT_EQUAL(actual, expected)
CU_ASSERT_PTR_NULL(value): Verifica que un puntero es NULL
CU_ASSERT_PTR_NOT_NULL(value)
CU_ASSERT_NSTRING_EQUAL(actual, expected, count): Verifica que los primeros count caracteres de las cadenas coinciden
*/

 
//aca se agregan los test !!LEER CON ANTENCION¡¡
//se puede poner mas de uno 
void agregar_tests() {
	CU_pSuite archivo = CU_add_suite("Prueba", inicializar, limpiar);  //nombre de la suite de prueba se puede cambiar y el puntero a las funciones NO se puede cambiar
	CU_add_test(archivo, "este es el mensaje que deberia decir que testea", test_para_ver_que_esto_funciona); //nombre de la prueba, puntero a la funcion prueba.
    	CU_add_test(archivo, "cambien el mensaje tambien, verifico prueba ()", test_para_ver_que_agregue_uno_distinto); 
}


//aca se hace la magia de los test. 
int gridTests() {

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	agregar_tests();

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	printf("\n");
	CU_basic_show_failures(CU_get_failure_list());
	printf("\n\n");

	CU_cleanup_registry();

	return CU_get_error();
}


