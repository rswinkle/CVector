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
#include "vector_tests.h"


CU_TestInfo vector_i_tests[] = {
	{ "push_test",		pushi_test },
	{ "erase_test",		erasei_test },
	{ "insert_test",	inserti_test },
	{ "pop_test",		popi_test },
	{ "reserve_test",	reservei_test },
	{ "set_val_test",	set_vali_test }
};

CU_TestInfo vector_d_tests[] = {
	{ "push_test",		pushd_test },
	{ "erase_test",		erased_test },
	{ "insert_test",	insertd_test },
	{ "pop_test",		popd_test },
	{ "reserve_test",	reserved_test },
	{ "set_val_test",	set_vald_test }
};

CU_TestInfo vector_s_tests[] = {
	{ "push_test",		pushs_test },
	{ "erase_test",		erases_test },
	{ "insert_test",	inserts_test },
	{ "pop_test",		pops_test },
	{ "reserve_test",	reserves_test },
	{ "set_val_test",	set_vals_test }
};


CU_TestInfo vector_tests[] = {
	{ "push_test",		push_test },
	{ "erase_test",		erase_test },
	{ "insert_test",	insert_test },
	{ "pop_test",		pop_test },
	{ "reserve_test",	reserve_test },
	{ "set_val_test",	set_val_test }
};



CU_SuiteInfo vector_suites[] = {
  { "vector_i", suite1_init-func, suite1_cleanup_func, vector_i_tests },
  { "vector_d", suite2_init-func, suite2_cleanup_func, vector_d_tests },
  { "vector_s", suite2_init-func, suite2_cleanup_func, vector_s_tests },
  { "vector", suite2_init-func, suite2_cleanup_func, vector_tests },
  CU_SUITE_INFO_NULL,
};



int main()
{

	/* test vector_i and vector_d */
	vector_i* testi = vec_i(0);
	vector_d* testd = vec_d(0);

	int i=0;
	for(i=0; i<100; i++) {
		push_backi(testi, i);
		push_backd(testd, i+0.5);
	}

	for(i=0; i<testi->size; i++) {
		printf("%d\t%lf\n", testi->a[i], testd->a[i]);
	}

	erasei(testi, 50, 50);
	erased(testd, 25, 74);

	for(i=0; i<testi->size; i++)
		printf("%d\n", testi->a[i]);

	printf("\n\n");
	for(i=0; i<testd->size; i++)
		printf("%lf\n", testd->a[i]);


	inserti(testi, 50, 50);
	for(i=0; i<50; i++)
		insertd(testd, 25, 74-i+0.5);

	for(i=0; i<testi->size; i++)
		printf("%d\n", testi->a[i]);

	printf("\n\n");
	for(i=0; i<testd->size; i++)
		printf("%lf\n", testd->a[i]);

	printf("\n\n");

	for(i=0; i<19; i++) {
		printf("%d\t%lf\n", pop_backi(testi), pop_backd(testd) );
	}


	printf("\n\n");
	for(i=0; i<testi->size; i++)
		printf("%d\n", testi->a[i]);

	printf("\n\n");
	for(i=0; i<testd->size; i++)
		printf("%lf\n", testd->a[i]);

	printf("\n\n");


	/* Test vector_s */
	char buffer[50];
	vector_s* test_s = vec_s(0);
	for(i=0; i<50; i++) {
		sprintf(buffer, "hello %d", i);
		push_backs(test_s, buffer);
	}

	for(i=0; i<test_s->size; i++)
		printf("%s\n", test_s->a[i]);

	printf("\n\n");

//	for(i=0; i<40-11; i++)
//		erases(test_s, 11, 11);

	erases(test_s, 11, 39);

	for(i=0; i<test_s->size; i++)
		printf("%s\n", test_s->a[i]);

	printf("\n\n");


	for(i=0; i<40-11; i++) {
		sprintf(buffer, "hello %d", 39-i);
		inserts(test_s, 11, buffer);
	}

	for(i=0; i<test_s->size; i++)
		printf("%s\n", test_s->a[i]);

	printf("\n\n");

	free_veci(testi);
	free_vecd(testd);
	free_vecs(test_s);


	return 0;
}
