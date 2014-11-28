/*
 * square.h
 * The Square struct and related methods
 */

#include "prod_dist.h"

#ifndef SQUARE_H
#define SQUARE_H

typedef struct {
	float eats;
	float repRate;
	int kind;
} Square;

Square* Square_from_args(int kind, float eats, float repRate);

Square* Square_from_dist(ProbDist* dist);

#endif
