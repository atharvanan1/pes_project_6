 /**
 * File - 	errno.h
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * Error Enum
 */

#ifndef LOGGER_ERRNO_H_
#define LOGGER_ERRNO_H_
#include <stdint.h>

// Error/Event Enum
typedef enum {
	StartSchedulerError,
}error_t;

extern error_t errno;

// Prototype function
const char* Get_Error_Message(error_t error);

#endif /* LOGGER_ERRNO_H_ */
