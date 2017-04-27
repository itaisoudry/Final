/*
 * main.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: soudry
 */

#include "SPImageProc.h"

extern "C" {
#include "SPLogger.h"
#include "Utils/AllocationHandler.h"
#include "Utils/ResponseCodes.h"
#include "SPConfig.h"
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
	SPPoint** arrayToKDARR;
	SPPoint*** imageProcessFeatures;
	int spNumOfImages, spPCADimension=20, spNumOfFeatures, spNumOfSimilarImages, spKNN, spNumOfFeaturesTotal, splitMethod;
	bool spMinimalGUI;
	int* featsArr ;
	int* nearestNeighbors;
	int* histogram;
	char* imgPathToShow;
	SMART_MALLOC(char*,imgPathToShow,sizeof(char)*1024);


	KDTreeNode* tree ;

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
		return EXIT_FAILURE;
	}
	//TODO - LOGGER INITIALIZE MSG
	//spLoggerPrintInfo (LOGGER_CREATED);

	//Create image proccess
	imageProcess = new sp::ImageProc(config);
	if (imageProcess == NULL) {
		spLoggerPrintError(configMsgToString(configMsg), __FILE__, __func__, __LINE__);
		return EXIT_FAILURE;
	}

	//TODO - is this really needed?
	spLoggerPrintInfo(IMG_PROC_CREATED);
	spNumOfImages = config->spNumOfImages;
	//handle extraction mode - true
	if (config->spExtractionMode == true) {
		spLoggerPrintInfo(EXTRACTION_MODE_TRUE);
		SMART_MALLOC(SPPoint***,imageProcessFeatures,sizeof(SPPoint**)*spNumOfImages);
		SMART_MALLOC(int*,featsArr,sizeof(int)*spNumOfImages);
		//now we get img path of every img and save it's features.
		for (int i=0;i< spNumOfImages;i++){
			configMsg=spConfigGetImagePath(imagePath,config,i);
			if (configMsg != SP_CONFIG_SUCCESS){
				spLoggerPrintError(configMsgToString(SP_CONFIG_INVALID_ARGUMENT),__FILE__,__FUNCTION__,__LINE__);

			}
			imageProcessFeatures[i]= imageProcess->getImageFeatures(imagePath,i,&spNumOfFeatures);
			featsArr[i] = spNumOfFeatures;
		}
		SMART_FUNCTION_CALL(saveOrLoadFeatsToFile(imageProcessFeatures,spNumOfImages,config,featsArr,true,&configMsg)); // making file of features




	} else {	//if extraction mode false
		spLoggerPrintInfo(EXTRACTION_MODE_FALSE);
		SMART_MALLOC(SPPoint***,imageProcessFeatures,sizeof(SPPoint**)*spNumOfImages);
		SMART_MALLOC(int*,featsArr,sizeof(int)*spNumOfImages);
		SMART_FUNCTION_CALL(saveOrLoadFeatsToFile(imageProcessFeatures,spNumOfImages,config,featsArr,false,&configMsg)); // load file of features
	}
	spNumOfFeaturesTotal=0;
	spLoggerPrintInfo(EXTRACTION_MODE_FINISHED);
	int q=0;
	for (int i=0;i<spNumOfImages;i++){
		spNumOfFeaturesTotal += featsArr[i];
		memcpy(&arrayToKDARR[q],imageProcessFeatures[i],sizeof(SPPoint**)*featsArr[i]);
		q+=featsArr[i];
	}
	spPCADimension = config ->spPCADimension;
	splitMethod = config->spKDTreeSplitMethod;
	spKNN = config->spKNN;
	SMART_MALLOC(KDTreeNode*,tree, treeSize());
	SMART_MALLOC(int*,nearestNeighbors, sizeof(int));

	SMART_FUNCTION_CALL(KDTreeInit(tree,arrayToKDARR,spNumOfFeaturesTotal,spPCADimension,splitMethod));
	//pre processing phase started
	spLoggerPrintInfo(PREPROCESSING_STARTED);

	//pre processing phase finished
	spLoggerPrintInfo(PREPROCESSING_FINISHED);

	spLoggerPrintInfo(QUERY_USER);

	//please enter image path
	char* input ;
	SMART_MALLOC(char*,input,sizeof(char)*1024);
	printf(ENTER_IMG_PATH);
	fflush(stdout);
	while (scanf("%s",input)!=0){
		if(!strcmp(input,"<>")){
			return SUCCESS;
		}
		SPPoint** queryFeats =  imageProcess->getImageFeatures(input,1024,&spNumOfFeatures);
		if(queryFeats==NULL){
			printf("%s is not found, try again:\n",input);
			continue;
		}

		SMART_MALLOC(int*,histogram,sizeof(int)*spNumOfImages);

		for(int i=0;i<spNumOfFeatures;i++){
			SMART_FUNCTION_CALL(KDTreeSearch(nearestNeighbors,tree,spKNN,queryFeats[i]));
			if(nearestNeighbors==NULL){
				spLoggerPrintError("KDTreeSearch failed", __FILE__,
										__FUNCTION__, __LINE__);
				return -1;
			}
			for(int j=0;j<spKNN; j++){
				histogram[nearestNeighbors[j]]+=1;
			}
		}
		qsort(histogram,spNumOfImages,sizeof(int),cmpfunc);
		spNumOfSimilarImages = config -> spNumOfSimilarImages;

		if(spMinimalGUI){
			for (int i=0;i<spNumOfSimilarImages;i++){
				configMsg =spConfigGetImagePath(imgPathToShow,config,histogram[i]);
				if(configMsg!= SP_CONFIG_SUCCESS){
					break;
				}
				imageProcess->showImage(imgPathToShow);
				getchar();
			}
		}else{
		printf("Best candidates for - <%s> - are:\n", input);
		for (int i=0;i<spNumOfSimilarImages;i++){
				configMsg =spConfigGetImagePath(imgPathToShow,config,histogram[i]);
				if(configMsg!= SP_CONFIG_SUCCESS){
					break;
				}
				printf("<%s>\n",imgPathToShow);
				fflush(stdout);
			}

		}
		printf(ENTER_IMG_PATH);
		fflush(stdout);
	}
	printf("Exiting...\n");
	SMART_FREE(imgPathToShow);
	SMART_FREE(imageProcessFeatures);
	SMART_FREE(featsArr);
	SMART_FREE(tree);
	SMART_FREE(nearestNeighbors);
	SMART_FREE(input);
	SMART_FREE(histogram);

	return resultValue;
	error:
		SMART_FREE(imgPathToShow);
		SMART_FREE(imageProcessFeatures);
		SMART_FREE(featsArr);
		SMART_FREE(tree);
		SMART_FREE(nearestNeighbors);
		SMART_FREE(input);
		SMART_FREE(histogram);
		return resultValue;
}


