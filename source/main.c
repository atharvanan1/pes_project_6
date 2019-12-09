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

uint16_t buffer[50];
uint8_t dac_index = 0;
uint32_t deciseconds = 0;
TaskHandle_t DAC_Task_Handler = NULL;
/*
 * @brief   Application entry point.
 */
int main(void) {

	TimerHandle_t xLoggerTimer;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    xTaskCreate(DAC_Task, "DAC Task", configMINIMAL_STACK_SIZE + 300, \
    		NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, DAC_Task_Handler);

    xLoggerTimer = xTimerCreate("Logger Timer", (100/ portTICK_PERIOD_MS), pdTRUE, \
    		(void *)&deciseconds, NULL);

    dac_lookup_init(buffer);
    dac_init();

    xTimerStart(xLoggerTimer, 0);
    vTaskStartScheduler();
    while(1);
}

void DAC_Task(void* parameters)
{
	for(;;)
	{
		TickType_t PreviousWakeTime = xTaskGetTickCount();
		for (;;)
		{
			dac_out(*(buffer + dac_index));
			dac_index++;
			if(dac_index == 50)
				dac_index = 0;

			vTaskDelayUntil(&PreviousWakeTime, DACTASKPERIOD);
		}
	}
}