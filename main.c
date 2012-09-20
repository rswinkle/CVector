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
#include "vector.h"


extern void pushi_test();
extern void erasei_test();
extern void inserti_test();
extern void popi_test();
extern void reservei_test();
extern void set_capacityi_test();
extern void set_vali_test();

extern void pushd_test();
extern void erased_test();
extern void insertd_test();
extern void popd_test();
extern void reserved_test();
extern void set_capacityd_test();
extern void set_vald_test();


extern void pushs_test();
extern void erases_test();
extern void inserts_test();
extern void pops_test();
extern void reserves_test();
extern void set_capacitys_test();
extern void set_vals_test();


extern void push_test();
extern void erase_test();
extern void insert_test();
extern void pop_test();
extern void reserve_test();
extern void set_capacity_test();
extern void set_val_test();
extern void vector_of_vectors_test();

extern void template_test();




CU_TestInfo vector_i_tests[] = {
	{ "push_test",           pushi_test },
	{ "erase_test",          erasei_test },
	{ "insert_test",         inserti_test },
	{ "pop_test",            popi_test },
	{ "reserve_test",        reservei_test },
	{ "set_capacityi_test",  set_capacityi_test },
	{ "set_val_test",        set_vali_test }
};

CU_TestInfo vector_d_tests[] = {
	{ "push_test",           pushd_test },
	{ "erase_test",          erased_test },
	{ "insert_test",         insertd_test },
	{ "pop_test",            popd_test },
	{ "reserve_test",        reserved_test },
	{ "set_capacityd_test",  set_capacityd_test },
	{ "set_val_test",        set_vald_test }
};

CU_TestInfo vector_s_tests[] = {
	{ "push_test",           pushs_test },
	{ "erase_test",          erases_test },
	{ "insert_test",         inserts_test },
	{ "pop_test",            pops_test },
	{ "reserve_test",        reserves_test },
	{ "set_capacitys_test",  set_capacitys_test },
	{ "set_val_test",        set_vals_test }
};


CU_TestInfo vector_tests[] = {
	{ "push_test",                 push_test },
	{ "erase_test",                erase_test },
	{ "insert_test",               insert_test },
	{ "pop_test",                  pop_test },
	{ "reserve_test",              reserve_test },
	{ "set_capacity_test",         set_capacity_test },
	{ "set_val_test",              set_val_test },
	{ "vector_of_vectors_test",    vector_of_vectors_test }
};

CU_TestInfo template_tests[] = {
	{ "template_test",       template_test }
};



CU_SuiteInfo vector_suites[] = {
	{ "vector_i", NULL, NULL, vector_i_tests },
	{ "vector_d", NULL, NULL, vector_d_tests },
	{ "vector_s", NULL, NULL, vector_s_tests },
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
	fprintf(stdout, "%d", CU_get_error());

	CU_cleanup_registry();

	return CU_get_error();

}
