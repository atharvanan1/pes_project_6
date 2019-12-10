# Principles of Embedded Software Project 6
### Title: FreeRTOS, DAC, ADC, and DMA

**Name: Atharva Nandanwar**

This repository contains source files for Principles of Embedded Software Project 6

---

**Source Folder:**

1. main.c/h - main subroutine

2. circular_buffer/circular_buffer.c/h - functions and structure definition for circular buffer

3. led_control/led_control.c/h - functions to control LED

4. logger/logger.c/h - functions to do logging

5. logger/errno.c/h - error handling routines

6. logger/timestamp.c/h - timestamp functionality

7. dac/dac.c/h - dac functions

8. adc/adc.c/h - adc functions

9. dma/dma.c/h - dma functions

10. lookup_generator/lookup.c/h - function for generating lookup table

---

**Observations:**

1. FreeRTOS makes task management easy. The functions provided by FreeRTOS as APIs, if read properly can be used to make a robust system.

2. Semaphores came off as something wonderful that we can use to block access to certain part of code. I have used it for LED access.

3. Time management, Priorities, Semaphores would make the system really complicated to handle, however the system would also get really powerful by using FreeRTOS.

---

**Installation/Execution Notes:**

Compiler - gcc-arm-none-eabi

To enable detailed logging Debug mode can be compiled by added **DEBUG_CODE** in preprocessor defines.

---
