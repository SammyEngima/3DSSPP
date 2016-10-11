#pragma once
#include "stdafx.h"

class TestTimer
{
public:
	TestTimer(void);
	~TestTimer(void);

	double SecondsSinceLast();

private:
	LARGE_INTEGER last;

	LARGE_INTEGER freq;
};
