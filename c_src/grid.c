#include "grid.h"
#define RD() gsl_ran_flat(self->rand_eng, 0.0, 1.0)
#define REP_RATE_RD() gsl_ran_gaussian(self->rand_eng, self->dist->mut_repRate)
#define EATS_RATE_RD() gsl_ran_gaussian(self->rand_eng, self->dist->mut_eats)

Grid * Grid_create(ProbDist *dist) {
	int size = dist->size;
	Grid *self = malloc(sizeof(Grid));
	self->rand_eng = gsl_rand_alloc(gsl_rng_mt19937);
	ProbDist *copy_of_dist = malloc(sizeof(Square));
	*copy_of_dist = *dist;
	self->dist = copy_of_dist;
	self->size = size;
	self->data = malloc(sizeof(Square) * (size * size));
	self->data_next = malloc(sizeof(Square) * (size * size));
	Grid_seed(self);
	return self;
}

void Grid_seed(Grid *self) {
	Square *next;
	double rand;
	for (int i = 0; i < (self->size * self->size); ++i) {
		next = &self->data_next[i];
		rand = gsl_ran_flat(self->rand_eng, 0.0, 1.0);
		if (rand <= self->dist->dist_predator) {
			next->kind = PREDATOR;
			next->repRate = 0.5;
			next->eats = (float) gsl_ran_gaussian(self->rand_eng, 0.5, 0.1);
		} else if (rand <= self->dist->dist_predator + self->dist->dist_prey1) {
			next->kind = PREY1;
			rext->repRate = 0.0;
			next->eats = 0.0;
		} else if (rand <= self->dist->dist_predator + self->dist->dist_prey1 + self->dist->dist_prey2) {
			next->kind = PREY2;
			next->repRate = 0.0;
			next->eats = 0.0;
		} else {
			next->kind = EMPTY;
			next->repRate = 0.0;
			next->eats = 0.0;
		}
	}
	Grid_refresh(self);
}

void Grid_dealloc(Grid *self) {
	free(self->data);
	free(self->data_next);
	gls_rng_free(self->rand_eng);
	free(self->dist);
	free(self);
}

void Grid_refresh(Grid *self) {
	grid_data tmp = self->data;
	self->data = self->data_next;
	self->data_next = tmp;
}

void Grid_step(Grid *self) {
	/* initialization of variables used throughout the loop */
	Square *cur;
	Square *next;
	int numPrey1;
	bool prey1reproduces;
	int numPrey2;
	bool prey2reproduces;
	Square *opts[8];
	int opts_size;

	/* main loop, travels through whole grid */
	for (int i = 0; i < self->size; ++i) {
		for (int j = 0; j < self->size, ++j) {
			cur = Grid_get_cur(self, i, j);
			next = Grid_get_next(self, i, j);
			if (cur->type == EMPTY) {
				numPrey1 = 0;
				prey1reproduces = false;
				numPrey2 = 0;
				prey2reproduces = false;
				for (int k = -1; k <= 1; ++k) {
					for (int l = -1; l <= 1; ++l) {
						if (Grid_get_cur(self, i + k, j + l)->type == PREY1)
							++numPrey1;
						if (Grid_get_cur(self, i + k, j + l)->type == PREY2)
							++numPrey2;
					}
				}
				if (RD() < (1 - pow(1 - self->dist->repRate_prey1, numPrey1))) {
					prey1reproduces = true;
				}
				if (RD() < (1 - pow(1 - self->dist->repRate_prey2, numPrey2))) {
					prey2reproduces = true;
				}
				if (prey1reproduces && prey2reproduces) {
					next->kind = (int) floor(RD() * 2 + 1);
				}
				else if (prey1reproduces) {
					next->kind = 1;
				}
				else if (prey2reproduces) {
					next->kind = 2;
				}
			}
			else if (cur->type == PREY1 || cur->type == PREY2) {
				opts_size = 0;
				Square *pred;
				for (int k = -1; k <= 1; ++k) {
					for (int l = -1 ; <= 1; ++l) {
						pred = Grid_get_cur(self, i + k, j + l);
						if (pred->type == 3 && RD() < pred->repRate) {
							opts[opts_size] = pred;
							opts_size++;
						}
					}
				}
				if (opts_size > 0) {
					Square *opt = opts[floor(RD() * opts_size)];
					next->kind = 3;
					next->repRate = contain_to_0_1(opt->repRate + REP_RATE_RD());
					next->eats = contain_to_0_1(opt->eats + EATS_RATE_RD());
				}
			}
			else if (cur->type == PREDATOR) {
				if (RD() < self->deathRate) {
					next->kind = 0;
				}
			}
		}
	}
	Grid_refresh(self);
}

Square * Grid_get_cur(Grid *self, int x, int y) {
	/* READONLY */
	return &self->data[((self->size * x) + y)];
}

Square * Grid_get_next(Grid *self, int x, int y) {
	/* WRITEONLY */
	return &self->data_next[((self->size * x) + y)]
}
