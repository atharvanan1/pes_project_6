/**
  * File Name 		- errno.h
  * Description 	- header file for errno.c
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	-
  * URL				-
  */

#ifndef LOGGER_ERRNO_H_
#define LOGGER_ERRNO_H_
#include <stdint.h>

// Error/Event Enum
typedef enum {
	eUART_Parity_Error = 0x2001,
	eUART_Framing_Error = 0x2002,
	eUART_Noise_Error = 0x2003,
	eUART_Overrun_Error = 0x2004,
}error_t;

extern error_t errno;

// Prototype function
const char* Get_Error_Message(error_t error);

#endif /* LOGGER_ERRNO_H_ */
