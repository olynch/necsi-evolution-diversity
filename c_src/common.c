#include "common.h"

float contain_to_0_1(float num) {
	if (num > 1.0) {
		return 1.0;
	}
	else if (num < 0.0) {
		return 0.0;
	}
	else {
		return num;
	}
}
