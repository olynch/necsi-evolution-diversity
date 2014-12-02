#include "prob_dist.h"

int main(int argc, char *argv[])
{
	Py_Initialize();
	printf("*** BEGINNING TESTS ****\n");

	printf("*** TESTING ProbDist_from_py_dict ****\n");
	PyObject *test_dict = PyDict_New();
	PyObject *dist_dict = PyDict_New();
	PyObject *mut_dict = PyDict_New();
	PyObject *repRate_dict = PyDict_New();
	PyDict_SetItemString(dist_dict, "Prey1", PyFloat_FromDouble(0.25));
	PyDict_SetItemString(dist_dict, "Prey2", PyFloat_FromDouble(0.2)); 
	PyDict_SetItemString(dist_dict, "Predator", PyFloat_FromDouble(0.1));
	PyDict_SetItemString(repRate_dict, "Prey1", PyFloat_FromDouble(0.3));
	PyDict_SetItemString(repRate_dict, "Prey2", PyFloat_FromDouble(0.3));
	PyDict_SetItemString(repRate_dict, "Predator", PyFloat_FromDouble(0.1));
	PyDict_SetItemString(mut_dict, "repRate", PyFloat_FromDouble(0.03));
	PyDict_SetItemString(mut_dict, "eats", PyFloat_FromDouble(0.04));
	PyDict_SetItemString(test_dict, "dist", dist_dict);
	PyDict_SetItemString(test_dict, "repRate", repRate_dict);
	PyDict_SetItemString(test_dict, "mut", mut_dict);
	PyDict_SetItemString(test_dict, "size", PyInt_FromLong(10l));
	PyDict_SetItemString(test_dict, "deathRate", PyFloat_FromDouble(0.3));

	ProbDist *test = ProbDist_from_py_dict(test_dict);
	printf("size: %i, deathRate: %f\n"
			"dist_prey1: %f, dist_prey2: %f, dist_predator: %f\n"
			"repRate_prey1: %f, repRate_prey2: %f, repRate_predator: %f\n"
			"mut_repRate: %f, mut_eats: %f\n"
			, test->size, test->deathRate, test->dist_prey1, test->dist_prey2, test->dist_predator, test->repRate_prey1, test->repRate_prey2, test->repRate_predator, test->mut_repRate, test->mut_eats);

	printf("*** TEST COMPLETED ****\n");

	printf("*** ALL TESTS COMPLETED ****\n");
	return 0;
}
