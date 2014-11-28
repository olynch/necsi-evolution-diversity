#include "square.h"

Square * Square_from_args(int kind, float eats, float repRate) {
	Square *self = malloc(sizeof(Square));
	self->kind = kind;
	self->eats = eats;
	self->repRate = repRate;
	return self;
}

Square * Square_new() {
	Square *self = malloc(sizeof(Square));
	self->kind = -1;
	self->eats = 0.f;
	self->repRate = 0.f;
	return self;
}

void Square_init_from_dist(Square *self, gsl_rng *eng, ProbDist *dist) {
	double kind = gsl_ran_flat(eng, 0.0, 1.0);
	self->eats = 0.f;
	self->repRate = 0.f;
	if (kind < dist->dist_prey1) {
		self->kind = 1;
	}
	else if (kind < dist->dist_prey1 + dist->dist_prey2) {
		self->kind = 2;
	}
	else if (kind < dist->dist_prey1 + dist->dist_prey2 + dist->dist_predator) {
		self->kind = 3;
		self->repRate = contain_to_0_1(dist->repRate_predator + gsl_ran_gaussian(eng, dist->mut_repRate));
		self->eats = contain_to_0_1(0.5 + gsl_ran_gaussian(eng, dist->mut_eats));
	}
	else {
		self->kind = 0;
	}
	return;
}
