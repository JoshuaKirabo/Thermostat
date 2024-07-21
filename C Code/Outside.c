// Outside.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Simulating an outside temperature
// We will go between 10°C and 30°C

int getOutsideTemperature()
	{
		srand(time(NULL));
		return (rand() % 21) + 10;
	}
