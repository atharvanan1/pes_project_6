/*
 * adc.h
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>
#include "MKL25Z4.h"

void adc_init(void);
uint16_t adc_value(void);

#endif /* ADC_H_ */
