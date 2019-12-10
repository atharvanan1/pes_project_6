 /**
 * File - 	timestamp.c
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * Timestamp Implementation
 */

#include "timestamp.h"

// Global deciseconds count
uint32_t deciseconds = 0;

/*
 * vTimestamp_Init
 * Initializes Time Stamp
 */
void vTimestamp_Init(void)
{
	TimerHandle_t xLoggerTimer;
    xLoggerTimer = xTimerCreate("Logger Timer", (100/ portTICK_PERIOD_MS), pdTRUE,
    		(void *) 0, vLoggerTimerCallback);

	xTimerStart(xLoggerTimer, 0);
}

/**
 * tTimestamp_Get_Timestamp
 * Gets Time Stamp data
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

/*
 * vLoggerTimerCallback
 * Timer callback function
 * @param
 * 		xLoggerTimer - timer handle for logger timer
 */
void vLoggerTimerCallback(TimerHandle_t xLoggerTimer)
{
	deciseconds++;
}
