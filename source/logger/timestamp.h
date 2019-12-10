/**
 * File - timestamp.h
 * Author - Atharva Nandanwar
 * Email - atharva.nandanwar@colorado.edu
 * Principles of Embedded Software
 * University of Colorado Boulder
 */
#ifndef LOGGER_TIMESTAMP_H_
#define LOGGER_TIMESTAMP_H_
// Include files
#include "MKL25Z4.h"

// Struct for timestamp information
typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t deciseconds;
} timestamp_t;

// Prototype Functions
void vTimestamp_Init(void);
timestamp_t tTimestamp_Get_Timestamp(void);
#endif /* LOGGER_TIMESTAMP_H_ */
