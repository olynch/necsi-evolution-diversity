/*
 * ProbDist Struct
 * Contains the various probabilities for certain actions to occur
 */
#include "Python.h"

#ifndef PROB_DIST_H
#define PROB_DIST_H

typedef struct {
	int size;
	float deathRate;
	float dist_prey1;
	float dist_prey2;
	float dist_predator;
	float repRate_prey1;
	float repRate_prey2;
	float repRate_predator;
	float mut_repRate;
	float mut_eats;
} ProbDist;

ProbDist * ProbDist_from_py_dict(PyObject *);

#endif
