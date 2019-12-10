/**
  * File Name 		- errno.c
  * Description 	- contains error enums, and related functions
  * Author			- Atharva Nandanwar
  * Tools			- GNU C Compiler / ARM Compiler Toolchain
  * Leveraged Code 	- https://android.googlesource.com/kernel/lk/+/upstream-master/include/errno.h
  * URL				-
  */

#include "errno.h"

/**
 * Get_Error_Message
 * returns with error message for particular errors
 * @param
 * 		error - error code
 * @return
 * 		returns error message
 */
const char* Get_Error_Message(error_t error)
{
	switch(error)
	{
	default:
		return "";
		break;
	}
}
