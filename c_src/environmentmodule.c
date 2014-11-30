#include <Python.h>
#include "prob_dist.h"
#include "square.h"
#include "common.h"
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

static int
Environment_init(Environment *self, PyObject *args, PyObject *kwds) {
	PyObject *probability_map = NULL;
	ProbDist *dist;
	static char *kwlist[] = {"probability_map", NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &probability_map)) {
		return -1;
	}

	if (probability_map) {
		/* init using probability_map */
		dist = ProbDist_from_py_dict(probability_map);
	}
	self->grid = Grid_create(dist);
	int dims[] = { dist->size, dist->size, 3 };
	self->state = PyArray_ZEROES(3, dims, NPY_UINT8, 0);
	/* needs to be np_array[size][size][3] of UINT8 */
	self->stats_maxrep = PyList_New(0);
	self->stats_minrep = PyList_New(0);
	self->stats_avgrep = PyList_New(0);
	self->stats_eats_data = PyList_New(0);
	return 0;
}

const npy_uchar EMPTY[3] = { 0xff, 0xff, 0xff };
const npy_uchar PREY1[3] = { 0x00, 0xff, 0xff };
const npy_uchar PREY2[3] = { 0x00, 0x88, 0x00 };
const npy_uchar PREDATOR_1[3] = { 0xff, 0x00, 0x00 };
const npy_uchar PREDATOR_2[3] = { 0x88, 0x00, 0x00 };

static PyObject *
Environment_step(Environment *self) {
	Grid_step(self->grid);
	Square *cur;
	npy_uchar *cur_R;
	npy_uchar *cur_G;
	npy_uchar *cur_B;
	int total_pred = 0;
	double eats_total = 0.0;
	float max_eats = 0.0;
	float min_eats = 0.0;
	Py_XDECREF(self->stats_eats_data);
	self->stats_eats_data = PyList_New(0);

	for (int i = 0; i < self->grid->size; ++i) {
		for (int j = 0; j < self->grid->size; ++j) {
			cur = Grid_get_cur(self->grid, i, j);
			switch (cur->kind) {
				cur_R = PyArray_GETPTR3(self->state, i, j, 0);
				cur_G = PyArray_GETPTR3(self->state, i, j, 1);
				cur_B = PyArray_GETPTR3(self->state, i, j, 2);
				case EMPTY:
					*cur_R = EMPTY[0];
					*cur_G = EMPTY[1];
					*cur_B = EMPTY[2];
				case PREY1:
					*cur_R = PREY1[0];
					*cur_G = PREY1[1];
					*cur_B = PREY1[2];
				case PREY2:
					*cur_R = PREY2[0];
					*cur_G = PREY2[1];
					*cur_B = PREY2[2];
				case PREDATOR:
					total_pred++;
					eats_total += (double) cur->eats;
					if (cur->eats > max_eats) {
						max_eats = cur->eats;
					}
					if (cur->eats < min_eats) {
						min_eats = cur->eats;
					}
					if (cur->eats < 0.5) {
						*cur_R = PREDATOR_1[0];
						*cur_G = PREDATOR_1[1];
						*cur_B = PREDATOR_1[2];
					}
					else {
						*cur_R = PREDATOR_2[0];
						*cur_G = PREDATOR_2[1];
						*cur_B = PREDATOR_2[2];
					}
					PyList_Append(self->stats_eats_data, PyFloat_FromDouble((double) self->eats));
			}
		}
	}
	PyList_Append(self->stats_maxrep, PyFloat_FromDouble((double) max_eats));
	PyList_Append(self->stats_minrep, PyFloat_FromDouble((double) min_eats));
	PyList_Append(self->stats_avgrep, PyFloat_FromDouble(eats_total / total_pred));
	Py_RETURN_NONE;
}

static PyMemberDef Environment_members[] = {
	{"state", T_OBJECT_EX, offsetof(Environment, state), 0, "state representation"},
	{"stats_maxrep", T_OBJECT_EX, offsetof(Environment, stats_maxrep), 0, "stats_maxrep"},
	{"stats_minrep", T_OBJECT_EX, offsetof(Environment, stats_minrep), 0, "stats_minrep"},
	{"stats_avgrep", T_OBJECT_EX, offsetof(Environment, stats_avgrep), 0, "stats_avgrep"},
	{"stats_eats_data", T_OBJECT_EX, offsetof(Environment, stats_eats_data), 0, "stats_eats_data"},
	{NULL} //sentinel
};

static PyMethodDef Environment_methods[] = {
	{"step", (PyCFunction)Environment_step, METH_NOARGS, "run the simulation for a step"},
	{NULL} //sentinel
};

static PyTypeObject EnvironmentType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "environment.Environment",             /*tp_name*/
    sizeof(Environment),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Environment_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Environment",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Environment_methods,             /* tp_methods */
    Environment_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Environment_init,      /* tp_init */
    0,                         /* tp_alloc */
    0,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC initenvironment(void) 
{
    PyObject* m;

	EnvironmentType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&EnvironmentType) < 0)
        return;

    m = Py_InitModule3("environment", module_methods,
                       "Module that contains the environment class for simulation.");

    if (m == NULL)
      return;

    Py_INCREF(&EnvironmentType);
    PyModule_AddObject(m, "Environment", (PyObject *)&EnvironmentType);
}
