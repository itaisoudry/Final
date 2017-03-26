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
SP_CONFIG_MSG message;
SPConfig config;
bool test() {
	message = SP_CONFIG_SUCCESS;
	printf("myconfig\n");
	config = spConfigCreate("myconfig.config", &message);
	printConfig(config);

	return true;
}
bool fullConfig() {
	config = spConfigCreate("unit_tests/ConfigFiles/FullConfig.config",
			&message);
	if (message == SP_CONFIG_SUCCESS) {
		return validateConfigByValues(config, "Directory/", "img", ".png", 50,
				15, "filename.txt", 10, true, 12, RANDOM, 10, true, 1,
				"logger.txt");
	}
	return false;
}
bool directoryTest() {
	config = spConfigCreate("unit_tests/ConfigFiles/DirectoryWithSpace.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate("unit_tests/ConfigFiles/DirectoryMissing.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_MISSING_DIR);
	return true;
}
bool prefixTest() {
	config = spConfigCreate("unit_tests/ConfigFiles/PrefixWithSpace.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate("unit_tests/ConfigFiles/PrefixMissing.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_MISSING_PREFIX);
	return true;
}
bool suffixTest() {
	config = spConfigCreate("unit_tests/ConfigFiles/SuffixWrong.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate("unit_tests/ConfigFiles/SuffixMissing.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_MISSING_SUFFIX);
	return true;
}
bool extractionModeTest() {
	config = spConfigCreate("unit_tests/ConfigFiles/ExtractionModeWrong.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate(
			"unit_tests/ConfigFiles/ExtractionModeMissing.config", &message);
	ASSERT_FALSE(config==NULL);
	ASSERT_TRUE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(spConfigIsExtractionMode(config,&message)==true);
	ASSERT_TRUE(message == SP_CONFIG_SUCCESS);
	return true;

}
bool pcaTest() {
	config = spConfigCreate("unit_tests/ConfigFiles/PCADimWrong.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate("unit_tests/ConfigFiles/PCADimMissing.config",
			&message);
	ASSERT_FALSE(config==NULL);
	ASSERT_TRUE(message == SP_CONFIG_SUCCESS);
	//default value
	ASSERT_TRUE(spConfigGetPCADim(config, &message) == 20);

	config = spConfigCreate("unit_tests/ConfigFiles/PCAFilenameSpace.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate("unit_tests/ConfigFiles/PCAFilenameMissing.config",
			&message);
	ASSERT_FALSE(config==NULL);
	ASSERT_TRUE(message == SP_CONFIG_SUCCESS);
	//default value
	ASSERT_TRUE(strcmp(config->spPCAFilename, "pca.yml") == 0);
	return true;
}
bool loggerTest() {
	config = spConfigCreate("unit_tests/ConfigFiles/LoggerFilenameSpace.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate(
			"unit_tests/ConfigFiles/LoggerFilenameMissing.config", &message);
	ASSERT_FALSE(config==NULL);
	ASSERT_TRUE(message == SP_CONFIG_SUCCESS);

	//default value
	ASSERT_TRUE(strcmp(config->spLoggerFilename, "stdout") == 0);
	config = spConfigCreate("unit_tests/ConfigFiles/LoggerLevelWrong.config",
			&message);
	ASSERT_TRUE(config==NULL);
	ASSERT_FALSE(message == SP_CONFIG_SUCCESS);
	ASSERT_TRUE(message == SP_CONFIG_INVALID_ARGUMENT);
	config = spConfigCreate("unit_tests/ConfigFiles/LoggerLevelMissing.config",
			&message);
	ASSERT_FALSE(config==NULL);
	ASSERT_TRUE(message == SP_CONFIG_SUCCESS);
	//default value
	ASSERT_TRUE(config->spLoggerLevel == 3);
	return true;
}
bool spacesTest() {
	config = spConfigCreate("unit_tests/ConfigFiles/SpacesConfig.config",
			&message);
	if (message == SP_CONFIG_SUCCESS)
		return validateConfigByValues(config, "./images/", "img", ".png", 2, 20,
				"pca.yml", 100, true, 5, DEFAULT_SP_EXTRACTION_MODE,
				DEFAULT_SP_KNN, DEFAULT_SP_MIN_GUI,
				DEFAULT_SP_LOGGER_LEVEL, "stdout");
	return message == SP_CONFIG_SUCCESS;
}
int main() {
//	RUN_TEST(spacesTest);
//	RUN_TEST(fullConfig);
//	RUN_TEST(directoryTest);
//	RUN_TEST(prefixTest);
//	RUN_TEST(suffixTest);
//	RUN_TEST(extractionModeTest);
//	RUN_TEST(pcaTest);
	RUN_TEST(loggerTest);
	spConfigDestroy(config);
	return 0;
	//TODO - combine tests - i.e dir test which include a test for space and a test for missing dir

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

