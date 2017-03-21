/*
 * SPConfig.c
 *
 *  Created on: Mar 14, 2017
 *      Author: soudry
 */
#include "SPConfig.h"
int extractConfigDataFromFile(const char* filename, SPConfig config);
int setConfigDefaultValues(SPConfig config);
int extractDataFromLine(SPConfig config, const char* filename, char* line,
		int lineNumber);
char* trimSpacesFromBothSides(char* line);
void printErrorMessage(const char* filename, int lineNumber, char* msg);

int validateAndInsert(SPConfig config, const char* filename, int lineNumber,
		char* key, char* value);
SP_CONFIG_MSG responseCodeToConfigMessage(ResponseCode code);
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	SPConfig config = NULL;
	int resultValue = SUCCESS;
	SMART_MALLOC(SPConfig, config, sizeof(*config));
	SMART_FUNCTION_CALL(setConfigDefaultValues(config));
	SMART_FUNCTION_CALL(extractConfigDataFromFile(filename, config));
	return config;
	error:
	SMART_FREE(config);
	*msg = SP_CONFIG_INVALID_ARGUMENT;
	return NULL;
}
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spExtractionMode;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spMinimalGUI;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NEGATIVE;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NEGATIVE;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NEGATIVE;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spPCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index) {
	if (config == NULL || imagePath == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	if (index >= config->spNumOfImages)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	int resultValue = SUCCESS;
	char*path = NULL;
	SMART_MALLOC(char*, path, LINE_LENGTH*sizeof(char));
	return responseCodeToConfigMessage(resultValue);
	error:
	SMART_FREE(path);
	return responseCodeToConfigMessage(resultValue);
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if (config == NULL || pcaPath == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
//won't be used here.
	int resultValue = SUCCESS;
	char* path = NULL;
	SMART_MALLOC(char*, path, LINE_LENGTH*sizeof(char));
	strcpy(path, EMPTY_STR);
	strcat(path, config->spImagesDirectory);
	strcat(path, config->spPCAFilename);
	if (sizeof(pcaPath) < strlen(path)) {
		SMART_FREE(path);
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	strcpy(pcaPath, EMPTY_STR);
	strcpy(pcaPath, path);
	return responseCodeToConfigMessage(resultValue);

	error: SMART_FREE(path);
	return responseCodeToConfigMessage(resultValue);
}

void spConfigDestroy(SPConfig config) {
	SMART_FREE(config->spImagesDirectory);
	SMART_FREE(config->spImagesPrefix);
	SMART_FREE(config->spImagesSuffix);
	SMART_FREE(config->spLoggerFilename);
	SMART_FREE(config->spPCAFilename);
	SMART_FREE(config);

}
int setConfigDefaultValues(SPConfig config) {
	int resultValue = SUCCESS;
	config->spPCADimension = DEFAULT_SP_PCA_DIM;
	config->spNumOfFeatures = DEFAULT_SP_NUM_FEATURES;
	config->spExtractionMode = DEFAULT_SP_EXTRACTION_MODE;
	config->spMinimalGUI = DEFAULT_SP_MIN_GUI;
	config->spNumOfSimilarImages = DEFAULT_SP_NUM_OF_SIMILAR_IMGS;
	config->spKNN = DEFAULT_SP_KNN;
	config->spKDTreeSplitMethod = DEFAULT_SP_KDTREE_SPLOT_METHOD;
	config->spLoggerLevel = DEFAULT_SP_LOGGER_LEVEL;

	config->spImagesDirectory = NULL;
	config->spImagesPrefix = NULL;
	config->spImagesSuffix = NULL;
	config->spLoggerFilename = NULL;
	config->spPCAFilename = NULL;

	SMART_MALLOC(char*, config->spImagesDirectory, LINE_LENGTH*sizeof(char));
	SMART_MALLOC(char*, config->spImagesPrefix, LINE_LENGTH*sizeof(char));
	SMART_MALLOC(char*, config->spImagesSuffix, LINE_LENGTH*sizeof(char));
	SMART_MALLOC(char*, config->spLoggerFilename, LINE_LENGTH*sizeof(char));
	SMART_MALLOC(char*, config->spPCAFilename, LINE_LENGTH*sizeof(char));

	strcpy(config->spImagesDirectory, EMPTY_STR);
	strcpy(config->spImagesPrefix, EMPTY_STR);
	strcpy(config->spImagesSuffix, EMPTY_STR);
	strcpy(config->spLoggerFilename, DEFAULT_LOGGER);
	strcpy(config->spPCAFilename, DEFAULT_PCA);
	return resultValue;
	error: SMART_FREE(config->spImagesDirectory);
	SMART_FREE(config->spImagesPrefix);
	SMART_FREE(config->spImagesSuffix);
	SMART_FREE(config->spLoggerFilename);
	SMART_FREE(config->spPCAFilename);
	return resultValue;

}

int extractConfigDataFromFile(const char* filename, SPConfig config) {
	int resultValue = SUCCESS;
	int lineNumber = 1;
	FILE* file = NULL;
	char* line = NULL;

	file = fopen(filename, "r");
	if (file == NULL) {
		return CFG_CANNOT_OPEN_FILE;
	}
	SMART_MALLOC(char*, line, LINE_LENGTH*sizeof(char));

	while (fgets(line, LINE_LENGTH, file) != NULL) {
		char* commentPtr = NULL;
		//trim
		line = trimSpacesFromBothSides(line);
		//if first char is not '#'
		if (line[0] != COMMENT && strcmp(line,EMPTY_STR)) {
			commentPtr = strchr(line, COMMENT);

			if (commentPtr != NULL) {
				line = strtok(line, "#");
			}
			// extract config key and value from line - if not valid, error will be printed and the program will end
			SMART_FUNCTION_CALL(
					extractDataFromLine(config, filename, line, lineNumber));
			// assign variables to config
		}

		lineNumber++;
	}
	return resultValue;
	error:
	SMART_FREE(line);
	return resultValue;

}
int extractDataFromLine(SPConfig config, const char* filename, char* line,
		int lineNumber) {
	int resultValue = SUCCESS;
	char* equalsPtr = NULL;
	char* key = NULL;
	char* value = NULL;
	equalsPtr = strchr(line, EQUALS);
	//line is not a comment and doesn't contains '=' - error
	if (equalsPtr == NULL) {
		return CFG_INVALID_LINE;
	}
	//break to key value using '\0'
	key = line;
	value = equalsPtr + 1;
	*equalsPtr = END_LINE;
	//trim key value
	key = trimSpacesFromBothSides(key);
	value = trimSpacesFromBothSides(value);
	//key  is invalid
	if (strchr(key, SPACE) != NULL) {
		printErrorMessage(filename, lineNumber, INVALID_CFG_LINE);
	}
	SMART_FUNCTION_CALL(validateAndInsert(config,filename,lineNumber,key, value));
	return resultValue;
	error: return resultValue;

}

int validateAndInsert(SPConfig config, const char* filename, int lineNumber,
		char* key, char* value) {
	int resultValue = SUCCESS;
	int intValue = 0;
	if (strcmp(key, SP_IMAGES_DIR) == 0) {
		//if dir contains spaces
		if (strchr(value, SPACE) != NULL) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			strcpy(config->spImagesDirectory, value);
		}
	}
	if (strcmp(key, SP_IMAGES_PREFIX) == 0) {
		//if images prefix contains spaces
		if (strchr(value, SPACE) != NULL) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			strcpy(config->spImagesPrefix, value);
		}
	}
	if (strcmp(key, SP_IMAGES_SUFFIX) == 0) {
		//suffix must be gif/bmp/jps/png
		if (strcmp(value, BMP) == 0 && strcmp(value, JPG) == 0
				&& strcmp(value, GIF) == 0 && strcmp(value, PNG)) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			strcpy(config->spImagesSuffix, value);
		}
	}
	if (strcmp(key, SP_LOGGER_FILENAME) == 0) {
		//if filename contains spaces
		if (strchr(value, SPACE) != NULL) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			strcpy(config->spLoggerFilename, value);
		}
	}
	if (strcmp(key, SP_PCA_FILENAME) == 0) {
		//if filename conains spaces
		if (strchr(value, SPACE) != NULL) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			strcpy(config->spPCAFilename, value);
		}
	}
	if (strcmp(key, SP_NUM_OF_IMAGES) == 0) {
		intValue = atoi(value);
		//value must be positive - if zero, means it is not valid.
		if (intValue <= 0) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spNumOfImages = intValue;
		}
	}
	if (strcmp(key, SP_PCA_DIM) == 0) {
		intValue = atoi(value);
		//number in range between 10 and 28
		if (intValue < 10 || intValue > 28) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spPCADimension = intValue;
		}
	}
	if (strcmp(key, SP_NUM_FEATURES) == 0) {
		intValue = atoi(value);
		//value must be positive
		if (intValue < 0) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spNumOfFeatures = intValue;
		}
	}
	if (strcmp(key, SP_NUM_SIMILAR_IMAGES) == 0) {
		intValue = atoi(value);
		//value must be positive
		if (intValue < 0) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spNumOfSimilarImages = intValue;
		}
	}
	if (strcmp(key, SP_KNN) == 0) {
		intValue = atoi(value);
		//value must be positive
		if (intValue < 0) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spKNN = intValue;
		}
	}
	if (strcmp(key, SP_LOGGER_LEVEL) == 0) {
		intValue = atoi(value);
		//value must be positive
		if (intValue < 1 || intValue > 4) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spLoggerLevel = intValue;
		}
	}
	if (strcmp(key, SP_MIN_GUI) == 0) {
		//if not 'true' or 'false'
		if (strcmp(value, "true") == 0 && strcmp(value, "false") == 0) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spMinimalGUI = strcmp(value, "true") == 0;
		}
	}
	if (strcmp(key, SP_EXTRACTION_MODE) == 0) {
		//if not 'true' or 'false'
		if (strcmp(value, "true") == 0 && strcmp(value, "false") == 0) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spExtractionMode = strcmp(value, "true") == 0;
		}
	}
	if (strcmp(key, SP_KDTREE_SPLIT_METHOD) == 0) {
		intValue = atoi(value);
		if (intValue != RANDOM && intValue != MAX_SPREAD
				&& intValue != INCREMENTAL) {
			resultValue = CFG_INVALID_ARGUMENT;
		} else {
			config->spKDTreeSplitMethod = intValue;
		}
	}
	if (resultValue == CFG_INVALID_ARGUMENT)
		printErrorMessage(filename, lineNumber, INVALID_CFG_VALUE);
	else if (resultValue != SUCCESS) {
		printErrorMessage(filename, lineNumber, INVALID_CFG_LINE);
	}
	return resultValue;
}
void printErrorMessage(const char* filename, int lineNumber, char* msg) {
	printf(ERROR_FORMAT, filename, lineNumber, msg);
}
char* trimSpacesFromBothSides(char* line) {
	int l = strlen(line);

	while (isspace(line[l - 1]))
		--l;
	while (*line && isspace(*line))
		++line, --l;
	//TODO - Deal with this.
	return strndup(line, l);
}
SP_CONFIG_MSG responseCodeToConfigMessage(ResponseCode code) {
	SP_CONFIG_MSG msg = 0;
	switch (code) {
	case ALLOCATION_FAILED:
		msg = SP_CONFIG_ALLOC_FAIL;
		break;
	case SUCCESS:
		msg = SP_CONFIG_SUCCESS;
		break;
	case CFG_INVALID_LINE:
		msg = SP_CONFIG_INVALID_STRING;
		break;
	case CFG_MISSING_PREFIX:
		break;
	case CFG_MISSING_SUFFIX:
		break;
	case CFG_MISSING_NUM_IMAGES:
		break;
	case CFG_CANNOT_OPEN_FILE:
		msg = SP_CONFIG_CANNOT_OPEN_FILE;
		break;
	case CFG_INVALID_INTEGER:
		break;
	case CFG_INVALID_STRING:
		break;
	case CFG_INVALID_ARGUMENT:
		break;
	case CFG_INDEX_OUT_OF_RANGE:
		msg = SP_CONFIG_INVALID_ARGUMENT;
		break;
	case CFG_MISSING_DIR:
		break;
	}

	return msg;
}

