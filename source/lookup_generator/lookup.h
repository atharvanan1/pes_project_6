/*
 * lookup.h
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#ifndef LOOKUP_H_
#define LOOKUP_H_
#include <stdint.h>
#include <arm_math.h>
#include "logger.h"

void dac_lookup_init(uint16_t* buffer);

#endif /* LOOKUP_H_ */
