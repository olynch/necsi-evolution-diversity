#include "square.h"
#include <stdio.h>

int main(int argc, char **argv) {
	Square *test;
	printf("*** BEGINNING TESTING ****");
	printf("*** TESTING Square_from_args ****\n");
	test = Square_from_args(3, 0.4f, 0.3f);
	printf("test->kind: %i, test->eats: %f, test->repRate: %f\n", test->kind, test->eats, test->repRate);
	printf("*** TEST COMPLETED\n");

	printf("*** TESTING Square_from_dist ****");
	ProbDist *dist = malloc(sizeof ProbDist);
	dist->size = 10;
	dist->deathRate = 0.5f;
	dist->dist_prey1 = 0.2f;
	dist->dist_prey2 = 0.25f;
	dist->dist_predator = 0.1f;
	dist->repRate_prey1 = 0.4f;
	dist->repRate_prey2 = 0.3f;
	dist->repRate_predator = 0.1f;
	dist->mut_repRate = 0.04f;
	dist->mut_eats = 0.05f;
	for (int i = 0; i < 10; ++i) {
		test = Square_from_dist(dist);
		printf("test->kind: %i, test->eats: %f, test->repRate: %f\n", test->kind, test->eats, test->repRate);
	}
	printf("*** TEST COMPLETED ****\n");

	printf("*** ALL TESTS FINISHED ****\n");
	return 0;
}
