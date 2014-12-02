#include "Python.h"
#include "grid.h"

int main(int argc, char *argv[])
{
	printf("*** BEGIN TESTING GRID ****\n");

	printf("*** TESTING Grid_new ****\n");
	Grid * test_new = Grid_new();
	printf("*** Grid_new PASSED TEST \n");

	printf("*** TESTING Grid_create ****\n");
	
	Py_Initialize();
	PyObject *test_dict = PyDict_New();
	PyObject *dist_dict = PyDict_New();
	PyObject *mut_dict = PyDict_New();
	PyObject *repRate_dict = PyDict_New();
	PyDict_SetItemString(dist_dict, "prey1", PyFloat_FromDouble(0.25));
	PyDict_SetItemString(dist_dict, "prey2", PyFloat_FromDouble(0.2)); 
	PyDict_SetItemString(dist_dict, "predator", PyFloat_FromDouble(0.1));
	PyDict_SetItemString(repRate_dict, "prey1", PyFloat_FromDouble(0.3));
	PyDict_SetItemString(repRate_dict, "prey2", PyFloat_FromDouble(0.3));
	PyDict_SetItemString(repRate_dict, "predator", PyFloat_FromDouble(0.1));
	PyDict_SetItemString(mut_dict, "repRate", PyFloat_FromDouble(0.03));
	PyDict_SetItemString(mut_dict, "eatsRate", PyFloat_FromDouble(0.04));
	PyDict_SetItemString(test_dict, "dist", dist_dict);
	PyDict_SetItemString(test_dict, "repRate", repRate_dict);
	PyDict_SetItemString(test_dict, "mut", mut_dict);
	PyDict_SetItemString(test_dict, "size", PyInt_FromLong(1000l));
	PyDict_SetItemString(test_dict, "deathRate", PyFloat_FromDouble(0.3));

	ProbDist *dist = ProbDist_from_py_dict(test_dict);

	Grid * test_create = Grid_create(dist);
	Grid_print(test_create);

	printf("*** Grid_create PASSED TEST \n");

	printf("*** TESTING Grid_refresh \n");

	printf("BEFORE REFRESH\n");
	Grid_print(test_create);
	Grid_refresh(test_create);
	printf("AFTER REFRESH\n");
	Grid_print(test_create);

	printf("*** Grid_refresh PASSED TEST\n");

	printf("*** TESTING Grid_get_cur ****\n");

	Square_print(Grid_get_cur(test_create, 4, 9));
	printf("OUT OF BOUNDS: SHOULD WRAP\n");
	Square_print(Grid_get_cur(test_create, 14, 0));

	printf("*** Grid_get_cur PASSED TEST\n");

	printf("*** TESTING Grid_step 1000 times ****\n");

	for (int i = 0; i < 1000; i++) {
		Grid_step(test_create);
		printf("\n\n");
	}

	printf("*** Grid_step PASSED TEST\n");

	printf("*** ALL TESTS COMPLETED ***\n");

	return 0;
}
