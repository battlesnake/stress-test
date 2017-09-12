#include "nr.h"

float f(float x)
{
	return sinf(x) + sinf(3 * x) / 3;
}

float f_(float x)
{
	return cosf(x) + cosf(3 * x);
}
