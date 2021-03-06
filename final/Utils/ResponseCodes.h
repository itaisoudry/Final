/*
 * ResponseCodes.h
 *
 *  Created on: Mar 13, 2017
 *      Author: soudry
 */

#ifndef UTILS_RESPONSECODES_H_
#define UTILS_RESPONSECODES_H_
typedef enum response_codes {
	SUCCESS,
	ALLOCATION_FAILED,
	CFG_INVALID_LINE,
	CFG_MISSING_DIR,
	CFG_MISSING_PREFIX,
	CFG_MISSING_SUFFIX,
	CFG_MISSING_NUM_IMAGES,
	CFG_CANNOT_OPEN_FILE,
	CFG_INVALID_INTEGER,
	CFG_INVALID_STRING,
	CFG_INVALID_ARGUMENT,
	CFG_INDEX_OUT_OF_RANGE,
} ResponseCode;

#endif /* UTILS_RESPONSECODES_H_ */
