/*
 * grid.h
 * Grid struct and associated functions
 */

#include "prob_dist.h"
#include "square.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include "common.h"
#include <stdbool.h>
#include <time.h>

#ifndef GRID_H
#define GRID_H

typedef Square *grid_data;

typedef struct {
	grid_data data;
	grid_data data_next;
	int size;
	gsl_rng *rand_eng;
	ProbDist *dist;
} Grid;

Grid * Grid_new(void);
Grid * Grid_create(ProbDist *);
void Grid_seed(Grid *);
void Grid_dealloc(Grid *);
void Grid_refresh(Grid *);
void Grid_step(Grid *);
void Grid_print(Grid *);
Square * Grid_get_cur(Grid *, int, int);
Square * Grid_get_next(Grid *, int, int);

void Grid_step(Grid*);

#endif
