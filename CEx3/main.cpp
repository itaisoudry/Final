/*
 * main.c

 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */
#include <stdlib.h>
#include "main_aux.h"
int main1(int argc, char *argv[]) {
	char* imagesPath = NULL;
	char* queryImagePath = NULL;
	char* imagesSuffix = NULL;
	char* imagesPrefix = NULL;
	int numOfImages, numOfBins, numOfFeatures;
	//This array will contain pointers to be checked-if one of them is null, all of them will be set free from memory
	char** validationArray = (char**) malloc(CHAR_ALLOCATION_IN_MAIN * sizeof(char*));
	if (validationArray == NULL) {
		printf(ERROR_GENERAL);
		return 0;
	}
	getImagesPath(imagesPath);
	getImagesPrefix(imagesPrefix);
	numOfImages = getNumberOfImages();
	getImagesSuffix(imagesSuffix);
	numOfBins = getNumberOfBins();
	numOfFeatures = getNumberOfFeatures();

	return 0;
}

