 /**
 * File - 	dma.c
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * DMA Functions
 */

#include "dma.h"

// DMA Handle for DMA Operation
dma_handle_t dma_handle;
// DMA Transfer Configuration for DMA Operation
dma_transfer_config_t dma_tx_struct;

/**
 * dma_init
 * Initializes DMA peripheral
 */
void dma_init(void)
{
	if(logger.Get_Log_Level() == lDebug)
		logger.Log_Write(__func__, mDebug, "DAC Initialization Started");
    /* Configure DMAMUX */
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, DMA_CHANNEL, DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, DMA_CHANNEL);
    /* Configure DMA one shot transfer */
    DMA_Init(DMA0);
    DMA_CreateHandle(&dma_handle, DMA0, DMA_CHANNEL);

    logger.Log_Write(__func__, mStatus, "DMA Initialized");
}

/**
 * dma_transfer
 * Initiates DMA transfer
 */
void dma_transfer(uint16_t* src_buffer, uint16_t* dest_buffer)
{
    DMA_SetCallback(&dma_handle, DMA_Callback, NULL);
    DMA_PrepareTransfer(&dma_tx_struct, src_buffer, sizeof(src_buffer[0]), dest_buffer, sizeof(dest_buffer[0]), BUFFER_LENGTH * 2,
                        kDMA_MemoryToMemory);
    DMA_SubmitTransfer(&dma_handle, &dma_tx_struct, kDMA_EnableInterrupt);
	DMA_StartTransfer(&dma_handle);
}
