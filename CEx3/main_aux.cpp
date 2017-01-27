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
	if (scanf("%d", &numberOfImages) == EOF || numberOfImages < MIN_NUM_OF_IMGS) {
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
	if ((scanf("%d", &numberOfBins) == EOF) || numberOfBins < MIN_NUM_OF_BINS
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
	if ((scanf("%d", &numberOfFeatures) == EOF)
			|| numberOfFeatures < MIN_NUM_OF_FEATURES) {
		printf(ERROR_NUM_OF_FEATURES);
		//-1 means the user input was not valid
		numberOfFeatures = ERROR;
	}
	getchar();
	return numberOfFeatures;
}

int* getHistogramsAndSiftDatabase(SPPoint**** RGBHistograms,
		SPPoint**** SIFTDatabase, char* imagesPath, char* imagesSuffix,
		char* imagesPrefix, int numOfImages, int numOfBins, int numOfFeatures) {
	char* fullImagePath;
	char buffer[2];
	*RGBHistograms = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**));
	int* featuresPerImage = (int*) malloc(numOfImages * sizeof(int));
	if (featuresPerImage == NULL) {
		printf(ERROR_ALLOCAT);
		return NULL;
	}
	if (*RGBHistograms == NULL) {
		printf(ERROR_ALLOCAT);
		free(featuresPerImage);
		return NULL;
	}
	*SIFTDatabase = (SPPoint***) malloc(numOfImages * sizeof(SPPoint**));
	if (*SIFTDatabase == NULL) {
		printf(ERROR_ALLOCAT);
		free(RGBHistograms);
		return NULL;
	}
	fullImagePath = (char*) malloc(MAX_STRING * sizeof(char));
	if (fullImagePath == NULL) {
		printf(ERROR_ALLOCAT);
		free(*RGBHistograms);
		free(*SIFTDatabase);
		return NULL;
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
			return NULL;
		}
		(*SIFTDatabase)[i] = spGetSiftDescriptors(fullImagePath, i,
				numOfFeatures, &featuresPerImage[i]);
		if ((*SIFTDatabase)[i] == NULL) {
			destroyDatabases(*SIFTDatabase, i);
			free(featuresPerImage);
			return NULL;
		}

	}

	return featuresPerImage;
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
		if (message != SP_BPQUEUE_SUCCESS && message != SP_BPQUEUE_FULL) {
			spBPQueueDestroy(queue);
			printf(ERROR_GENERAL);
			return ERROR;
		}
	}
	//allocate queue element to peek inside the queue
	element = (BPQueueElement*) malloc(sizeof(BPQueueElement));
	if (element == NULL) {
		printf(ERROR_ALLOCAT);
		spBPQueueDestroy(queue);
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
				imagesHitCounter[results[j]]++;
			}
		}
	}
	printf(MSG_NEAREST_IMGS_LOCAL);
	int maxIndex;
	int maxCount;
	for (int i = 0; i < MAX_LOCAL_HIST_SIZE && i < numOfImages; i++) {
		maxIndex = -1;
		maxCount = 0;
		for (int j = 0; j < numOfImages; j++) {
			if (imagesHitCounter[j] > maxCount) {
				maxIndex = j;
				maxCount = imagesHitCounter[j];
			}
		}
		if (i != 0)
			printf(",");
		printf("%d", maxIndex);
		imagesHitCounter[maxIndex] = -1;
	}
	printf("\n");

	free(results);
	free(imagesHitCounter);
	return SUCCESS;

}
char* queryOrTerminate() {
	printf(INPUT_QUERY_OR_TERMINATE);
	char* input = (char*) malloc(MAX_STRING * sizeof(char));
	if (input == NULL) {
		printf(ERROR_ALLOCAT);
		return NULL;
	}
	if (fgets(input, MAX_STRING, stdin) == NULL) {
		free(input);
		printf(ERROR_GENERAL);
		return NULL;
	}
	if (input[0] == '#') {
		printf(MSG_EXITING);

		return NULL;
	}
	char* newPath = (char*) malloc(MAX_STRING * sizeof(char));
	if (newPath == NULL) {
		printf(ERROR_ALLOCAT);
		return NULL;
	}
	strcpy(newPath, "./");
	strcat(newPath, input);
	strtok(newPath, "\n");
	free(input);
	return newPath;

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
		destroyValidationArrayBySize(validationArray, size);
		exit(-1);
	}

}
void destroyValidationArray(char** validationArray) {
	for (int i = 0; i < CHAR_ALLOCATION_IN_MAIN; i++)
		free(validationArray[i]);
	free(validationArray);
}
void destroy(SPPoint*** RGB, SPPoint*** SIFT, char* imagesPath,
		char*imagesPrefix, char*imagesSuffix, char** validationArray,
		int numOfImages) {
	destroyDatabases(RGB, numOfImages);
	destroyDatabases(SIFT, numOfImages);
	destroyInputs(imagesSuffix, imagesPrefix, imagesPath);
	destroyValidationArrayBySize(validationArray, 3);
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
void destroyValidationArrayBySize(char** validationArray, int size) {
	int index;
	//if size==1 noting were allocated...
	for (index = 0; index < size; index++) {
		if (validationArray[index] != NULL) {
			free(validationArray[index]);
		}
	}
	free(validationArray);
}

char* trimImageNameFromPath(char* path) {
	char* imageName = (char*)malloc(MAX_FILE_NAME*sizeof(char));
	//if allocation failed, return path - cause the proccess is going to end anyway...
	if(imageName==NULL){
		return path;
	}
	char* token;
	token = strtok(path,"/");
	while(token!=NULL) {
		token=strtok(NULL,"/");
		if(token!=NULL) {
			strcpy(imageName,token);
		}
	}

	return imageName;
}
