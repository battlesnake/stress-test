#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float rnd()
{
	return rand() * 1.0f / RAND_MAX * 2 - 1;
}

float dot(const float *a, const float *b, unsigned length)
{
	float c = 0;
	while (length--) {
		c += *a++ * *b++;
	}
	return c;
}

void fill_with_crap(float *a, unsigned length, unsigned period)
{
	for (unsigned i = 0; i < length && i < period; i++) {
		a[i] = rnd();
	}

	for (unsigned i = period; i < length; i+=period) {
		memcpy(a + i, a, period * sizeof(float));
	}

	unsigned i = length / period * period;
	unsigned j = length % period;

	if (i > 0) {
		memcpy(a + i, a, j * sizeof(float));
	}
}

void test(unsigned length, unsigned repeats)
{
	printf("Testing vector length %d, %d times...\n", length, repeats);

	printf(" - Allocating %dkB...\n", (length * sizeof(float) * 2) >> 10);

	float *a = malloc(length * sizeof(*a));
	float *b = malloc(length * sizeof(*b));

	printf(" - Filling arrays with random crap...\n");

	fill_with_crap(a, length, 1000);
	fill_with_crap(b, length, 1000);

	float res = dot(a, b, length);

	for (unsigned k = 0; k < repeats; k++) {
		if (!(k & 0x3fff)) {
			printf("\r - Stressing %.1f%%...", k * 100.0f / repeats);
		}
		float c = dot(a, b, length);
		if (c != res) {
			printf("\n");
			printf("Error: incorrect result\n");
			getchar();
		}
	}
	printf("\n");

	printf(" - Freeing buffers\n");

	free(b);
	free(a);

	printf("\n");
}

int main(int argc, char *argv[])
{
	while (1) {
		test(1, 10000000);
		test(100, 100000);
		test(1000, 10000);
		test(10000, 1000);
		test(1000000, 10);
		test(10000000, 1);
	}
}
