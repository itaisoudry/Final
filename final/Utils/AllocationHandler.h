/*
 * AllocationHandler.h
 *
 *  Created on: Mar 13, 2017
 *      Author: soudry
 */

#ifndef UTILS_ALLOCATIONHANDLER_H_
#define UTILS_ALLOCATIONHANDLER_H_
#include <stdlib.h>

/**
 * This function is used to safely allocate memory.
 * If memory allocation failes, the function will jump to the error label ( which will free all the memory allocated in the function )
 */
#define SMART_MALLOC(type, ptr, size)\
		ptr = (type) malloc(size);\
		if(ptr==NULL){\
			returnValue = ALLOCATION_FAILED;\
			goto error;\
		}

#define SMART_FREE(ptr)\
		if (ptr!=NULL){\
			free(ptr);\
		}\
		ptr = NULL;\


#define SMART_REALLOC(type, ptr, size)\
		if(ptr==NULL){\
			SMART_MALLOC(type, ptr, size);\
		}else{\
		ptr = (type) realloc(ptr, size);\
		if(ptr==NULL){\
			returnValue = ALLOCATION_FAILED;\
			goto error;\
		}}

#define SMART_FUNCTION_CALL(function)\
	returnValue = function;\
	if(returnValue!=SUCCESS){\
		goto error;\
	}
#endif /* UTILS_ALLOCATIONHANDLER_H_ */
