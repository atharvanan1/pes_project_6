 /**
 * File - 	adc.c
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * ADC Peripheral Implementation
 */

#include "adc.h"

/*
 * adc_init
 * Initializes ADC Peripherals
 */
void adc_init(void)
{
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mStatus, "ADC Initialization Started");

	// Turn on ADC Gating Clock
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	// Port E Pin 20 Setting
	PORTE->PCR[20] |= PORT_PCR_MUX(0);

	// ADC Configuration Settings
	/*
	 * Normal Power Configuration
	 * Clock Divide Set at 8
	 * Long Sample Time
	 * 16-bit Conversion Mode
	 * Bus Clock / 2 as Clock Source
	 */
	ADC0->CFG1 = 0x7D;


	ADC0->CFG2 |= ADC_CFG2_ADHSC(1);

	/*
	 * Setting up Conversion Trigger as software
	 * Disable Compare Function
	 */
	ADC0->SC2 = 0;

	/*
	 * Turn Off Calibration
	 * Hardware Average Function On
	 * 4 Samples for Averaging
	 */
	ADC0->SC3 |= ADC_SC3_ADCO_MASK |
			ADC_SC3_AVGE_MASK |
			ADC_SC3_AVGS(0x00);

	logger.Log_Write(__func__, mStatus, "ADC Initialized");
}

/*
 * adc_value
 * Starts ADC conversion, and waits till the conversion is over
 * @return
 * 		returns ADC value
 */
uint16_t adc_value(void)
{
	// Write to SC1 to start conversion
	ADC0->SC1[0] = 0;

	// Wait for conversion to complete
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));

	// Return ADC value
	return ADC0->R[0];
}
