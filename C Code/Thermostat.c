#include "Thermostat.h"
#include "House.h"
#include "Time.h"

#include <stdio.h>
#include <unistd.h>

#define TEMP_ADJUST_RATE 1 // degrees per interval

// Global variable to store the current state of the Thermostat
enum state state = HomeScreen;

void adjustHouseTemperatureStep()
	{
		// Adjust the temperature only if the thermostat is not off
		if (state == HomeScreen || state == SetTemp)
			{
				// Adjust the temperature by one degree towards the set temperature
				if (houseTemp < setTemp)
					{
						houseTemp += TEMP_ADJUST_RATE;
					}
				else if (houseTemp > setTemp)
					{
						houseTemp -= TEMP_ADJUST_RATE;
					}
			}
	}




