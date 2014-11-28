/*
 * square.h
 * The Square struct and related methods
 */

#include "prob_dist.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include "common.h"

#ifndef SQUARE_H
#define SQUARE_H

typedef struct {
	float eats;
	float repRate;
	int kind;
} Square;

Square * Square_from_args(int kind, float eats, float repRate);

Square * Square_new(void);

void Square_init_from_dist(Square *self, gsl_rng *eng, ProbDist *dist);

#endif
