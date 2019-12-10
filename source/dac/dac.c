 /**
 * File - 	dac.c
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * DAC Peripheral Functions
 */

#include "dac.h"

/*
 * dac_init
 * Initializes DAC Peripherals
 */
void dac_init(void)
{
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mDebug, "DAC Initialization Started");

	// Turn on Gating Clock
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;

	// Set up Port Pins
	PORTE->PCR[30] |= PORT_PCR_MUX(0);

	DAC0->C0 |= DAC_C0_DACEN_MASK |		// Enable the DAC Module
			DAC_C0_DACTRGSEL_MASK |		// Select software trigger
			DAC_C0_DACRFS_MASK;			// Selecting VCCA

	// Buffer Normal Mode
	DAC0->C1 |= DAC_C1_DACBFMD_MASK;

	// DAC Buffer Settings OFF
	DAC0->C2 = 0;
	logger.Log_Write(__func__, mStatus, "DAC Initialized");
}

/*
 * dac_out
 * Outputs value in DAC0
 */
void dac_out(uint16_t data)
{
	DAC0->DAT->DATH = (data & 0xF00) >> 8;
	DAC0->DAT->DATL = data & 0x0FF;
}
