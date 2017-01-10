/*
 * main_aux.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */

#include "main_aux.h"

void getImagesPath(char* imagePathInput) {
	imagePathInput = (char*) malloc(MAX_STRING * sizeof(char));
	if (imagePathInput == NULL) {
		printf(ERROR_ALLOCAT);
		return;
	}
	printf(INPUT_IMG_DIRECTORY_PATH);
	if (fgets(imagePathInput, MAX_STRING, stdin) == NULL) {
		printf(ERROR_GENERAL);
	}

}

void getImagesPrefix(char* imagesPrefix) {
	imagesPrefix = (char*) malloc(MAX_STRING * sizeof(char));
	if (imagesPrefix == NULL) {
		printf(ERROR_ALLOCAT);
		return;
	}
	printf(INPUT_IMG_PREFIX);
	if (fgets(imagesPrefix, MAX_STRING, stdin) == NULL) {
		printf(ERROR_GENERAL);
	}
}

int getNumberOfImages() {
	int numberOfImages = -1;
	printf(INPUT_NUM_IMGS);
	scanf("%d", &numberOfImages);
	if (numberOfImages < MIN_NUM_OF_IMGS) {
		printf(ERROR_NUM_OF_IMGS);
		//-1 means the user input was not valid
		numberOfImages = -1;
	}
	return numberOfImages;
}
void getImagesSuffix(char* imagesSuffixInput) {
	imagesSuffixInput = (char*) malloc(MAX_STRING * sizeof(char));
	if (imagesSuffixInput == NULL) {
		printf(ERROR_ALLOCAT);
		return;
	}
	printf(INPUT_IMG_SUFFIX);
	if (fgets(imagesSuffixInput, MAX_STRING, stdin) == NULL) {
		printf(ERROR_GENERAL);
	}
}

int getNumberOfBins() {
	int numberOfBins = -1;
	printf(INPUT_NUM_OF_BINS);
	scanf("%d", &numberOfBins);
	if (numberOfBins < MIN_NUM_OF_BINS || numberOfBins > MAX_NUM_OF_BINS) {
		printf(ERROR_NUM_OF_BINS);
		//-1 means the user input was not valid
		numberOfBins = -1;
	}
	return numberOfBins;
}
int getNumberOfFeatures() {
	int numberOfFeatures = -1;
	printf(INPUT_NUM_OF_FEATURES);
	scanf("%d", &numberOfFeatures);
	if (numberOfFeatures < MIN_NUM_OF_FEATURES) {
		printf(ERROR_NUM_OF_FEATURES);
		//-1 means the user input was not valid
		numberOfFeatures = -1;
	}
	return numberOfFeatures;
}

void validateCharAllocation(char** validationArray, int size) {
	int index = 0;
	bool found = false;
	while (!found && index < size) {
		if (validationArray[index] == NULL) {
			found = true;
		}
		index++;
	}
	if (found) {
		for (index = 0; index < size; index++) {
			if (validationArray[index] != NULL) {
			free(validationArray[index]);
		}
	}

}

}
