/*
 * main.c

 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */
#include "main_aux.h"

int main1() {
	char* imagesPath = NULL;
	char* imagesSuffix = NULL;
	char* imagesPrefix = NULL;
	SPPoint*** RGBHistograms = NULL;
	SPPoint*** SIFTDatabase = NULL;
	int numOfImages, numOfBins, numOfFeatures, imageIndex;
	//This array will contain pointers to be checked-if one of them is null, all of them will be set free from memory
	char** validationArray = (char**) malloc(
	CHAR_ALLOCATION_IN_MAIN * sizeof(char*));
	if (validationArray == NULL) {
		printf(ERROR_GENERAL);
		return 0;
	}

	getImagesPath(imagesPath);
	validationArray[0] = imagesPath;
	validateCharAllocation(validationArray, 1);

	getImagesPrefix(imagesPrefix);
	validationArray[1] = imagesPrefix;
	validateCharAllocation(validationArray, 2);

	numOfImages = getNumberOfImages();
	getImagesSuffix(imagesSuffix);
	validationArray[2] = imagesSuffix;
	validateCharAllocation(validationArray, 3);

	numOfBins = getNumberOfBins();
	numOfFeatures = getNumberOfFeatures();

	int result = getHistogramsAndSiftDatabase(RGBHistograms, SIFTDatabase,
			imagesPath, imagesSuffix, imagesPrefix, numOfImages, numOfBins,
			numOfFeatures);
	if (result == 0) {
		destroyInputs(imagesSuffix, imagesPrefix, imagesPath);
	}
	char* query = queryOrTerminate();
	if (query == NULL) {
		destroy(RGBHistograms, SIFTDatabase, imagesSuffix, imagesPrefix,
				imagesPath, validationArray, numOfImages);
		return 0;
	}
	//as long as the user didn't terminate the program
	while (query[0] != '#') {
		int nFeatures;
		SPPoint** RGBQuery = spGetRGBHist(query, -1, numOfBins);
		SPPoint** SIFTQuery = spGetSiftDescriptors(query, -1, numOfFeatures,
				&nFeatures);
		if (RGBQuery == NULL || SIFTQuery == NULL) {
			printf (ERROR_ALOCAT);
			destroy(RGBHistograms, SIFTDatabase, imagesSuffix, imagesPrefix,
					imagesPath, validationArray, numOfImages);
			free(query);
			if (RGBQuery != NULL)
				destroyHistOrSIFT(RGBQuery)
			return 0;
		}
		searchUsingGlobalFeatures(RGBQuery,RGBHistograms,numOfImages);


	}
	printf("%d , %d , %d", numOfImages, numOfBins, numOfFeatures);

	return 0;
}

