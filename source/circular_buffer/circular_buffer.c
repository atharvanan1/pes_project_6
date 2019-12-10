	/**
 * File - circular_buffer.c
 * Author - Atharva Nandanwar
 * Email - atharva.nandanwar@colorado.edu
 * Principles of Embedded Software
 * University of Colorado Boulder
 */

#include "circular_buffer.h"

/**
 * cb_init_buffer
 * Creates a circular buffer
 * @param
 * 		length - size of circular buffer
 * @return
 * 		pointer to circular buffer
 */
circular_buffer_t* cb_init_buffer(uint16_t length)
{
	// Allocate memory for buffer structure, and memory for buffer
	circular_buffer_t* buffer_pointer = NULL;
	buffer_pointer = (circular_buffer_t *) malloc(sizeof(circular_buffer_t));
	buffer_pointer->pointer = (uint8_t *) malloc(length);

	// Set all the parameters
	buffer_pointer->head = buffer_pointer->pointer;
	buffer_pointer->tail = buffer_pointer->pointer;
	buffer_pointer->count = 0;
	buffer_pointer->length = length;
	return buffer_pointer;
}

/**
 * cb_destroy_buffer
 * Destroys the circular buffer
 * @param
 * 		buffer - pointer to circular buffer
 * @return
 * 		status of operation
 */
CB_status_t cb_destroy_buffer(circular_buffer_t* buffer)
{
	// Free the memory for buffer, and buffer structure
	free(buffer->pointer);
	buffer->pointer = NULL;
	free(buffer);
	buffer = NULL;
	return CB_buffer_destroyed;
}

/**
 * cb_check_full
 * Checks if buffer is full
 * @param
 * 		buffer - pointer to circular buffer
 * @return
 * 		status of operation
 */
CB_status_t cb_check_full(circular_buffer_t* buffer)
{
	// Flag error
	if(buffer == NULL)
	{
		return CB_buffer_error;
	}

	// Check full
	if(buffer->count == buffer->length)
	{
		return CB_buffer_full;
	}
	else
	{
		return CB_buffer_not_full;
	}
}

/**
 * cb_check_empty
 * Checks if buffer is empty
 * @param
 * 		buffer - pointer to circular buffer
 * @return
 * 		status of operation
 */
CB_status_t cb_check_empty(circular_buffer_t* buffer)
{
	// Flag error
	if(buffer == NULL)
	{
		return CB_buffer_error;
	}

	// Check empty
	if(buffer->count == 0)
	{
		return CB_buffer_empty;
	}
	else
	{
		return CB_buffer_not_empty;
	}
}

/**
 * cb_add_item
 * Checks if buffer is full, and adds item if not full
 * @param
 * 		buffer - pointer to circular buffer
 * @param
 * 		item - data to be added into circular buffer
 * @return
 * 		status of operation
 */
CB_status_t cb_add_item(circular_buffer_t* buffer, uint8_t item)
{
	// Flag error
	if(buffer == NULL)
	{
		return CB_buffer_error;
	}

	START_CRITICAL();
	// If not full, then update parameters
	if(cb_check_full(buffer) == CB_buffer_full)
	{
		return CB_buffer_full;
	}
	else
	{
		*(buffer->head) = item;
		buffer->head += 1;
		buffer->head = (uint32_t) (buffer->head - buffer->pointer) % buffer->length + buffer->pointer;
		buffer->count += 1;
	}
	END_CRITICAL();
	return CB_buffer_operation_success;
}


/**
 * cb_remove_item
 * Checks if circular buffer is empty, and removes the item
 * @param
 * 		buffer - pointer to circular buffer
 * @return
 * 		status of operation
 */
CB_status_t cb_remove_item(circular_buffer_t* buffer, uint8_t* data)
{
	// Flag error
	if(buffer == NULL)
	{
		return CB_buffer_error;
	}

	START_CRITICAL();
	// If not empty, then update parameters
	if(cb_check_empty(buffer) == CB_buffer_empty)
	{
		return CB_buffer_empty;
	}
	else
	{
		*data = *(buffer->tail);
		buffer->tail += 1;
		buffer->tail = (uint32_t) (buffer->tail - buffer->pointer) % buffer->length + buffer->pointer;
		buffer->count -= 1;
	}
	END_CRITICAL();
	return CB_buffer_operation_success;
}

/**
 * cb_verify_init
 * Verifies buffer initialization
 * @param
 * 		buffer - pointer to circular buffer
 * @return
 * 		status of operation
 */
CB_status_t cb_verify_init(circular_buffer_t* buffer)
{
	// Flag error
	if(buffer == NULL)
	{
		return CB_buffer_error;
	}

	// Checks for initialization error
	if(buffer->pointer == NULL)
	{
		return CB_buffer_error_init;
	}
	else if(buffer->head != buffer->pointer || buffer->tail != buffer->pointer)
	{
		return CB_buffer_error_init;
	}
	else if(buffer->count != 0)
	{
		return CB_buffer_error_init;
	}
	return CB_buffer_initialized;
}

void cb_empty_buffer(circular_buffer_t* buffer)
{
	buffer->count = 0;
}
