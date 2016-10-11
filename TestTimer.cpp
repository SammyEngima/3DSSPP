#include "TestTimer.h"

TestTimer::TestTimer(void)
{
	QueryPerformanceCounter(&last);
	QueryPerformanceFrequency(&freq);
}

TestTimer::~TestTimer(void)
{
}

double TestTimer::SecondsSinceLast() {
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	double time = 1.0*(current.QuadPart - last.QuadPart) / freq.LowPart;
	last = current;
	return time;
}
