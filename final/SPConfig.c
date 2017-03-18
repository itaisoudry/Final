/*
 * SPConfig.c
 *
 *  Created on: Mar 14, 2017
 *      Author: soudry
 */
#include "SPConfig.h"

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	SPConfig config = NULL;
	int returnValue = SUCCESS;
	SMART_MALLOC(SPConfig, config, sizeof(*config));
	return config;
	error:
	SMART_FREE(config);
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
	int returnValue = SUCCESS;
	char*path = NULL;
	SMART_MALLOC(char*, path, LINE_LENGTH*sizeof(char));
	return SP_CONFIG_SUCCESS;
	error:
	SMART_FREE(path);
	return SP_CONFIG_ALLOC_FAIL;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if (config == NULL || pcaPath == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	//won't be used here.
	int returnValue = SUCCESS;
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
	return SP_CONFIG_SUCCESS;

	error: SMART_FREE(path);
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
	int returnValue = SUCCESS;
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
	return returnValue;
	error: SMART_FREE(config->spImagesDirectory);
	SMART_FREE(config->spImagesPrefix);
	SMART_FREE(config->spImagesSuffix);
	SMART_FREE(config->spLoggerFilename);
	SMART_FREE(config->spPCAFilename);

}

