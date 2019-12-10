 /**
 * File - 	dma.h
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * DMA Functions
 */

#ifndef DMA_H_
#define DMA_H_
// Include files
#include <stdint.h>
#include "fsl_dma.h"
#include "fsl_dmamux.h"
#include "logger.h"

// Macros
#define BUFFER_LENGTH (64)
#define DMA_CHANNEL (0)
#define DMA_SOURCE (63)

// Prototype Functions
void dma_init(void);
void dma_transfer(uint16_t* src_buffer, uint16_t* dest_buffer);
void DMA_Callback(dma_handle_t *handle, void *param);

#endif /* DMA_H_ */
