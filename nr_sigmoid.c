#include "nr.h"

float sigmoid(float x)
{
	return 1 / (1 + expf(-x));
}

float sigmoid_(float x)
{
	float fx = sigmoid(x);
	return fx * (1 - fx);
}

float f(float x)
{
	return sigmoid(x) - 0.5;
}

float f_(float x)
{
	return sigmoid_(x);
}

