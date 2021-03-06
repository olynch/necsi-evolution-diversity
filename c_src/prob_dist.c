#include <Python.h>
#include "prob_dist.h"

ProbDist * ProbDist_from_py_dict(PyObject *dict) {
	PyObject *dist = PyDict_GetItemString(dict, "dist");
	PyObject *repRate = PyDict_GetItemString(dict, "repRate");
	PyObject *mut = PyDict_GetItemString(dict, "mut");
	ProbDist *self = malloc(sizeof(ProbDist));
	self->size = PyInt_AsLong(PyDict_GetItemString(dict, "size"));
	self->deathRate = (float) PyFloat_AsDouble(PyDict_GetItemString(dict, "deathRate"));
	self->dist_prey1 = (float) PyFloat_AsDouble(PyDict_GetItemString(dist, "Prey1"));
	self->dist_prey2 = (float) PyFloat_AsDouble(PyDict_GetItemString(dist, "Prey2"));
	self->dist_predator = (float) PyFloat_AsDouble(PyDict_GetItemString(dist, "Predator"));
	self->repRate_prey1 = (float) PyFloat_AsDouble(PyDict_GetItemString(repRate, "Prey1"));
	self->repRate_prey2 = (float) PyFloat_AsDouble(PyDict_GetItemString(repRate, "Prey2"));
	self->repRate_predator = (float) PyFloat_AsDouble(PyDict_GetItemString(repRate, "Predator"));
	self->mut_repRate = (float) PyFloat_AsDouble(PyDict_GetItemString(mut, "repRate"));
	self->mut_eats = (float) PyFloat_AsDouble(PyDict_GetItemString(mut, "eats"));
	return self;
}
