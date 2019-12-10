 /**
 * File - 	errno.c
 * Author-	Atharva Nandanwar
 * Principles of Embedded Software Project 6
 * University of Colorado Boulder
 * Error Enum
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
