/*
 * dac.h
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#ifndef DAC_DAC_H_
#define DAC_DAC_H_
#include <stdint.h>
#include "MKL25Z4.h"

void dac_init(void);
void dac_out(uint16_t data);

#endif /* DAC_DAC_H_ */
