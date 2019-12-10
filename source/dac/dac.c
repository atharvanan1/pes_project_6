/*
 * dac.c
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#include "dac.h"

void dac_init(void)
{
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mDebug, "DAC Initialization Started");
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;

	PORTE->PCR[30] |= PORT_PCR_MUX(0);

	DAC0->C0 |= DAC_C0_DACEN_MASK |		// Enable the DAC Module
			DAC_C0_DACTRGSEL_MASK |		// Select software trigger
			DAC_C0_DACRFS_MASK;			// Selecting VCCA

	DAC0->C1 |= DAC_C1_DACBFMD_MASK;
	DAC0->C2 = 0;
	logger.Log_Write(__func__, mStatus, "DAC Initialized");
}

void dac_out(uint16_t data)
{
	DAC0->DAT->DATH = (data & 0xF00) >> 8;
	DAC0->DAT->DATL = data & 0x0FF;
}
