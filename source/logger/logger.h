/**
  * File Name 		- logger.h
  * Description 	- header file for logger.c
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

// Include Files
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdarg.h>
#include "timestamp.h"

// Log Level and Message Type enums
typedef enum {lDebug, lNormal} log_level_t;
typedef enum {mError, mDebug, mStatus} message_type_t;

// Logger Instance struct
typedef struct {
	void ( * const Init )( void );
	void ( * const Log_Write )( const char* function_name, \
			message_type_t message_type, const char *msg, ... );
	void ( * const Set_Log_Level )( log_level_t level );
	log_level_t ( * const Get_Log_Level )( void );
}logger_instance;

extern logger_instance const logger;
#endif /* LOGGER_LOGGER_H_ */
