# Thermostat 🌡️
Embedded Systems 2 Project, based on a simple Thermostat.

# Brief Description:
This project is meant to emulate the Operation of a Thermostat controlling an AC System. Temperature cooling and heating are simulated in C code since I do not have control of the outside temperature and would therefore be unable to test it out in different use cases. However, this would act as a proof of concept for a real cooling system controlled by a thermostat.

# Features
### Setting the Temperature 🌡️:
- The user can set the temperature to a range between 10ºC and 30ºC, just like you would on a real-world Thermostat.
- To have the AC adjust to that temperature, they hit the "Set" button.
  
### Turning off the Thermostat 🟡:
- There is an "Off" option which the user selects using the buttons and presses 'OK'.
  
### Running the Program on Your Machine 💻:
#### You Will Need:
- DE10-Standard: Terasic - SoC Platform - Cyclone

### Using the Tools.
- Get the executable file, labeled Thermostat, and copy it over onto the board, using FileZilla or any other FTP mechanism
- In your terminal on the board, access the file and give it permissions using 'chmod +x'.
- Then run the executable file in your terminal.
- Interface with the board to see the different functionalities. 