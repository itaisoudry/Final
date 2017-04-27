/*
 * main_aux.h
 *
 *  Created on: Apr 26, 2017
 *      Author: ifat
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_
#include <stdio.h>
#include <stdlib.h>
#include "SPConfig.h"
#include "SPPoint.h"

#include "math.h"
#include "SPLogger.h"
#include "Utils/AllocationHandler.h"
#include "Utils/ResponseCodes.h"
#include <ctype.h>
#include "SPLogger.h"
#include <stdbool.h>

#define MSG0 "SP_CONFIG_MISSING_DIR"
#define MSG1 "SP_CONFIG_MISSING_PREFIX"
#define MSG2 "SP_CONFIG_MISSING_SUFFIX"
#define	MSG3 "SP_CONFIG_MISSING_NUM_IMAGES"
#define	MSG4 "SP_CONFIG_CANNOT_OPEN_FILE"
#define	MSG5 "SP_CONFIG_ALLOC_FAIL"
#define	MSG6 "SP_CONFIG_INVALID_INTEGER"
#define	MSG7 "SP_CONFIG_INVALID_STRING"
#define	MSG8 "SP_CONFIG_INVALID_ARGUMENT"
#define	MSG9 "SP_CONFIG_INDEX_OUT_OF_RANGE"
#define	MSG10 "SP_CONFIG_SUCCESS"

int saveOrLoadFeatsToFile(SPPoint*** featsArr, int numOfImages, SPConfig config,int* featsArrNum, bool spExtractionMode, SP_CONFIG_MSG* msg);
const char* configMsgToString( SP_CONFIG_MSG msg);
int cmpfunc (const void * a, const void * b);



#endif /* MAIN_AUX_H_ */
