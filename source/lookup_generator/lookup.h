 /**
 * File - 	lookup.h
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * Lookup Table Generator
 */

#ifndef LOOKUP_H_
#define LOOKUP_H_
// Include files
#include <stdint.h>
#include <arm_math.h>
#include "logger.h"

// Prototype Function
void dac_lookup_init(uint16_t* buffer);

#endif /* LOOKUP_H_ */
