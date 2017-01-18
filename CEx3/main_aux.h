/*
 * main_aux.h
 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sp_image_proc_util.h"

//Input
#define INPUT_IMG_DIRECTORY_PATH "Enter images directory path:\n"
#define INPUT_IMG_PREFIX "Enter images prefix:\n"
#define INPUT_IMG_SUFFIX "Enter images suffix:\n"
#define INPUT_NUM_IMGS "Enter number of images:\n"
#define INPUT_NUM_OF_BINS "Enter number of bins:\n"
#define INPUT_NUM_OF_FEATURES "Enter number of features:\n"
#define INPUT_QUERY_OR_TERMINATE "Enter a query image or # to terminate:\n"

//Errors
#define ERROR_NUM_OF_IMGS "An error occurred - invalid number of images\n"
#define ERROR_NUM_OF_BINS "An error occurred - invalid number of bins\n"
#define ERROR_NUM_OF_FEATURES "An error occurred - invalid number of features\n"
#define ERROR_ALLOCAT "An error occurred - allocation failure\n"
#define ERROR_LOAD_IMAGE "Image cannot be loaded - ‘%s’:\n"
#define ERROR_GENERAL "An error occurred"
//MESSAGES
#define MSG_EXITING "Exiting...\n"
#define MSG_NEAREST_IMGS_GLOBAL "Nearest images using global descriptors:\n"
#define MSG_NEAREST_IMGS_LOCAL "Nearest images using local descriptors:\n"
//General
#define MAX_STRING 1024
#define CHAR_ALLOCATION_IN_MAIN 4
#define MIN_NUM_OF_BINS 1
#define MAX_NUM_OF_BINS 255
#define MIN_NUM_OF_IMGS 1
#define MIN_NUM_OF_FEATURES 1
#define NORMALIZE_FACTOR 0.33
#define POINTS_ARR_SIZE 3

//functions
void getImagesPath(char *imagePathInput);
void getImagesPrefix(char *imagesPrefix);
int getNumberOfImages();
void getImagesSuffix(char* imagesSuffixInput);
int getNumberOfBins();
int getNumberOfFeatures();
void validateCharAllocation(char** validationArray, int size);
void destroyDatabases(SPPoint*** arrayToDestroy, int size);
void destroyInputs(char* imagesPath, char* imagesPrefix, char* imagesSuffix);
char* queryOrTerminate();
void validateCharAllocation(char** validationArray, int size);
void destroy(SPPoint*** RGB, SPPoint*** SIFT, char* imagesPath,
		char*imagesPrefix, char*imagesSuffix, char** validationArray,
		int numOfImages);
void destroyDatabases(SPPoint*** arrayToDestroy, int size);
void destroyInputs(char* imagesPath, char* imagesPrefix, char* imagesSuffix);
void destroyValidationArray(char** validationArray, int size);
void destroyHistOrSIFT(SPPoint** toDestroy);

/**
 * creates sift and histograms databases
 * @return 0 if error occurred 1 otherwise
 */

int getHistogramsAndSiftDatabase(SPPoint*** RGBHistograms,
		SPPoint*** SIFTDatabase, char* imagesPath, char* imagesSuffix,
		char* imagesPrefix, int numOfImages, int numOfBins, int numOfFeatures);
#endif /* MAIN_AUX_H_ */
