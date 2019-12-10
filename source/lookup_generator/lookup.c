 /**
 * File - 	lookup.c
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * Lookup Table Generator
 */

#include "lookup.h"

/*
 * Function - dac_lookup_init
 * Parameter - pointer to buffer
 * Brief 	- Fills up the buffer with lookup table values
 */
void dac_lookup_init(uint16_t* buffer)
{
    if(logger.Get_Log_Level() == lDebug)
    	logger.Log_Write(__func__, mDebug, "Initialization Lookup Table");

	for(uint8_t index = 0; index < 50; index++)
	{
		buffer[index] = (uint16_t)((sin(2*PI* (float) index/(float) 50) + 2) * (float) 4096 / 3.3);
	}
	logger.Log_Write(__func__, mStatus, "Lookup Table Initialized");
}
