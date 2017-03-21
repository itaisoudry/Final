/*
 * sp_config_unit_test.c
 *
 *  Created on: Mar 18, 2017
 *      Author: soudry
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPConfig.h"
void printConfig(SPConfig config);
bool test() {
	SPConfig config = NULL;
	SP_CONFIG_MSG msg = SP_CONFIG_SUCCESS;
	config = spConfigCreate("myconfig.cfg", &msg);
	printf("%d", msg);
	config = spConfigCreate("unit_tests/ConfigFiles/BadEnumExtractionMode.txt", &msg);
	printf("%d", msg);
	config = spConfigCreate("unit_tests/ConfigFiles/NoDirectory.txt", &msg);
	printf("%d", msg);
	config = spConfigCreate("unit_tests/ConfigFiles/NoNumberOfImages.txt", &msg);
	printf("%d", msg);
	config = spConfigCreate("unit_tests/ConfigFiles/NoPrefix.txt", &msg);
	printf("%d", msg);
	config = spConfigCreate("unit_tests/ConfigFiles/NoSuffix.txt", &msg);
	printf("%d", msg);
	config = spConfigCreate("unit_tests/ConfigFiles/PositiveNumberOfFeatures.txt", &msg);
	printf("%d", msg);
	printConfig(config);
	return true;
}
int main() {
	RUN_TEST(test);

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

