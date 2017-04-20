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
//#include "KDTree.h"
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
	sp::ImageProc* imageProc = NULL;
	SPConfig config = NULL;
	SP_CONFIG_MSG configMsg = SP_CONFIG_SUCCESS;
	SP_LOGGER_MSG loggerMsg = SP_LOGGER_SUCCESS;

	char configFileName[LINE_LENGTH];

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

	//Create image proccess
	imageProc = new sp::ImageProc(config);
	if (imageProc == NULL) {
		printf(ERROR_CREATE_IMG_PRC);
		return EXIT_FAILURE;
	}

	//TODO - LOG IMAGE PROC CREATION ???

	//handle extraction mode - true
	if (config->spExtractionMode == true) {

	} else {	//if extraction mode false

	}

	return EXIT_SUCCESS;
}

