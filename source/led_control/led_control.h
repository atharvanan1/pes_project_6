 /**
 * File - 	led_control.h
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * LED Control Library
 */
#ifndef LED_CONTROL_LED_CONTROL_H_
#define LED_CONTROL_LED_CONTROL_H_

// Include files
#include <stdint.h>
#include "MKL25Z4.h"
#include "logger.h"

// Macros
#define RED_LED		(0x1 << 18U)
#define GREEN_LED	(0x1 << 19U)
#define BLUE_LED	(0x1 << 1U)

// Enum for LED Color
typedef enum {
	Red,
	Green,
	Blue,
} led_color_t;

// Prototype functions
void LED_Init(void);
void Turn_On_LED(led_color_t LED);
void Turn_Off_LED(led_color_t LED);
void Toggle_LED(led_color_t LED);

#endif /* LED_CONTROL_LED_CONTROL_H_ */
