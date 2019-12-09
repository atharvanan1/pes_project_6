/*
 * lookup.c
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#include "lookup.h"

void dac_lookup_init(uint16_t* buffer)
{
	for(uint8_t index = 0; index < 50; index++)
	{
		buffer[index] = (uint16_t)((sin(2*PI* (float) index/(float) 50) + 2) * (float) 4096 / 3.3);
	}
}
