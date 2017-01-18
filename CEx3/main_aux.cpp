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
		free(imagesPrefix);
		printf(ERROR_GENERAL);
	}
}

int getNumberOfImages() {
	int numberOfImages;
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
		free(imagesSuffixInput);
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

int getHistogramsAndSiftDatabase(SPPoint*** RGBHistograms,
		SPPoint*** SIFTDatabase, char* imagesPath, char* imagesSuffix,
		char* imagesPrefix, int numOfImages, int numOfBins, int numOfFeatures) {
	char imageIndex;
	RGBHistograms = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**));
	int* featuresPerImage = (int*) malloc(numOfImages * sizeof(int));
	if (featuresPerImage == NULL) {
		printf(ERROR_ALLOCAT);
		return 0;
	}
	if (RGBHistograms == NULL) {
		printf(ERROR_ALLOCAT);
		free(featuresPerImage);
		return 0;
	}
	SIFTDatabase = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**));
	if (SIFTDatabase == NULL) {
		printf(ERROR_ALLOCAT);
		free(RGBHistograms);
		return 0;
	}
	char* fullImagePath = (char*) malloc(MAX_STRING * sizeof(char));
	if (fullImagePath == NULL) {
		printf(ERROR_ALLOCAT);
		free(RGBHistograms);
		free(SIFTDatabase);
		return 0;
	}

	for (int i = 0; i < numOfImages; i++) {
		imageIndex = i + '0';
		strcpy(fullImagePath, imagesPath);
		strcat(fullImagePath, imagesPrefix);
		strcat(fullImagePath, &imageIndex);
		strcat(fullImagePath, imagesSuffix);

		RGBHistograms[i] = spGetRGBHist(fullImagePath, i, numOfBins);
		if (RGBHistograms[i] == NULL) {
			printf(ERROR_ALLOCAT);
			destroyDatabases(RGBHistograms, i - 1);
			free(featuresPerImage);
			return 0;
		}
		SIFTDatabase[i] = spGetSiftDescriptors(fullImagePath, i, numOfFeatures,
				&featuresPerImage[i]);
		if (SIFTDatabase[i] == NULL) {
			printf(ERROR_ALLOCAT);
			destroyDatabases(SIFTDatabase, i);
			free(featuresPerImage);
			return 0;
		}
		//If allocation failed - destroy everything

	}

	return 1;
}
void searchUsingGlobalFeatures(SPPoint** RGBQuery,SPPoint***RGBHistograms,int numOfImages){

	for(int i=0;i<numOfImages;i++){

	}

}
char* queryOrTerminate() {
	printf(INPUT_QUERY_OR_TERMINATE);
	char* input = (char*) malloc(MAX_STRING * sizeof(char));
	if (input == NULL) {
		printf(ERROR_ALLOCAT);
		return NULL;
	}
	printf(INPUT_IMG_SUFFIX);
	if (fgets(input, MAX_STRING, stdin) == NULL) {
		free(input);
		printf(ERROR_GENERAL);
		return NULL;
	}
	if (input[0] == '#') {
		printf(MSG_EXITING);
		return NULL;
	}
	return input;
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
		destroyValidationArray(validationArray, size);
	}

}
void destroy(SPPoint*** RGB, SPPoint*** SIFT, char* imagesPath,
		char*imagesPrefix, char*imagesSuffix, char** validationArray,
		int numOfImages) {
	destroyDatabases(RGB, numOfImages);
	destroyDatabases(SIFT, numOfImages);
	destroyInputs(imagesSuffix, imagesPrefix, imagesPath);
	destroyValidationArray(validationArray, 3);
}
void destroyDatabases(SPPoint*** arrayToDestroy, int size) {
	for (int i = 0; i < size; i++) {
		int sizeOfSubArray =
				(int) (sizeof(arrayToDestroy[i]) / sizeof(SPPoint*));
		for (int j = 0; j < sizeOfSubArray; j++) {
			spPointDestroy(arrayToDestroy[i][j]);
		}
		free(arrayToDestroy[i]);
	}
	free(arrayToDestroy);
}
void destroyHistOrSIFT(SPPoint** toDestroy) {
	int size = (int) (sizeof(toDestroy) / sizeof(SPPoint*));
	for (int i = 0; i < size; i++) {
		spPointDestroy(toDestroy[i]);
	}
	free(toDestroy);
}
void destroyInputs(char* imagesPath, char* imagesPrefix, char* imagesSuffix) {
	free(imagesPath);
	free(imagesSuffix);
	free(imagesPrefix);
}
void destroyValidationArray(char** validationArray, int size) {
	for (int index = 0; index < size; index++) {
		if (validationArray[index] != NULL) {
			free(validationArray[index]);
		}
	}
	free(validationArray);
}
