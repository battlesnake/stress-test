#include "nr.h"

float f(float x)
{
	return tanf(x);
}

float f_(float x)
{
	float c = cosf(x);
	return 1 / (c * c);
}
