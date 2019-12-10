 /**
 * File - 	adc.h
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * ADC Peripheral Functions
 */

#ifndef ADC_H_
#define ADC_H_

// Include Files
#include <stdint.h>
#include "MKL25Z4.h"
#include "logger.h"

// Prototype Functions
void adc_init(void);
uint16_t adc_value(void);

#endif /* ADC_H_ */
