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
bool validateConfigByValues(SPConfig config, char * spImagesDirectory,
		char * spImagesPrefix, char* spImagesSuffix, int spNumOfImages,
		int spPCADimension, char* spPCAFilename, int spNumOfFeatures,
		bool spExtractionMode, int spNumOfSimilarImages,
		SP_KDTREE_SPLIT_METHODS spKDTreeSplitMethod, int spKNN,
		bool spMinimalGUI, int spLoggerLevel, char* spLoggerFilename);
void printConfig(SPConfig config);
bool test() {
	SPConfig config = NULL;
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	printf("myconfig\n");
	config = spConfigCreate("myconfig.config", &msg);
	printConfig(config);

	return true;
}
bool missingDir(){
	return false;
}
bool missingExtractionMode(){
	return false;
}
bool missingKDTreeSplitMethod(){
	return false;
}
bool missingKNN(){
	return false;
}
bool missingLoggerFilename(){
	return false;
}
bool missingLoggerLevel(){
	return false;
}
bool missingMinimalGUI(){
	return false;
}
bool missingNumImages(){
	return false;
}
bool missingNumFeatures(){
	return false;
}
bool missingNumSimilarImages(){
	return false;
}
bool missingPCADim(){
	return false;
}
bool missingPCAFilename(){
	return false;
}
bool missingPrefix(){
	return false;
}
bool missingSuffix(){
	return false;
}
bool fullConfig() {
	SPConfig config = NULL;
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	config = spConfigCreate("unit_tests/ConfigFiles/FullConfig.config", &msg);
	if (msg == SP_CONFIG_SUCCESS) {
		return validateConfigByValues(config, "Directory/", "img", ".png", 50,
				15, "filename.txt", 10, true, 12, RANDOM, 10, true, 1,
				"logger.txt");
	}
	return false;
}
bool spacesTest() {
	SPConfig config = NULL;
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	config = spConfigCreate("unit_tests/ConfigFiles/SpacesConfig.config", &msg);
	if (msg == SP_CONFIG_SUCCESS)
		return validateConfigByValues(config, "./images/", "img", ".png", 2, 20,
				"pca.yml", 100, true, 5, DEFAULT_SP_EXTRACTION_MODE, DEFAULT_SP_KNN, DEFAULT_SP_MIN_GUI,
				DEFAULT_SP_LOGGER_LEVEL, "stdout");
	return msg == SP_CONFIG_SUCCESS;
}
int main() {
	RUN_TEST(spacesTest);
	RUN_TEST(fullConfig);

}
bool validateConfigByValues(SPConfig config, char * spImagesDirectory,
		char * spImagesPrefix, char* spImagesSuffix, int spNumOfImages,
		int spPCADimension, char* spPCAFilename, int spNumOfFeatures,
		bool spExtractionMode, int spNumOfSimilarImages,
		SP_KDTREE_SPLIT_METHODS spKDTreeSplitMethod, int spKNN,
		bool spMinimalGUI, int spLoggerLevel, char* spLoggerFilename) {
	if (config->spExtractionMode != spExtractionMode || config->spKNN != spKNN
			|| config->spLoggerLevel != spLoggerLevel
			|| config->spMinimalGUI != spMinimalGUI
			|| config->spNumOfFeatures != spNumOfFeatures
			|| config->spNumOfImages != spNumOfImages
			|| config->spNumOfSimilarImages != spNumOfSimilarImages
			|| config->spPCADimension != spPCADimension) {
		return false;
	}
	if (strcmp(config->spImagesDirectory, spImagesDirectory) != 0
			|| strcmp(config->spImagesPrefix, spImagesPrefix) != 0
			|| strcmp(config->spImagesSuffix, spImagesSuffix) != 0
			|| strcmp(config->spLoggerFilename, spLoggerFilename) != 0
			|| strcmp(config->spPCAFilename, spPCAFilename) != 0) {
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

