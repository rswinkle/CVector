/*
 * main.c
 *
 *  Created on: Mar 22, 2011
 *      Author: robert
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Automated.h>
#include "vector_all.h"


extern void push_i_test();
extern void erase_i_test();
extern void insert_i_test();
extern void insert_array_i_test();
extern void pop_i_test();
extern void reserve_i_test();
extern void set_capacity_i_test();
extern void set_val_i_test();

extern void push_d_test();
extern void erase_d_test();
extern void insert_d_test();
extern void insert_array_d_test();
extern void pop_d_test();
extern void reserve_d_test();
extern void set_capacity_d_test();
extern void set_val_d_test();


extern void push_str_test();
extern void erase_str_test();
extern void insert_str_test();
extern void insert_array_str_test();
extern void pop_str_test();
extern void reserve_str_test();
extern void set_capacity_str_test();
extern void set_val_str_test();


extern void push_void_test();
extern void erase_void_test();
extern void insert_void_test();
extern void insert_array_void_test();
extern void pop_void_test();
extern void reserve_void_test();
extern void set_capacity_void_test();
extern void set_val_void_test();
extern void vector_of_vectors_test();

extern void template_test();




CU_TestInfo vector_i_tests[] = {
	{ "push_i_test",           push_i_test },
	{ "erase_i_test",          erase_i_test },
	{ "insert_i_test",         insert_i_test },
	{ "insert_array_i_test",   insert_array_i_test },
	{ "pop_i_test",            pop_i_test },
	{ "reserve_i_test",        reserve_i_test },
	{ "set_capacity_i_test",   set_capacity_i_test },
	{ "set_val_i_test",        set_val_i_test },
	CU_TEST_INFO_NULL
};

CU_TestInfo vector_d_tests[] = {
	{ "push_d_test",           push_d_test },
	{ "erase_d_test",          erase_d_test },
	{ "insert_d_test",         insert_d_test },
	{ "insert_array_d_test",   insert_array_d_test },
	{ "pop_d_test",            pop_d_test },
	{ "reserve_d_test",        reserve_d_test },
	{ "set_capacity_d_test",   set_capacity_d_test },
	{ "set_val_d_test",        set_val_d_test },
	CU_TEST_INFO_NULL
};

CU_TestInfo vector_s_tests[] = {
	{ "push_str_test",           push_str_test },
	{ "erase_str_test",          erase_str_test },
	{ "insert_str_test",         insert_str_test },
	{ "insert_array_str_test",   insert_array_str_test },
	{ "pop_str_test",            pop_str_test },
	{ "reserve_str_test",        reserve_str_test },
	{ "set_capacity_str_test",   set_capacity_str_test },
	{ "set_val_str_test",        set_val_str_test },
	CU_TEST_INFO_NULL
};


CU_TestInfo vector_tests[] = {
	{ "push_test",                 push_void_test },
	{ "erase_test",                erase_void_test },
	{ "insert_test",               insert_void_test },
	{ "insert_array_test",         insert_array_void_test },
	{ "pop_test",                  pop_void_test },
	{ "reserve_test",              reserve_void_test },
	{ "set_capacity_test",         set_capacity_void_test },
	{ "set_val_test",              set_val_void_test },
	{ "vector_of_vectors_test",    vector_of_vectors_test },
	CU_TEST_INFO_NULL
};

CU_TestInfo template_tests[] = {
	{ "template_test",       template_test },
	CU_TEST_INFO_NULL
};



CU_SuiteInfo vector_suites[] = {
	{ "vector_i", NULL, NULL, vector_i_tests },
	{ "vector_d", NULL, NULL, vector_d_tests },
	{ "vector_str", NULL, NULL, vector_s_tests },
	{ "vector",   NULL, NULL, vector_tests   },
	{ "template", NULL, NULL, template_tests },
	CU_SUITE_INFO_NULL
};



int main()
{
	CU_ErrorCode error;
	if (CUE_SUCCESS != CU_initialize_registry())
	      return CU_get_error();

	error = CU_register_suites(vector_suites);

	if (error != CUE_SUCCESS)
		fprintf(stderr, "wtf!");


	CU_automated_run_tests();
	fprintf(stdout, "CU_get_error() returned %d\n", CU_get_error());

	CU_cleanup_registry();

	return CU_get_error();

}
