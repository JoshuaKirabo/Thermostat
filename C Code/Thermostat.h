#ifndef THERMOSTAT_H
#define THERMOSTAT_H

// Enum for Thermostat states
enum state
	{
		HomeScreen,
		SetTemp,
		Off,
	};

extern enum state state;
extern int setTemp;
extern int houseTemp;

void adjustHouseTemperature();
void thermostatControlLoop();

#endif
