/*
 * grid.h
 * Grid struct and associated functions
 */

#include "prob_dist.h"
#include "square.h"
#include "gsl_rng.h"
#include "gsl_randist.h"

#ifndef GRID_H
#define GRID_H

const int EMPTY = 0;
const int PREY1 = 1;
const int PREY2 = 2;
const int PREDATOR = 3;

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
void Grid_refresh(Grid *);
void Grid_step(Grid *);
void Grid_print(Grid *);
Square * Grid_get_cur(Grid *, int, int);
Square * Grid_get_next(Grid *, int, int);

void Grid_step(Grid*);

#endif
