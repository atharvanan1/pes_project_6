
 
/**
 * @file    pes_project_6.c
 * @brief   Application entry point.
 */
#include "main.h"
;/* TODO: insert other definitions and declarations here. */
#define DACTASKPERIOD (100 / portTICK_PERIOD_MS)
#define ADCTASKPERIOD (100 / portTICK_PERIOD_MS)

// Used to log run number
uint8_t run_number = 0;

// Used to put lookup table values for DAC
uint16_t buffer[50];

// Circular buffer for ADC
circular_buffer_t* adc_buffer = NULL;

// Buffer for DSP
uint16_t dsp_buffer[64];

// Index for DAC
uint8_t dac_index = 0;

// Task Handlers for all the tasks
TaskHandle_t DAC_Task_Handler;
TaskHandle_t ADC_Task_Handler;
TaskHandle_t DSP_Task_Handler;

// Timer Handler
TimerHandle_t xLoggerTimer;
SemaphoreHandle_t xLED_Semaphore;

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
#ifdef DEBUG_CODE
    logger.Set_Log_Level(lDebug);
#else
    logger.Set_Log_Level(lNormal);
#endif
    logger.Log_Write(__func__, mError, "Starting Program");
    LED_Init();

    // Task Creation
    if(logger.Get_Log_Level() == lDebug)
    	logger.Log_Write(__func__, mDebug, "Creating Tasks");
    // DAC Task on Max priority as it is critical to output Sine Wave
    xTaskCreate(DAC_Task, "DAC Task", configMINIMAL_STACK_SIZE + 100, \
    		NULL, (configMAX_PRIORITIES - 1), &DAC_Task_Handler);
    // ADC Task on Minimum priority as it is not so critical task
    xTaskCreate(ADC_Task, "ADC Task", configMINIMAL_STACK_SIZE + 100, \
    		NULL, (configMAX_PRIORITIES - 3), &ADC_Task_Handler);
    // DSP Task as it shall run after DMA Transfer is complete
    xTaskCreate(DSP_Task, "DSP Task", configMINIMAL_STACK_SIZE + 500, \
    		NULL, (configMAX_PRIORITIES - 2), &DSP_Task_Handler);

    // Semaphore Creation
    if(logger.Get_Log_Level() == lDebug)
    	logger.Log_Write(__func__, mDebug, "Creating Semaphores");
    xLED_Semaphore = xSemaphoreCreateCounting(5, 0);

    // Peripheral Initialization Routines
    dac_lookup_init(buffer);
    dac_init();
    adc_buffer = cb_init_buffer(64);
    adc_init();
    // DSP Buffer Initialization
    for(uint8_t index = 0; index < 64; index++)
    	dsp_buffer[index] = 0;
    dma_init();

    // Starting Scheduler
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mDebug, "Starting Scheduler");
    vTaskStartScheduler();
    while(1);
}

void DAC_Task(void* parameters)
{
	// Tick count used to block the task for certain amount of time
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	for (;;)
	{
		if(logger.Get_Log_Level() == lDebug)
			logger.Log_Write(__func__, mDebug, "DAC Task");

		// Output buffer values continuously
		dac_out(*(buffer + dac_index));
		dac_index++;
		if(dac_index == 50)
			dac_index = 0;

		// Access LED if not used by other systems
		if(uxSemaphoreGetCount(xLED_Semaphore) == 0)
		{
			Turn_Off_LED(Blue);
			Toggle_LED(Green);
		}
		else
			xSemaphoreTake(xLED_Semaphore, 0);

		// Block the task for 100 ms
		vTaskDelayUntil(&PreviousWakeTime, DACTASKPERIOD);
	}
}

void ADC_Task(void* parameters)
{
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	for (;;)
	{
		if(logger.Get_Log_Level() == lDebug)
			logger.Log_Write(__func__, mDebug, "ADC Task");

		// Fill the circular buffer with ADC values
		if(CB_buffer_full == cb_add_item(adc_buffer, adc_value()))
		{
			run_number++;
			logger.Log_Write(__func__, mStatus, "Run Number %d", run_number);
			logger.Log_Write(__func__, mStatus, "DMA Transfer Started");

			// Semaphore for LED Control
			xSemaphoreGive(xLED_Semaphore);
			xSemaphoreGive(xLED_Semaphore);
			xSemaphoreGive(xLED_Semaphore);
			xSemaphoreGive(xLED_Semaphore);
			Turn_Off_LED(Green);
			Turn_On_LED(Blue);

			// Start DMA Transfer
			dma_transfer(adc_buffer->pointer, dsp_buffer);
			cb_empty_buffer(adc_buffer);
		}

		// Block the task for another 100 ms
		vTaskDelayUntil(&PreviousWakeTime, ADCTASKPERIOD);
	}
}

void DSP_Task(void* parameters)
{
	for (;;)
	{
		if(logger.Get_Log_Level() == lDebug)
			logger.Log_Write(__func__, mDebug, "DSP Task");

		// DSP Operations
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

		// Temporary pointer used to point at string from float_string function
		char* temp = NULL;
		logger.Log_Write(__func__, mStatus, "Mean - %s", temp = float_string(mean));
		if(mean)
			free(temp);		// freeing the pointer as it was dynamically allocated
		logger.Log_Write(__func__, mStatus, "Max - %d", max);
		logger.Log_Write(__func__, mStatus, "Min - %d", min);
		logger.Log_Write(__func__, mStatus, "SD - %s", temp = float_string(standard_deviation));
		if(mean)
			free(temp);

		// Exit when Run Number 5 is over
		if(run_number == 5)
		{
			logger.Log_Write(__func__, mError, "Exiting Program");
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
	// Temporary variable to hold value
	uint32_t temp = value;

	// Buffer to hold individual values
	uint8_t buffer[12];
	uint8_t index = 0;

	// Logic for zero
	if(value == 0)
	{
		return "0.00";
	}

	// Extract each number
	while(temp != 0)
	{
		buffer[index] = temp % 10;
		temp = temp / 10;
		index++;
	}

	// Character buffer that will hold the numerical string
	char* char_buffer = (char *) malloc(sizeof(char) * 13);
	uint8_t char_index = 0;
	index--;
	while(index - char_index >= 2)
	{
		*(char_buffer + char_index) = buffer[index - char_index] + '0';
		char_index++;
	}

	// For values less than 1.00
	if(value < 100)
	{
		char_buffer[0] = '0';
		char_index++;
	}

	// Decimal point and decimal values
	*(char_buffer + char_index) = '.';
	char_index++;
	*(char_buffer + char_index) = buffer[1] + '0';
	char_index++;
	*(char_buffer + char_index) = buffer[0] + '0';
	char_index++;

	// Terminate with NULL
	*(char_buffer + char_index) = 0;

	// Return the string
	return char_buffer;
}
