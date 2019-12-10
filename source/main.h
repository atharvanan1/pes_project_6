 /**
 * File - 	main.h
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * Main file
 */

#ifndef MAIN_H_
#define MAIN_H_
// Library includes
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

// Program Includes
#include "lookup.h"
#include "dac.h"
#include "adc.h"
#include "dma.h"
#include "led_control.h"
#include "logger.h"
#include "circular_buffer.h"

void DAC_Task(void* parameters);
void ADC_Task(void* parameters);
void DSP_Task(void* parameters);
char* float_string(uint32_t value);

#endif /* MAIN_H_ */
