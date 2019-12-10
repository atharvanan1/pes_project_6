 /**
 * File - 	dac.h
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * DAC Peripheral Functions
 */

#ifndef DAC_DAC_H_
#define DAC_DAC_H_

// Include Files
#include <stdint.h>
#include "MKL25Z4.h"
#include "logger.h"

// Prototype Functions
void dac_init(void);
void dac_out(uint16_t data);

#endif /* DAC_DAC_H_ */
