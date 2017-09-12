#include <math.h>
#include <stdlib.h>
#include <stdio.h>

float f(float x);
float f_(float x);

static inline float nr(float x)
{
	return x - f(x) / f_(x);
}

static inline float rnd()
{
	return rand() * 2.0f / RAND_MAX - 1;
}

const float thresh = 1e-3;

const int e_max = 6;

int main(int argc, char *argv[])
{
	while (1) {
		float x = rnd();
		printf("init: %.4f\t", x);
		for (int e = 0, t = 1, u = 0; e <= e_max; e++, t *= 10) {
			for (; u < t; u++) {
				x = nr(x);
			}
			printf("10^%d: %.4f\t", e, x);
		}
		printf("\n");
		if (fabsf(x) > thresh) {
			printf("ERROR: |x| = %.4f > %e after 10^%d iterations\n", fabsf(x), thresh, e_max);
			getchar();
		}
	}
}
