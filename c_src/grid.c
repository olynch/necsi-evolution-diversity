#include "grid.h"

Grid * Grid_new() {
	Grid *self = malloc(sizeof(Grid));
	self->rand_eng = NULL;
	self->dist = NULL;
	self->size = 0;
	self->data = NULL;
	self->data_next = NULL;
	return self;
}

Grid * Grid_create(ProbDist *dist) {
	int size = dist->size;
	Grid *self = malloc(sizeof(Grid));
	self->rand_eng = gsl_rng_alloc(gsl_rng_mt19937);
	gsl_rng_set(self->rand_eng, (uint32_t)time(NULL));
	ProbDist *copy_of_dist = malloc(sizeof(ProbDist));
	*copy_of_dist = *dist;
	self->dist = copy_of_dist;
	self->size = size;
	self->data = malloc(sizeof(Square) * (size * size));
	self->data_next = malloc(sizeof(Square) * (size * size));
	Grid_seed(self);
	return self;
}

void Grid_print(Grid *self) {
	Square *cur;
	for (int i = 0; i < self->size; ++i) {
		for (int j = 0; j < self->size; ++j) {
			cur = Grid_get_cur(self, i, j);
			if (cur->kind == EMPTY)
				printf(" ");
			else if (cur->kind == PREY1)
				printf("@");
			else if (cur->kind == PREY2)
				printf("#");
			else
				printf("^");
		}
		printf("\n");
	}
}

void Grid_seed(Grid *self) {
	Square *cur;
	Square *next;
	for (int i = 0; i < (self->size * self->size); ++i) {
		cur = &self->data[i];
		next = &self->data_next[i];
		Square_init_from_dist(cur, self->rand_eng, self->dist);
		next->kind = cur->kind;
		next->eats = cur->eats;
		next->repRate = cur->repRate;
	}
}

void Grid_dealloc(Grid *self) {
	free(self->data);
	free(self->data_next);
	gsl_rng_free(self->rand_eng);
	free(self->dist);
	free(self);
	return;
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
		for (int j = 0; j < self->size; ++j) {
			cur = Grid_get_cur(self, i, j);
			next = Grid_get_next(self, i, j);
			next->kind = cur->kind;
			next->eats = cur->eats;
			next->repRate = cur->repRate;
			if (cur->kind == EMPTY) {
				numPrey1 = 0;
				prey1reproduces = false;
				numPrey2 = 0;
				prey2reproduces = false;
				for (int k = -1; k <= 1; ++k) {
					for (int l = -1; l <= 1; ++l) {
						if (Grid_get_cur(self, i + k, j + l)->kind == PREY1)
							++numPrey1;
						if (Grid_get_cur(self, i + k, j + l)->kind == PREY2)
							++numPrey2;
					}
				}
				if (gsl_ran_flat(self->rand_eng, 0.0, 1.0) < (1 - pow(1 - self->dist->repRate_prey1, numPrey1))) {
					prey1reproduces = true;
				}
				if (gsl_ran_flat(self->rand_eng, 0.0, 1.0) < (1 - pow(1 - self->dist->repRate_prey2, numPrey2))) {
					prey2reproduces = true;
				}
				if (prey1reproduces && prey2reproduces) {
					next->kind = (int) floor(gsl_ran_flat(self->rand_eng, 0.0, 1.0) * 2 + 1);
				}
				else if (prey1reproduces) {
					next->kind = PREY1;
				}
				else if (prey2reproduces) {
					next->kind = PREY2;
				}
			}
			else if (cur->kind == PREY1)  {
				opts_size = 0;
				Square *pred;
				for (int k = -1; k <= 1; ++k) {
					for (int l = -1 ; l <= 1; ++l) {
						pred = Grid_get_cur(self, i + k, j + l);
						if (pred->kind  == 3 && pred->eats <= 0.5 && gsl_ran_flat(self->rand_eng, 0.0, 1.0) < pred->repRate) {
							opts[opts_size] = pred;
							opts_size++;
						}
					}
				}
				if (opts_size > 0) {
					Square *opt = opts[(int) floor(gsl_ran_flat(self->rand_eng, 0.0, 1.0) * opts_size)];
					next->kind = PREDATOR;
					next->repRate = contain_to_0_1(opt->repRate + gsl_ran_gaussian(self->rand_eng, self->dist->mut_repRate));
					next->eats = contain_to_0_1(opt->eats + gsl_ran_gaussian(self->rand_eng, self->dist->mut_eats));
				}
			}
			else if (cur->kind == PREY2) {
				opts_size = 0;
				Square *pred;
				for (int k = -1; k <= 1; ++k) {
					for (int l = -1 ; l <= 1; ++l) {
						pred = Grid_get_cur(self, i + k, j + l);
						if (pred->kind  == 3 && pred->eats > 0.5 && gsl_ran_flat(self->rand_eng, 0.0, 1.0) < pred->repRate) {
							opts[opts_size] = pred;
							opts_size++;
						}
					}
				}
				if (opts_size > 0) {
					Square *opt = opts[(int) floor(gsl_ran_flat(self->rand_eng, 0.0, 1.0) * opts_size)];
					next->kind = PREDATOR;
					next->repRate = contain_to_0_1(opt->repRate + gsl_ran_gaussian(self->rand_eng, self->dist->mut_repRate));
					next->eats = contain_to_0_1(opt->eats + gsl_ran_gaussian(self->rand_eng, self->dist->mut_eats));
				}
			}
			else if (cur->kind == PREDATOR) {
				if (gsl_ran_flat(self->rand_eng, 0.0, 1.0) < self->dist->deathRate) {
					next->kind = 0;
				}
			}
		}
	}
	Grid_refresh(self);
}

Square * Grid_get_cur(Grid *self, int x, int y) {
	/* READONLY */
	return &self->data[((self->size * (x % self->size)) + (y % self->size))];
}

Square * Grid_get_next(Grid *self, int x, int y) {
	/* WRITEONLY */
	return &self->data_next[((self->size * (x % self->size)) + (y % self->size))];
}
