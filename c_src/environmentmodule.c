#include <Python.h>
#include "prob_dist.h"
#include "square.h"
#include "grid.h"

typedef struct {
	PyObject_HEAD
	Grid *grid;
	PyObject *state;
	PyObject *stats_maxrep;
	PyObject *stats_minrep;
	PyObject *stats_avgrep;
	PyObject *stats_eats_data;
} Environment;

static void Environment_dealloc(Environment* self) {
	Py_XDECREF(self->state);
	Py_XDECREF(self->stats_maxrep);
	Py_XDECREF(self->stats_minrep);
	Py_XDECREF(self->stats_avgrep);
	Py_XDECREF(self->stats_eats_data);
	Grid_dealloc(self->grid);
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
Envrironment_init(Environment *self, PyObject *args, PyObject *kwds) {
	PyObject *probability_map = NULL;
	static char *kwlist[] = {"probability_map", NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &probability_map)) {
		return -1;
	}

	if (probability_map) {
		/* init using probability_map */
		ProbDist *dist = ProbDist_from_py_dict(probability_map);
	}
	self->grid = Grid_create(dist);
	self->state = NULL; /* needs to be np_array[size][size][3] of doubles */
	self->stats_maxrep = PyList_New(0);
	self->stats_minrep = PyList_New(0);
	self->stats_avgrep = PyList_New(0);
	self->stats_eats_data = PyList_New(0);
	return self;
}
