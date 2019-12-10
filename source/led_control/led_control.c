/**
 * File - led_control.c
 * Author - Atharva Nandanwar
 * Email - atharva.nandanwar@colorado.edu
 * Principles of Embedded Software
 * University of Colorado Boulder
 */
#include "led_control.h"

/**
 * LED_Init
 * Initialization of LEDs
 */
void LED_Init(void)
{
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mDebug, "Starting LED Initialization");

	// Set up clock for Port peripheral
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	// Set mux for each port pins
	PORTB->PCR[18] = PORT_PCR_MUX(1);
	PORTB->PCR[19] = PORT_PCR_MUX(1);
	PORTD->PCR[1] = PORT_PCR_MUX(1);

	// Set data directions, and turn the LEDs off
	GPIOB->PDDR |= RED_LED | GREEN_LED;
	GPIOD->PDDR |= BLUE_LED;
	Turn_Off_LED(Red);
	Turn_Off_LED(Green);
	Turn_Off_LED(Blue);
}

/**
 * Turn_On_Only_LED
 * Turns on only specified color LED
 * @param
 * 		LED - color of LED
 */
void Turn_On_LED(led_color_t LED)
{
	switch(LED)
	{
	case Red:
		GPIOB->PCOR |= RED_LED;			// Turn On
		break;
	case Green:
		GPIOB->PCOR |= GREEN_LED;		// Turn On
		break;
	case Blue:
		GPIOD->PCOR |= BLUE_LED;		// Turn On
		break;
	}
}

/**
 * Turn_Off_LEDs
 * turns off all LEDs
 */
void Turn_Off_LED(led_color_t LED)
{
	switch(LED)
	{
	case Red:
		GPIOB->PSOR |= RED_LED;			// Turn On
		break;
	case Green:
		GPIOB->PSOR |= GREEN_LED;		// Turn On
		break;
	case Blue:
		GPIOD->PSOR |= BLUE_LED;		// Turn On
		break;
	}
}

void Toggle_LED(led_color_t LED)
{
	switch(LED)
	{
	case Red:
		GPIOB->PTOR |= RED_LED;			// Turn On
		break;
	case Green:
		GPIOB->PTOR |= GREEN_LED;		// Turn On
		break;
	case Blue:
		GPIOD->PTOR |= BLUE_LED;		// Turn On
		break;
	}
}
