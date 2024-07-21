#include "terasic_os_includes.h"
#include "LCD_Lib.h"
#include "lcd_graphic.h"
#include "font.h"
#include "./address_map_arm.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Thermostat.h"
#include "House.h"

#define DEBOUNCE_TIME 100000
#define DOT_THRESHOLD 100
#define DASH_THRESHOLD 300

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

// Heating and cooling limits
#define MIN_TEMP 10
#define MAX_TEMP 30

volatile unsigned int *JP1_ptr; // Pointer to the FPGA

int highlightedOption; // To help manage options within states
int houseTemp; // Temperature inside the house
int setTemp = 20; // This is the default AC Temperature before anyone tampers with it

int main()
	{
		// Initialize Value
		void *LW_virtual;
		void *virtual_base;

		int fd;

		if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 )
			{
				printf( "ERROR: could not open \"/dev/mem\"...\n" );
				return( 1 );
			}

		if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
			{
				printf( "ERROR: mmap() failed...\n" );
				close( fd );
				return( 1 );
			}

		if ((virtual_base = map_physical (fd, HW_REGS_BASE, HW_REGS_SPAN)) == NULL)
			{
				printf( "ERROR: mmap() failed...\n" );
				close( fd );
				return( 1 );
			}

		JP1_ptr = (unsigned int*)(LW_virtual + JP1_BASE);
		*(JP1_ptr + 1) = 0x0000000F;



		LCD_CANVAS LcdCanvas;
		LcdCanvas.Width = LCD_WIDTH;
		LcdCanvas.Height = LCD_HEIGHT;
		LcdCanvas.BitPerPixel = 1;
		LcdCanvas.FrameSize = LcdCanvas.Width * LcdCanvas.Height / 8;
		LcdCanvas.pFrame = (void *)malloc(LcdCanvas.FrameSize);

		if (LcdCanvas.pFrame == NULL)
			{
				printf("failed to allocate lcd frame buffer\r\n");
			}
		else
			{
				LCDHW_Init(virtual_base);
				LCDHW_BackLight(true); // turn on LCD backlight

				LCD_Init();

				// clear screen
				DRAW_Clear(&LcdCanvas, LCD_WHITE);

				// demo font
				DRAW_Refresh(&LcdCanvas);
				free(LcdCanvas.pFrame);
			}

		// Upon entry to the app, default state is HomeScreen
		state = HomeScreen;

		DRAW_Clear(&LcdCanvas, LCD_WHITE);

		// Selecting Rectangle
		//DRAW_Rect(&LcdCanvas, 25,48, LcdCanvas.Width-11, LcdCanvas.Height-4, LCD_BLACK);
	    DRAW_Rect(&LcdCanvas, 0,0, LcdCanvas.Width-1, LcdCanvas.Height-1, LCD_BLACK);
		DRAW_PrintString(&LcdCanvas, 19, 6, "Set To: 20°C", LCD_BLACK, &font_16x16);
		//DRAW_PrintString(&LcdCanvas, 10, 5+16, "Temperature: 209C", LCD_BLACK, &font_16x16);
	    DRAW_PrintString(&LcdCanvas, 50, 5+20, "Set", LCD_BLACK, &font_16x16);
	    DRAW_PrintString(&LcdCanvas, 50, 5+35, "Off", LCD_BLACK, &font_16x16);
	    DRAW_Refresh(&LcdCanvas);



		volatile int *KEY_ptr = (unsigned int*)(LW_virtual + KEY_BASE);

		int prev_key_value = *KEY_ptr;
		int buttonValue = *KEY_ptr;

		unsigned int last_adjustment_time = 0;
		const unsigned int adjustment_interval = 5; // seconds

		while (1)
			{
		        buttonValue = *KEY_ptr;
		        if(buttonValue!= prev_key_value)
					{
						// OK Button
						if (buttonValue == 1)
							{
								printf("OK\n");

								if(state == HomeScreen)
									{
										if(highlightedOption == 1)
											{
												state = SetTemp;

												DRAW_Clear(&LcdCanvas, LCD_WHITE);
												DRAW_Rect(&LcdCanvas, 0,0, LcdCanvas.Width-1, LcdCanvas.Height-1, LCD_BLACK);

												DRAW_Rect(&LcdCanvas, 50,20, LcdCanvas.Width-60, LcdCanvas.Height-30, LCD_BLACK);

												DRAW_PrintString(&LcdCanvas, 30, 5, "Set Temp", LCD_BLACK, &font_16x16);
												char tempBuffer[20];
												snprintf(tempBuffer, sizeof(tempBuffer), "%d°C", setTemp); // Format the temperature string
												DRAW_PrintString(&LcdCanvas, 52, 20, tempBuffer, LCD_BLACK, &font_16x16); // Display the temperature string

								 				DRAW_Refresh(&LcdCanvas);
											}
										if(highlightedOption == 2)
											{
												state = Off;

											    DRAW_Clear(&LcdCanvas, LCD_WHITE);
											    DRAW_Rect(&LcdCanvas, 0, 0, LcdCanvas.Width - 1, LcdCanvas.Height - 1, LCD_BLACK);

											    // Get the actual house temperature
											    houseTemp = getHouseTemperature();
											    char tempBuffer[20];

											    // Format the temperature string
											    snprintf(tempBuffer, sizeof(tempBuffer), "Temp: %d°C", houseTemp);

											    DRAW_PrintString(&LcdCanvas, 20, 6, tempBuffer, LCD_BLACK, &font_16x16); // Display the temperature string
											    DRAW_PrintString(&LcdCanvas, 30, 5 + 20, "AC is off", LCD_BLACK, &font_16x16);

											    DRAW_Refresh(&LcdCanvas);
											}
									}
								else if(state == Off)
									{
										state = HomeScreen;
										DRAW_Clear(&LcdCanvas, LCD_WHITE);
										DRAW_Rect(&LcdCanvas, 0, 0, LcdCanvas.Width - 1, LcdCanvas.Height - 1, LCD_BLACK);

										char tempBuffer[20];
										snprintf(tempBuffer, sizeof(tempBuffer), "Set To: %d°C", setTemp); // Format the temperature string

				    					DRAW_PrintString(&LcdCanvas, 19, 6, tempBuffer, LCD_BLACK, &font_16x16); // Display the current temperature
										DRAW_PrintString(&LcdCanvas, 50, 5 + 20, "Set", LCD_BLACK, &font_16x16);
						      			DRAW_PrintString(&LcdCanvas, 50, 5 + 35, "Off", LCD_BLACK, &font_16x16);
										DRAW_Refresh(&LcdCanvas);
									}
								else if(state == SetTemp)
									{
										state = HomeScreen;

										DRAW_Clear(&LcdCanvas, LCD_WHITE);
										DRAW_Rect(&LcdCanvas, 0, 0, LcdCanvas.Width - 1, LcdCanvas.Height - 1, LCD_BLACK);

										char tempBuffer[20];
										snprintf(tempBuffer, sizeof(tempBuffer), "Set To: %d°C", setTemp); // Format the temperature string

										DRAW_PrintString(&LcdCanvas, 19, 6, tempBuffer, LCD_BLACK, &font_16x16); // Display the current temperature
										DRAW_PrintString(&LcdCanvas, 50, 5 + 20, "Set", LCD_BLACK, &font_16x16);
										DRAW_PrintString(&LcdCanvas, 50, 5 + 35, "Off", LCD_BLACK, &font_16x16);
										DRAW_Refresh(&LcdCanvas);

										// call some code from Thermostat.c to start affecting the current house temperature value
									}


								// Updating the 7 segment display
								*JP1_ptr = houseTemp;

							}
						// UP Button
						else if (buttonValue == 8)
							{
								printf("UP\n");

								if(state == HomeScreen)
									{
										DRAW_Clear(&LcdCanvas, LCD_WHITE);
										DRAW_Rect(&LcdCanvas, 49,27, LcdCanvas.Width-50, LcdCanvas.Height-25, LCD_BLACK); // Selecting Rectangle

										DRAW_Rect(&LcdCanvas, 0,0, LcdCanvas.Width-1, LcdCanvas.Height-1, LCD_BLACK);
										DRAW_PrintString(&LcdCanvas, 19, 6, "Set To: 20°C", LCD_BLACK, &font_16x16);
										DRAW_PrintString(&LcdCanvas, 50, 5+20, "Set", LCD_BLACK, &font_16x16);
										DRAW_PrintString(&LcdCanvas, 50, 5+35, "Off", LCD_BLACK, &font_16x16);
										DRAW_Refresh(&LcdCanvas);

										highlightedOption = 1;
									}
								else if(state == SetTemp)
									{
										if (setTemp < MAX_TEMP)
											{
												// Decrease the value to set the AC to when down button is pressed
												setTemp++;

												// Update the displayed temperature
												char tempBuffer[20];
												snprintf(tempBuffer, sizeof(tempBuffer), "%d°C", setTemp); // Format the temperature string

												DRAW_Clear(&LcdCanvas, LCD_WHITE);
												DRAW_Rect(&LcdCanvas, 0, 0, LcdCanvas.Width - 1, LcdCanvas.Height - 1, LCD_BLACK);
												DRAW_Rect(&LcdCanvas, 50, 20, LcdCanvas.Width - 60, LcdCanvas.Height - 30, LCD_BLACK);
												DRAW_PrintString(&LcdCanvas, 52, 20, tempBuffer, LCD_BLACK, &font_16x16); // Display the temperature string
												DRAW_PrintString(&LcdCanvas, 30, 5, "Set Temp", LCD_BLACK, &font_16x16);
												DRAW_Refresh(&LcdCanvas);
											}
									}
							}
						// DOWN Button
						else if (buttonValue == 4)
							{
								printf("DOWN\n");

								if(state == HomeScreen)
									{
										DRAW_Clear(&LcdCanvas, LCD_WHITE);
										DRAW_Rect(&LcdCanvas, 49,40, LcdCanvas.Width-50, LcdCanvas.Height-11, LCD_BLACK); // Selecting Rectangle

										DRAW_Rect(&LcdCanvas, 0,0, LcdCanvas.Width-1, LcdCanvas.Height-1, LCD_BLACK);
										DRAW_PrintString(&LcdCanvas, 19, 6, "Set To: 20°C", LCD_BLACK, &font_16x16);
										DRAW_PrintString(&LcdCanvas, 50, 5+20, "Set", LCD_BLACK, &font_16x16);
										DRAW_PrintString(&LcdCanvas, 50, 5+35, "Off", LCD_BLACK, &font_16x16);
										DRAW_Refresh(&LcdCanvas);

										highlightedOption = 2;
									}
								else if(state == SetTemp)
									{
										if (setTemp > MIN_TEMP)
											{
												// Decrease the value to set the AC to when down button is pressed
												setTemp--;

												// Update the displayed temperature
												 char tempBuffer[20];
												 snprintf(tempBuffer, sizeof(tempBuffer), "%d°C", setTemp); // Format the temperature string

												 DRAW_Clear(&LcdCanvas, LCD_WHITE);
												 DRAW_Rect(&LcdCanvas, 0, 0, LcdCanvas.Width - 1, LcdCanvas.Height - 1, LCD_BLACK);
												 DRAW_Rect(&LcdCanvas, 50, 20, LcdCanvas.Width - 60, LcdCanvas.Height - 30, LCD_BLACK);
												 DRAW_PrintString(&LcdCanvas, 52, 20, tempBuffer, LCD_BLACK, &font_16x16); // Display the temperature string
												 DRAW_PrintString(&LcdCanvas, 30, 5, "Set Temp", LCD_BLACK, &font_16x16);
												 DRAW_Refresh(&LcdCanvas);
											}
									}
							}
						// RIGHT Button
						else if (buttonValue == 2)
							{
								printf("RIGHT");
							}

						// Update Previous button value
					 	prev_key_value = *KEY_ptr;
					}

		        // Periodically check and adjust the house temperature
		        unsigned int current_time = time(NULL);
		        if (current_time - last_adjustment_time >= adjustment_interval)
		        	{
		        		// Check if houseTemp is not equal to setTemp
		                if (houseTemp != setTemp)
		                	{
		                    	adjustHouseTemperatureStep();
		                        last_adjustment_time = current_time;
		                    }
		            }
			}

		free(LcdCanvas.pFrame);

	 	 // clean up our memory mapping and exit
	 	 if( munmap( virtual_base, HW_REGS_SPAN ) != 0 )
	 	 	{
	 		 	printf( "ERROR: munmap() failed...\n" );
				close( fd );
				return( 1 );
			}


		close( fd );
		return( 0 );
	}



