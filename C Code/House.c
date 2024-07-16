// House.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define constants
#define ROOM_TEMPERATURE 22.0
#define MAX_DIFF 10.0
#define MAX_TEMP_DIFF 40.0

// Declaration of the function from Outside.c
int getOutsideTemperature();

// Function to calculate temperature difference factor
double calculateTempDifference(double outsideTemp)
	{
		double tempDiff = fabs(outsideTemp - ROOM_TEMPERATURE);
		return MAX_DIFF * (1 - (tempDiff / MAX_TEMP_DIFF));
	}

// Function to estimate inside temperature
double estimateInsideTemp(double outsideTemp)
	{
		double changeInTemperature = calculateTempDifference(outsideTemp);

		if (outsideTemp > ROOM_TEMPERATURE)
			{
				return outsideTemp - changeInTemperature;
			}
		else if (outsideTemp < ROOM_TEMPERATURE)
			{
				return outsideTemp + changeInTemperature;
			}
		else
			{
				return outsideTemp; // When outside temperature is at room temperature
			}
	}

// Function to get the house temperature based on the outside temperature
int getHouseTemperature()
	{
		int outsideTemp = getOutsideTemperature();
		double insideTemp = estimateInsideTemp(outsideTemp);
		return (int)insideTemp;
	}
