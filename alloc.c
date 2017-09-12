#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>

#define COMMIT_STEP_SIZE 1024

void timer_start(clock_t *t0)
{
	*t0 = clock();
}

float timer_end(clock_t *t0, unsigned ops)
{
	float dt = (clock() - *t0) * 1.0f / CLOCKS_PER_SEC;
	float rate = ops / (dt + 1e-9);
	return rate;
}

void pattern(char *c, unsigned size, char init)
{
	clock_t t0;
	printf(" - Initialising to 0x%02hhx...\n", init);
	timer_start(&t0);
	memset(c, init, size);
	float speed_w = timer_end(&t0, size);

	printf(" - Validating...\n");
	timer_start(&t0);
	for (unsigned i = 0; i < size; i++) {
		if (c[i] != init) {
			printf("ERROR: 0x%02x != 0x%02x at address %p\n", c[i], init, c+i);
			getchar();
		}
	}
	float speed_r = timer_end(&t0, size);

	printf(" - Speed: r = %.2f MB/s, w = %.2f MB/s\n", speed_r / 1048576, speed_w / 1048576);
}

void test(unsigned size)
{
	printf("Size %ukB...\n", size >> 10);
	printf(" - Allocating...\n");
	char *c = malloc(size);
	if (!c) {
		printf("Error: malloc failed!\n");
		getchar();
		return;
	}
	for (unsigned i = 0; i < size; i += COMMIT_STEP_SIZE) {
		memset(c + i, 0, COMMIT_STEP_SIZE);
		if (!(i & 0xffff)) {
			printf("\r - Committed %d/%d kB @ %p...", i >> 10, size >> 10, (void *) &c[i]);
		}
	}
	printf("\r - Committed %d/%d kB...                    \n", size >> 10, size >> 10);
	printf(" - Locking...\n");
	if (mlockall(MCL_CURRENT | MCL_FUTURE) != 0) {
		printf("Warn: mlockall failed!\n");
	}
	pattern(c, size, 0x00);
	pattern(c, size, 0xff);
	pattern(c, size, 0xf0);
	pattern(c, size, 0x0f);
	pattern(c, size, 0xcc);
	pattern(c, size, 0x66);
	pattern(c, size, 0x33);
	pattern(c, size, 0x99);
	pattern(c, size, 0xaa);
	pattern(c, size, 0x55);
	printf(" - size, Freeing...\n");
	munlockall();
	free(c);
	printf("\n");
}

int main(int argc, char *argv[])
{
	while (1) {
		for (int e = 1; e <= 128; e <<= 1) {
			test(e << 20);
			test((e << 1 | e) << 19);
		}
	}
}
