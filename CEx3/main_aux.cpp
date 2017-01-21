/*
 * main_aux.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */

#include "main_aux.h"

void getImagesPath(char** imagePathInput) {
	*imagePathInput = (char*) malloc(MAX_STRING * sizeof(char));
	if (*imagePathInput == NULL) {
		printf(ERROR_ALLOCAT);
		return;
	}
	printf(INPUT_IMG_DIRECTORY_PATH);
	if (fgets(*imagePathInput, MAX_STRING, stdin) == NULL) {
		free(*imagePathInput);
		printf(ERROR_GENERAL);
	}
	//drops the \n added by fgets
	strtok(*imagePathInput, "\n");
}

void getImagesPrefix(char** imagesPrefix) {
	*imagesPrefix = (char*) malloc(MAX_STRING * sizeof(char));
	if (*imagesPrefix == NULL) {
		printf(ERROR_ALLOCAT);
		return;
	}
	printf(INPUT_IMG_PREFIX);
	if (fgets(*imagesPrefix, MAX_STRING, stdin) == NULL) {
		free(*imagesPrefix);
		printf(ERROR_GENERAL);
	}
	//drops the \n added by fgets
	strtok(*imagesPrefix, "\n");

}

int getNumberOfImages() {
	int numberOfImages;
	printf(INPUT_NUM_IMGS);
	if (scanf("%d", &numberOfImages) == NULL || numberOfImages < MIN_NUM_OF_IMGS) {
		printf(ERROR_NUM_OF_IMGS);
		//-1 means the user input was not valid
		numberOfImages = ERROR;
	}
	//scanf leaves a \n afterwards which is messing up the fgets function
	getchar();
	return numberOfImages;
}
void getImagesSuffix(char** imagesSuffixInput) {
	*imagesSuffixInput = (char*) malloc(MAX_STRING * sizeof(char));
	if (imagesSuffixInput == NULL) {
		printf(ERROR_ALLOCAT);
		return;
	}
	printf(INPUT_IMG_SUFFIX);
	if (fgets(*imagesSuffixInput, MAX_STRING, stdin) == NULL) {
		free(*imagesSuffixInput);
		printf(ERROR_GENERAL);
	}
	strtok(*imagesSuffixInput, "\n");
}

int getNumberOfBins() {
	int numberOfBins = -1;
	printf(INPUT_NUM_OF_BINS);
	if (scanf("%d", &numberOfBins) == NULL || numberOfBins < MIN_NUM_OF_BINS
			|| numberOfBins > MAX_NUM_OF_BINS) {
		printf(ERROR_NUM_OF_BINS);
		//-1 means the user input was not valid
		numberOfBins = ERROR;
	}
	return numberOfBins;
}
int getNumberOfFeatures() {
	int numberOfFeatures = -1;
	printf(INPUT_NUM_OF_FEATURES);
	if (scanf("%d", &numberOfFeatures) == NULL
			|| numberOfFeatures < MIN_NUM_OF_FEATURES) {
		printf(ERROR_NUM_OF_FEATURES);
		//-1 means the user input was not valid
		numberOfFeatures = ERROR;
	}
	return numberOfFeatures;
}

int getHistogramsAndSiftDatabase(SPPoint**** RGBHistograms,
		SPPoint**** SIFTDatabase, char* imagesPath, char* imagesSuffix,
		char* imagesPrefix, int numOfImages, int numOfBins, int numOfFeatures,
		int* featuresPerImage) {
	char* fullImagePath;
	char buffer[2];
	*RGBHistograms = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**));
	featuresPerImage = (int*) malloc(numOfImages * sizeof(int));
	if (featuresPerImage == NULL) {
		printf(ERROR_ALLOCAT);
		return ERROR;
	}
	if (*RGBHistograms == NULL) {
		printf(ERROR_ALLOCAT);
		free(featuresPerImage);
		return ERROR;
	}
	*SIFTDatabase = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**));
	if (*SIFTDatabase == NULL) {
		printf(ERROR_ALLOCAT);
		free(RGBHistograms);
		return ERROR;
	}
	fullImagePath = (char*) malloc(MAX_STRING * sizeof(char));
	if (fullImagePath == NULL) {
		printf(ERROR_ALLOCAT);
		free(*RGBHistograms);
		free(*SIFTDatabase);
		return ERROR;
	}

	for (int i = 0; i < numOfImages; i++) {
		strcpy(fullImagePath, imagesPath);
		strcat(fullImagePath, imagesPrefix);
		sprintf(buffer, "%d", i);
		strcat(fullImagePath, buffer);
		strcat(fullImagePath, imagesSuffix);

		(*RGBHistograms)[i] = spGetRGBHist(fullImagePath, i, numOfBins);
		if ((*RGBHistograms)[i] == NULL) {
			destroyDatabases(*RGBHistograms, i - 1);
			free(featuresPerImage);
			return ERROR;
		}
		(*SIFTDatabase)[i] = spGetSiftDescriptors(fullImagePath, i,
				numOfFeatures, &featuresPerImage[i]);
		if ((*SIFTDatabase)[i] == NULL) {
			printf(ERROR_ALLOCAT);
			destroyDatabases(*SIFTDatabase, i);
			free(featuresPerImage);
			return ERROR;
		}

	}

	return SUCCESS;
}
//return 0 if error occured
int searchUsingGlobalFeatures(SPPoint** RGBQuery, SPPoint***RGBHistograms,
		int numOfImages) {
	BPQueueElement* element;
	SP_BPQUEUE_MSG message;
	double result;
	SPBPQueue* queue = spBPQueueCreate(MAX_GLOBAL_HIST_SIZE);
	if (queue == NULL) {
		printf(ERROR_ALLOCAT);
		return ERROR;
	}
	for (int i = 0; i < numOfImages; i++) {
		result = spRGBHistL2Distance(RGBQuery, RGBHistograms[i]);
		message = spBPQueueEnqueue(queue, i, result);
		if (message != SP_BPQUEUE_SUCCESS) {
			spBPQueueDestroy(queue);
			printf(ERROR_GENERAL);
			return ERROR;
		}
	}
	//allocate queue element to peek inside the queue
	element = (BPQueueElement*) malloc(sizeof(BPQueueElement));
	if (element == NULL) {
		printf(ERROR_ALLOCAT);
		message = spBPQueueDestroy(queue);
		return ERROR;
	}
	printf(MSG_NEAREST_IMGS_GLOBAL);
	for (int i = 0; i < MAX_GLOBAL_HIST_SIZE; i++) {
		//if queue is empty and there are less than 5 images, break.
		if (spBPQueueIsEmpty(queue)) {
			spBPQueueDestroy(queue);
			free(element);
			printf("\n");
			return ERROR;
		}
		if (i != 0)
			printf(",");
		message = spBPQueuePeek(queue, element);
		message = spBPQueueDequeue(queue);
		if (message != SP_BPQUEUE_SUCCESS) {
			spBPQueueDestroy(queue);
			free(element);
			printf("\n");
			return ERROR;
		}
		printf("%d", element->index);
	}
	//free allocated memory
	printf("\n");
	spBPQueueDestroy(queue);
	free(element);
	return SUCCESS;
}
int searchUsingLocalFeatures(SPPoint** query, SPPoint*** SIFTDatabase,
		int nFeatures, int numOfImages, int* featuresPerImage) {
	int* imagesHitCounter = (int*) malloc(numOfImages * sizeof(int));
	if (imagesHitCounter == NULL) {
		printf(ERROR_ALLOCAT);
		return ERROR;
	}
	//initialize imagesHitCounter array
	for (int i = 0; i < numOfImages; i++)
		imagesHitCounter[i] = 0;
	int* results;
	for (int i = 0; i < nFeatures; i++) {
		results = spBestSIFTL2SquaredDistance(MAX_LOCAL_HIST_SIZE, query[i],
				SIFTDatabase, numOfImages, featuresPerImage);
		if (results == NULL) {
			free(imagesHitCounter);
			return ERROR;
		}
		for (int j = 0; j < MAX_LOCAL_HIST_SIZE; j++) {
			//in case we got less than MAX_LOCAL_HIST_SIZE
			if (&results[j] != NULL) {
				imagesHitCounter[j]++;
			}
		}
	}
	//quick sort in descending order using comperator
	qsort(imagesHitCounter, numOfImages, sizeof(int), comperator);
	printf(MSG_NEAREST_IMGS_LOCAL);
	for (int i = 0; i < MAX_LOCAL_HIST_SIZE && i < numOfImages; i++) {
		if (i != 0)
			printf(",");
		printf("%d", imagesHitCounter[i]);
	}
	printf("\n");
	if (results != NULL)
		free(results);
	free(imagesHitCounter);

}
int comperator(const void * a, const void * b) {
	return (*(int*) b - *(int*) a);
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
	int index;
	//if size==1 noting were allocated...
	for (index = 0; index < size; index++) {
		if (validationArray[index] != NULL) {
			free(validationArray[index]);
		}
	}
	free(validationArray);
}
