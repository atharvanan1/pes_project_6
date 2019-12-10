/*
 * adc.c
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#include "adc.h"

void adc_init(void)
{
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	PORTE->PCR[20] |= PORT_PCR_MUX(0);

	ADC0->CFG1 = 0;
	ADC0->CFG1 = 0x7D;

	ADC0->CFG2 = 0;

	ADC0->SC2 = 0;

	ADC0->SC3 |= ADC_SC3_ADCO_MASK |
			ADC_SC3_AVGE_MASK |
			ADC_SC3_AVGS(0x00);
}

uint16_t adc_value(void)
{
	ADC0->SC1[0] = 0;
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
	return ADC0->R[0];
}
