/**
 * File - timestamp.c
 * Author - Atharva Nandanwar
 * Email - atharva.nandanwar@colorado.edu
 * Principles of Embedded Software
 * University of Colorado Boulder
 */

#include "timestamp.h"

// Global deciseconds count
uint32_t deciseconds = 0;

void tTimestamp_Init(void)
{
    xLoggerTimer = xTimerCreate("Logger Timer", (100/ portTICK_PERIOD_MS), pdTRUE,
    		(void *) 0, vLoggerTimerCallback);

	xTimerStart(xLoggerTimer, 0);
}

/**
 * tTimestamp_Get_Timestamp
 * Gets time stamp data
 * @return
 * 		returns a struct with timestamp information
 */
timestamp_t tTimestamp_Get_Timestamp(void)
{
	uint32_t temp;
	timestamp_t currentTime;
	currentTime.hours = deciseconds / 36000;
	temp = deciseconds % 36000;
	currentTime.minutes = temp / 600;
	temp = temp % 600;
	currentTime.seconds = temp / 10;
	currentTime.deciseconds = temp % 10;
	return currentTime;
}

static void vLoggerTimerCallback(TimerHandle_t xLoggerTimer)
{
	deciseconds++;
}
