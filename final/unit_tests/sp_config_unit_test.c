/*
 * sp_config_unit_test.c
 *
 *  Created on: Mar 18, 2017
 *      Author: soudry
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPConfig.h"
void printConfig(SPConfig config);
bool test() {
	SPConfig config = NULL;
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	printf("myconfig\n");
	config = spConfigCreate("myconfig.config", &msg);
	printConfig(config);

	return true;
}
bool spacesTest() {
	SPConfig config = NULL;
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	config = spConfigCreate("unit_tests/ConfigFiles/SpacesConfig.config", &msg);
	printConfig(config);
	return msg == SP_CONFIG_SUCCESS;
}
int main() {
	//RUN_TEST(test);
	RUN_TEST(spacesTest);

}
bool validateConfigByValues(SPConfig config, bool spExtractionMode,
		char* spImagesDirectory, char* spImagesPrefix, char* spImagesSuffix,
		int spKDTreeSplitMethod, int spKNN, char* spLoggerFilename,
		int spLoggerLevel, int spMinimalGUI, int spNumOfFeatures,
		int spNumOfImages, int spNumOfSimilarImages, int spPCADimension,
		char* spPCAFilename) {
	if (config->spExtractionMode != spExtractionMode || config->spKNN != spKNN
			|| config->spLoggerLevel != spLoggerLevel
			|| config->spMinimalGUI != spMinimalGUI
			|| config->spNumOfFeatures != spNumOfFeatures
			|| config->spNumOfImages != spNumOfImages
			|| config->spNumOfSimilarImages != spNumOfSimilarImages
			|| config->spPCADimension != spPCADimension) {
		return false;
	}
	//TODO add more constraints
	if (strcmp(config->spImagesDirectory, spImagesDirectory) == 0
			|| strcmp(config->spImagesPrefix, spImagesPrefix) == 0
			|| strcmp(config->spImagesSuffix, spImagesSuffix) == 0
			|| strcmp(config->spLoggerFilename, spLoggerFilename) == 0) {
		return false;
	}
	return true;
}
void printConfig(SPConfig config) {
	if (config == NULL) {
		printf("NULL");
		return;

	}

	printf("Extraction mode: %d\n", config->spExtractionMode);
	printf("Images Directory: %s\n", config->spImagesDirectory);
	printf("Images Prefix: %s\n", config->spImagesPrefix);
	printf("Images Suffix: %s\n", config->spImagesSuffix);
	printf("Split Method: %d\n", config->spKDTreeSplitMethod);
	printf("KNN: %d\n", config->spKNN);
	printf("Logger file name: %s\n", config->spLoggerFilename);
	printf("Logger Leverl: %d\n", config->spLoggerLevel);
	printf("Minimal GUI: %d\n", config->spMinimalGUI);
	printf("Num of features: %d\n", config->spNumOfFeatures);
	printf("Num of Images: %d\n", config->spNumOfImages);
	printf("Num of similar images: %d\n", config->spNumOfSimilarImages);
	printf("PCA Dimension: %d\n", config->spPCADimension);
	printf("PCA File name: %s\n", config->spPCAFilename);
}

