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
uint8_t dma_flag = 0;
uint16_t buffer[50];
circular_buffer_t* adc_buffer = NULL;
uint16_t dsp_buffer[BUFFER_LENGTH];
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
    		NULL, (configMAX_PRIORITIES - 1), &DAC_Task_Handler);
    xTaskCreate(ADC_Task, "ADC Task", configMINIMAL_STACK_SIZE + 100, \
    		NULL, (configMAX_PRIORITIES - 3), &ADC_Task_Handler);
    xTaskCreate(DSP_Task, "DSP Task", configMINIMAL_STACK_SIZE + 500, \
    		NULL, (configMAX_PRIORITIES - 2), &DSP_Task_Handler);

    dac_lookup_init(buffer);
    dac_init();
    adc_buffer = cb_init_buffer(BUFFER_LENGTH);
    adc_init();
    for(uint8_t index = 0; index < BUFFER_LENGTH; index++)
    	dsp_buffer[index] = 0;
    dma_init();

//    uint16_t data_data = 0;
//    uint16_t* data = &data_data;
//    for(uint8_t index = 0; index < 64; index++)
//    {
//    	cb_add_item(adc_buffer, 3456+index);
//    }
//    for(uint8_t index = 0; index < 64; index++)
//    {
//    	cb_remove_item(adc_buffer, data);
//    	logger.Log_Write(__func__, mStatus, "Value %d", *data);
//    }
//    logger.Log_Write(__func__, mStatus, "Size %d", sizeof(adc_buffer->pointer[0]));
//    dma_transfer(adc_buffer->pointer, dsp_buffer);
//    while(dma_flag == 0);
//    for(uint8_t index = 0; index < 64; index++)
//    {
//       	logger.Log_Write(__func__, mStatus, "Value %d", dsp_buffer[index]);
//    }

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
		if(logger.Get_Log_Level() == lDebug)
			logger.Log_Write(__func__, mDebug, "DAC Values %d", *(buffer + dac_index));

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
		for(uint8_t index = 0; index < BUFFER_LENGTH; index++)
		{
			volatile uint16_t value = *(dsp_buffer + index);
			mean += value;
			if(max < value)
			{
				max = value;
			}
			if(min > value)
			{
				max = value;
			}
		}
		temp_mean = mean / BUFFER_LENGTH;
		mean = temp_mean * 100;
		for(uint8_t index = 0; index < BUFFER_LENGTH; index++)
		{
			volatile uint16_t value = *(dsp_buffer + index);
			temp_sd += pow((value - temp_mean), 2);
//			logger.Log_Write(__func__, mStatus, "ADC Value - %f", (float) value * 3.3 / 65535);
		}
		temp_sd = temp_sd / BUFFER_LENGTH;
		standard_deviation = temp_sd * 100;
//		logger.Log_Write(__func__, mStatus, "Max - %d", max);
//		logger.Log_Write(__func__, mStatus, "Min - %d", min);
//		logger.Log_Write(__func__, mStatus, "Mean - %f", temp_mean);
//		logger.Log_Write(__func__, mStatus, "SD - %f", temp_sd);

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
