/*
 * main.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: soudry
 */

#include "SPImageProc.h"

extern "C" {
#include "SPConfig.h"
#include "SPLogger.h"
#include "Utils/AllocationHandler.h"
#include "Utils/ResponseCodes.h"
#include "KDTree.h"
#include "main_aux.h"
}

//Messages
#define EXIT_MSG "Exiting...\n"
#define ENTER_IMG_PATH "Please enter an image path:\n"
#define INVALID_CMD_LINE "Invalid command line: use -c %s\n"
#define ERROR_OPEN_CFG  "The configuration file %s couldn't be open\n"
#define ERROR_GENERAL_CFG "Error: couldn't create / open config file\n"
#define ERROR_OPEN_DEFAULT_CFG "The default configuration file spcbir.config couldn't be open\n"
#define ERROR_MISSING_ARGS "Error: Invalid arguments in command line\n"
#define ERROR_LOGGER "Error creating logger\n"
#define ERROR_CREATE_IMG_PRC "Error creating ImageProc\n"
#define LOGGER_CREATED "Logger created\n"

#define CONFIG_FLAG "-c"
#define DEFAULT_CFG_FILE "spcbir.config"

int main(int argc, char** argv) {
	int resultValue = SUCCESS;
	sp::ImageProc* imageProcess = NULL;
	SPConfig config = NULL;
	SP_CONFIG_MSG configMsg = SP_CONFIG_SUCCESS;
	SP_LOGGER_MSG loggerMsg = SP_LOGGER_SUCCESS;

	char configFileName[LINE_LENGTH];
	char imagePath[LINE_LENGTH];

	//variables
	SPPoint** arrayToKDARR = NULL;
	SPPoint*** imageProcessFeatures;
	int spNumOfImages, spPCADimension = 20, spNumOfFeatures, spNumOfSimilarImages, spKNN, spNumOfFeaturesTotal,
			splitMethod;
	bool spMinimalGUI;
	int* featsArr;
	int* nearestNeighbors;
	int* nearestArr;
	char* imgPathToShow;
	char logInfoMsg[100];
	int q = 0;
	SMART_MALLOC(char*, imgPathToShow, sizeof(char) * 1024);

	KDTreeNode* tree;

	//if there is a config file
	if (argc == 3) {
		//validate config flag (-c)
		if (strcmp(argv[1], CONFIG_FLAG) == 0) {

			//get config file name
			strcpy(configFileName, argv[2]);

		}

	} else if (argc == 1) { // argc != 1 or 2 -> default

		config = spConfigCreate(DEFAULT_CFG_FILE, &configMsg);

	} else { //otherwise, missing arguments in command line / too many arguments
		printf(ERROR_MISSING_ARGS);
		return EXIT_FAILURE;
	}

	//since there is no memory allocation yet, I can just terminate the proccess without memory management.
	//validate config
	if (config == NULL || configMsg != SP_CONFIG_SUCCESS) {
		printf(ERROR_GENERAL_CFG);
		return EXIT_FAILURE;
	}

	//create logger
	loggerMsg = spLoggerCreate(config->spLoggerFilename, config->spLoggerLevel);
	if (loggerMsg != SP_LOGGER_SUCCESS) {
		printf(ERROR_LOGGER);
		spConfigDestroy(config);
		return EXIT_FAILURE;
	}

	spLoggerPrintInfo(LOGGER_CREATED);

	//Create image proccess
	imageProcess = new sp::ImageProc(config);
	if (imageProcess == NULL) {
		spLoggerPrintError(configMsgToString(configMsg), __FILE__, __func__,
		__LINE__);
		spConfigDestroy(config);
		spLoggerDestroy();
		return EXIT_FAILURE;
	}

	spLoggerPrintInfo(IMG_PROC_CREATED);
	spNumOfImages = config->spNumOfImages;

	//handle extraction mode - true
	if (config->spExtractionMode == true) {
		spLoggerPrintInfo(EXTRACTION_MODE_TRUE);
		SMART_MALLOC(SPPoint***, imageProcessFeatures, sizeof(SPPoint**) * spNumOfImages);
		SMART_MALLOC(int*, featsArr, sizeof(int) * spNumOfImages);

		//now we get img path of every img and save it's features.
		for (int i = 0; i < spNumOfImages; i++) {
			configMsg = spConfigGetImagePath(imagePath, config, i);
			if (configMsg != SP_CONFIG_SUCCESS) {
				spLoggerPrintError(configMsgToString(SP_CONFIG_INVALID_ARGUMENT), __FILE__, __FUNCTION__, __LINE__);

			}
			imageProcessFeatures[i] = imageProcess->getImageFeatures(imagePath, i, &spNumOfFeatures);
			featsArr[i] = spNumOfFeatures;
		}
		SMART_FUNCTION_CALL(
				saveOrLoadFeatsToFile(imageProcessFeatures, spNumOfImages, config, featsArr, true, &configMsg)); // making file of features

	} else {	//if extraction mode false
		spLoggerPrintInfo(EXTRACTION_MODE_FALSE);
		SMART_MALLOC(SPPoint***, imageProcessFeatures, sizeof(SPPoint**) * spNumOfImages);
		SMART_MALLOC(int*, featsArr, sizeof(int) * spNumOfImages);
		SMART_FUNCTION_CALL(
				saveOrLoadFeatsToFile(imageProcessFeatures, spNumOfImages, config, featsArr, false, &configMsg)); // load file of features
	}

	spNumOfFeaturesTotal = 0;
	spLoggerPrintInfo(EXTRACTION_MODE_FINISHED);

	for (int i = 0; i < spNumOfImages; i++) {
		spNumOfFeaturesTotal += featsArr[i];
	}

	SMART_MALLOC(SPPoint**, arrayToKDARR, spNumOfFeaturesTotal * sizeof(SPPoint*));

	for (int i = 0; i < spNumOfImages; i++) {
		memcpy(&arrayToKDARR[q], imageProcessFeatures[i], sizeof(SPPoint*) * featsArr[i]);
		q += featsArr[i];
	}

	spPCADimension = config->spPCADimension;
	splitMethod = config->spKDTreeSplitMethod;
	spKNN = config->spKNN;
	SMART_MALLOC(KDTreeNode*, tree, treeSize());
	SMART_MALLOC(int*, nearestNeighbors, sizeof(int) * spKNN);

	SMART_FUNCTION_CALL(KDTreeInit(tree, arrayToKDARR, spNumOfFeaturesTotal, splitMethod));
	//pre processing phase starSted
	spLoggerPrintInfo(PREPROCESSING_STARTED);

	//pre processing phase finished
	spLoggerPrintInfo(PREPROCESSING_FINISHED);

	spLoggerPrintInfo(QUERY_USER);
	spMinimalGUI = config->spMinimalGUI;

	//please enter image path
	char input[1024];
	printf(ENTER_IMG_PATH);
	fflush(stdout);

	while (scanf("%s", input) != 0) {
		if (!strcmp(input, "<>")) {
			break;
		}

		SPPoint** queryFeats = imageProcess->getImageFeatures(input, spPCADimension + 1, &spNumOfFeatures);
		if (queryFeats == NULL) {

			sprintf(logInfoMsg,"%s is not found, try again:\n", input);
			spLoggerPrintInfo(logInfoMsg);
			continue;
		}

		spNumOfSimilarImages = config->spNumOfSimilarImages;

		SMART_MALLOC(int*, nearestArr, sizeof(int) * spKNN);
		for (int j = 0; j < spKNN; j++) {
			nearestArr[j] = 0;
		}

		for (int i = 0; i < spNumOfFeatures; i++) {
			KDTreeSearch(nearestNeighbors, tree, spKNN, queryFeats[i]);
			if (nearestNeighbors == NULL) {
				spLoggerPrintError("KDTreeSearch failed", __FILE__, __FUNCTION__, __LINE__);
				return -1;
			}
			for (int j = 0; j < spKNN; j++) {
				nearestArr[nearestNeighbors[j]] += 1;
			}
		}

		int* results;
		SMART_MALLOC(int*, results, sizeof(int) * spNumOfSimilarImages);
		for (int i = 0; i < spNumOfSimilarImages; i++) {
			results[i] = mostSimilar(spKNN, nearestArr);
			nearestArr[results[i]] = 0;
		}

		if (spMinimalGUI) {
			for (int i = 0; i < spNumOfSimilarImages; i++) {
				configMsg = spConfigGetImagePath(imgPathToShow, config, results[i]);
				if (configMsg != SP_CONFIG_SUCCESS) {
					SMART_FREE(imgPathToShow);
					SMART_FREE(nearestNeighbors);
					SMART_FREE(nearestArr);
					SMART_FREE(results);
					break;
				}
				imageProcess->showImage(imgPathToShow);
				getchar();
			}
		} else {
			sprintf(logInfoMsg,"Best candidates for - <%s> - are:\n", input);
			spLoggerPrintInfo(logInfoMsg);

			for (int i = 0; i < spNumOfSimilarImages; i++) {
				configMsg = spConfigGetImagePath(imgPathToShow, config, results[i]);
				if (configMsg != SP_CONFIG_SUCCESS) {
					SMART_FREE(imgPathToShow);
					SMART_FREE(nearestNeighbors);
					SMART_FREE(nearestArr);
					SMART_FREE(results);
					break;
				}

				sprintf(logInfoMsg,"<%s>\n", imgPathToShow);
				spLoggerPrintInfo(logInfoMsg);
			}

			SMART_FREE(imgPathToShow);
			SMART_FREE(nearestNeighbors);
			SMART_FREE(nearestArr);
			SMART_FREE(results);

		}
		printf(ENTER_IMG_PATH);
		fflush(stdout);
	}
	spLoggerPrintInfo(EXIT_MSG);
	for (int i;i<spNumOfImages;i++){
		for (int j; j<featsArr[i]; j++){
			spPointDestroy(imageProcessFeatures[i][j]);
		}
		SMART_FREE(imageProcessFeatures[i]);

	}
//	for (int i=0; i<spNumOfFeaturesTotal;i++){
//		spPointDestroy(arrayToKDARR[i]);
//	}
	SMART_FREE(arrayToKDARR);
	SMART_FREE(imageProcessFeatures);
	SMART_FREE(featsArr);
	destroyTree(tree);
	spConfigDestroy(config);
	spLoggerDestroy();

	return resultValue;
	error:
	spLoggerPrintInfo(EXIT_MSG);
	for (int i;i<spNumOfImages;i++){
		for (int j; j<featsArr[i]; j++){
			spPointDestroy(imageProcessFeatures[i][j]);
		}
		SMART_FREE(imageProcessFeatures[i]);

	}
	for (int i=0; i<spNumOfFeaturesTotal;i++){
		spPointDestroy(arrayToKDARR[i]);
	}
	SMART_FREE(arrayToKDARR);
	SMART_FREE(imageProcessFeatures);
	SMART_FREE(featsArr);
	destroyTree(tree);
	spConfigDestroy(config);

	if (resultValue == ALLOCATION_FAILED) {
		spLoggerPrintError(ERROR_MSG_ALLOCATION, __FILE__, __FUNCTION__, __LINE__);
	}else{
		spLoggerPrintError(ERROR_MSG_GENERAL, __FILE__,
								__FUNCTION__, __LINE__);
	}
	spLoggerDestroy();

	return resultValue;
}

