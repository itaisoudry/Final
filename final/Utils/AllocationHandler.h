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
 * If memory allocation fails, the function will jump to the error label ( which will free all the memory allocated in the function )
 */
#define SMART_MALLOC(pointerType, pointer, size)\
		pointer = (pointerType) malloc(size);\
		if(pointer==NULL){\
			resultValue = ALLOCATION_FAILED;\
			goto error;\
		}

#define SMART_FREE(pointer)\
		if (pointer!=NULL){free(pointer);}\
		pointer = NULL;\

#define SMART_FUNCTION_CALL(function)\
	resultValue = function;\
	if(resultValue!=SUCCESS){goto error;}
#endif /* UTILS_ALLOCATIONHANDLER_H_ */
