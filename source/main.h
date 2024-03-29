/*
 * main.h
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

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

#endif /* MAIN_H_ */
