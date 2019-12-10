/*
 * dma.h
 *
 *  Created on: Dec 8, 2019
 *      Author: athar
 */

#ifndef DMA_H_
#define DMA_H_
#include <stdint.h>
#include "fsl_dma.h"
#include "fsl_dmamux.h"

#define BUFFER_LENGTH (64)
#define DMA_CHANNEL (0)
#define DMA_SOURCE (63)

void dma_init(void);
void dma_transfer(uint16_t* src_buffer, uint16_t* dest_buffer);
void DMA_Callback(dma_handle_t *handle, void *param);

#endif /* DMA_H_ */
