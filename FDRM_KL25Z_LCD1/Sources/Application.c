/*
 * Application.c
 *
 *  Created on: 8 Nov, 2015
 *      Author: kzw
 */
#include "LED.h"
#include "WAIT1.h"
#include "ILI9325.h"

//void Application_Run();

void Application_Run()
{
	uint16_t id;
	int16_t height;
	int16_t width;
	ILI9325Reset();
	WAIT1_Waitms(1000);

	id = ILI9325ReadID();
	ILI9325Begin(id);
	height = getHeight();
	width = getWidth();

	for(;;)
	{
		GPIO_DRV_TogglePinOutput(LEDG);


//		ILI9325FillScreen(RED);
//		WAIT1_Waitms(1000);
//		ILI9325FillScreen(YELLOW);
//		WAIT1_Waitms(1000);
//		drawLine(0, 0, 100, 100,BLACK);
//		WAIT1_Waitms(1000);
		ILI9325FillScreen(BLACK);
		write('H');
//		drawRect(0, 0, 150, 150, RED);
		drawCircle(width/2, height/2, 80, WHITE);
		WAIT1_Waitms(2000);


    /*
	ILI9328_Init(1,1);
	WAIT1_Waitms(10);
	LCD_Print( "Hello, World!", CENTER, 50);
	WAIT1_Waitms(1000);
	*/
	}
/*
	for(;;)
	{
	  GPIO_DRV_TogglePinOutput(LEDR);
	  WAIT1_Waitms(1000);
	  GPIO_DRV_TogglePinOutput(LEDG);
	  WAIT1_Waitms(1000);
	  GPIO_DRV_TogglePinOutput(LEDB);
	  WAIT1_Waitms(1000);
	}
	*/




}
