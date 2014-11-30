/*
 * square.h
 * The Square struct and related methods
 */

#include "prob_dist.h"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include "common.h"
#include <stdio.h>

#ifndef SQUARE_H
#define SQUARE_H

extern const int EMPTY;
extern const int PREY1;
extern const int PREY2;
extern const int PREDATOR;

typedef struct {
	float eats;
	float repRate;
	int kind;
} Square;

Square * Square_from_args(int kind, float eats, float repRate);

Square * Square_new(void);

void Square_init_from_dist(Square *self, gsl_rng *eng, ProbDist *dist);
void Square_print(Square *self);

#endif
