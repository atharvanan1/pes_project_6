/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    pes_project_6.c
 * @brief   Application entry point.
 */
#include "main.h"
;/* TODO: insert other definitions and declarations here. */
#define DACTASKPERIOD (100 / portTICK_PERIOD_MS)
#define ADCTASKPERIOD (100 / portTICK_PERIOD_MS)

uint8_t run_number = 0;
uint16_t buffer[50];
circular_buffer_t* adc_buffer = NULL;
uint16_t dsp_buffer[64];
uint8_t dac_index = 0;
TaskHandle_t DAC_Task_Handler;
TaskHandle_t ADC_Task_Handler;
TaskHandle_t DSP_Task_Handler;
TimerHandle_t xLoggerTimer;

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    logger.Init();
    LED_Init();
#ifdef DEBUG_CODE
    logger.Set_Log_Level(lDebug);
#else
    logger.Set_Log_Level(lNormal);
#endif

    xTaskCreate(DAC_Task, "DAC Task", configMINIMAL_STACK_SIZE + 100, \
    		NULL, 2, &DAC_Task_Handler);
    xTaskCreate(ADC_Task, "ADC Task", configMINIMAL_STACK_SIZE + 100, \
    		NULL, 2, &ADC_Task_Handler);
    xTaskCreate(DSP_Task, "DSP Task", configMINIMAL_STACK_SIZE + 500, \
    		NULL, 2, &DSP_Task_Handler);

    dac_lookup_init(buffer);
    dac_init();
    adc_buffer = cb_init_buffer(64);
    adc_init();
    for(uint8_t index = 0; index < 64; index++)
    	dsp_buffer[index] = 0;
    dma_init();

    vTaskStartScheduler();
    while(1);
}

void DAC_Task(void* parameters)
{
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	for (;;)
	{
		dac_out(*(buffer + dac_index));
		dac_index++;
		if(dac_index == 50)
			dac_index = 0;

		Toggle_LED(Green);

		vTaskDelayUntil(&PreviousWakeTime, DACTASKPERIOD);
	}
}

void ADC_Task(void* parameters)
{
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	for (;;)
	{
		if(CB_buffer_full == cb_add_item(adc_buffer, adc_value()))
		{
			run_number++;
			logger.Log_Write(__func__, mStatus, "Run Number %d", run_number);
			logger.Log_Write(__func__, mStatus, "DMA Transfer Started");
			dma_transfer(adc_buffer->pointer, dsp_buffer);
			cb_empty_buffer(adc_buffer);
		}
		vTaskDelayUntil(&PreviousWakeTime, ADCTASKPERIOD);
	}
}

void DSP_Task(void* parameters)
{
	for (;;)
	{
		uint32_t mean = 0;
		uint16_t max = 0;
		uint16_t min = 0xffff;
		uint32_t standard_deviation = 0;
		float temp_mean = 0;
		float temp_sd = 0;
		for(uint8_t index = 0; index < 64; index++)
		{
			volatile uint16_t value = *(dsp_buffer + index);
			mean += value;
			if(max < value)
			{
				max = value;
			}
			if(min > value)
			{
				min  = value;
			}
		}
		temp_mean = mean / 64;
		mean = temp_mean * 100;

		for(uint8_t index = 0; index < 64; index++)
		{
			volatile uint16_t value = *(dsp_buffer + index);
			temp_sd += pow((value - temp_mean), 2);
		}
		temp_sd = temp_sd / 64;
		temp_sd = sqrt(temp_sd);
		standard_deviation = temp_sd * 100;

		char* temp = NULL;
		logger.Log_Write(__func__, mStatus, "Mean - %s", temp = float_string(mean));
		if(mean)
			free(temp);
		logger.Log_Write(__func__, mStatus, "Max - %d", max);
		logger.Log_Write(__func__, mStatus, "Min - %d", min);
		logger.Log_Write(__func__, mStatus, "SD - %s", temp = float_string(standard_deviation));
		if(mean)
			free(temp);

		if(run_number == 5)
		{
			vTaskDelete(DAC_Task_Handler);
			vTaskDelete(ADC_Task_Handler);
			vTaskDelete(DSP_Task_Handler);
		}

		vTaskSuspend(NULL);
	}
}

void DMA_Callback(dma_handle_t *handle, void *param)
{
	logger.Log_Write(__func__, mStatus, "DMA Transfer Finished");
	xTaskResumeFromISR(DSP_Task_Handler);
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE(1);
}

char* float_string(uint32_t value)
{
	uint32_t temp = value;
	uint8_t buffer[12];
	uint8_t index = 0;
	if(value == 0)
	{
		return "0.00";
	}
	while(temp != 0)
	{
		buffer[index] = temp % 10;
		temp = temp / 10;
		index++;
	}

	char* char_buffer = (char *) malloc(sizeof(char) * 13);
	uint8_t char_index = 0;
	index--;
	while(index - char_index >= 2)
	{
		*(char_buffer + char_index) = buffer[index - char_index] + '0';
		char_index++;
	}

	if(value < 100)
	{
		char_buffer[0] = '0';
		char_index++;
	}

	*(char_buffer + char_index) = '.';
	char_index++;
	*(char_buffer + char_index) = buffer[1] + '0';
	char_index++;
	*(char_buffer + char_index) = buffer[0] + '0';
	char_index++;
	*(char_buffer + char_index) = 0;
	return char_buffer;
}
