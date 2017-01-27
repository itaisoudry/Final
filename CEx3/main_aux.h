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
extern "C" {
#include "SPBPriorityQueue.h"
}

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
#define MAX_FILE_NAME 20
#define CHAR_ALLOCATION_IN_MAIN 4
#define MIN_NUM_OF_BINS 1
#define MAX_NUM_OF_BINS 255
#define MIN_NUM_OF_IMGS 1
#define MIN_NUM_OF_FEATURES 1
#define NORMALIZE_FACTOR 0.33
#define POINTS_ARR_SIZE 3
#define MAX_GLOBAL_HIST_SIZE 5
#define MAX_LOCAL_HIST_SIZE 5
#define ERROR -1
#define SUCCESS 1

//functions

/*
 * getImagesPath
 * gets  Images path from the user, and saves it to imagePathInput.
 * @param imagePathInput - The pointer to the path of the images that the user will insert
 */
void getImagesPath(char** imagePathInput);
/*
 * getImagesPrefix
 * gets  Images prefix from the user, and saves it to imagesPrefix.
 * @param imagesPrefix - The pointer to the images prefix that the user will insert
 */
void getImagesPrefix(char** imagesPrefix);
/*
 * getImagesSuffix
 * gets  Images suffix  from the user, and saves it to imagesSuffixInput.
 * @param imagesSuffixInput - The pointer to the path of the images suffix that the user will insert
 */
void getImagesSuffix(char** imagesSuffixInput);
/*
 * getNumberOfImages
 * gets  the number of images from the user, and returns it.
 * @return ERROR if user input is not valid else return number of images.
 */
int getNumberOfImages();
/*
 * getNumberOfBins
 * gets  the number of Bins from the user, and returns it.
 * @return ERROR if user input is not valid else return number of Bins.
 */
int getNumberOfBins();
/*
 * getNumberOfFeatures
 * gets  the number of features from the user, and returns it.
 * @return ERROR if user input is not valid else return number of features.
 */
int getNumberOfFeatures();
/*
 * validateCharAllocation
 *checks if all allocations of the array succeeded . if they don't, it destroys all allocations before it and exit the program
 *@param validationArray - array to be validated.
 *@param the size of the array.
 */
void validateCharAllocation(char** validationArray, int size);
/*
 * destroyDatabases
 * destroys all database, free all memory
 *@param arrayToDestroy - array of database to be destroyed.
 *@param the size of the array.
 */
void destroyDatabases(SPPoint*** arrayToDestroy, int size);
/*
 * destroyInputs
 * destroys all user's inputs, free all memory
 *@param imagesPath - the path that the user inserted to be destroyed.
 *@param imagesPrefix -the images prefix that the user inserted to be destroyed
 *@param imagesSuffix -the images suffix that the user inserted to be destroyed
 */
void destroyInputs(char* imagesPath, char* imagesPrefix, char* imagesSuffix);
/*
 * searchUsingGlobalFeatures
 * computes for each RGB histogram of the images in the database the L2-Squared distans between the query image and the histograms.
 * show the best 5 image indexes.
 * @param RGBQuery - query histogram
 * @param RGBHistograms histograms of all images
 * @param numOfImages the number of images
 * @return 0 if error occurred. else prints the closest 5 images and return SUCCESS
 *
 */
int searchUsingGlobalFeatures(SPPoint** RGBQuery, SPPoint***RGBHistograms,
		int numOfImages);
/*
 * searchUsingLocalFeatures
 * computes for each sift features of the images in the database the spBestSIFTL2SquaredDistance between the query image and the images.
 * show the best 5 image indexes (closest ones).
 * @param query - query path
 * @param SIFTDatabase database of all sift features of all images
 * @param nFeatures the number of features
 * @param numOfImages - the number of images
 * @param featuresPerImage - the number of features for image
 *
 * @return 0 if error occurred. else prints the closest 5 images and return SUCCESS
 *
 */
int searchUsingLocalFeatures(SPPoint** query, SPPoint*** SIFTDatabase,
		int nFeatures, int numOfImages, int* featuresPerImage);
/*
 * queryOrTerminate
 * gets  imagesPath and query image name from the user, and saves it to query.
 * @param imagesPath - The path of the images
 * @return NULL if allocation went wrong or if user didn't insert a path,otherwise returns the user input.
 */

char* queryOrTerminate();

/* destroy
 * uses all the destroy functions and destroys everything. exiting the program.
 *@param RGB - RGB SPPoint*** to be destroyed.
 *@param SIFT- SIFT SPPoint*** to be destroyed.
 *@param imagesPath - the path to images folder to be destroyed.
 *@param imagesPrefix the prefix of the images to be destroyed.
 *@param imagesSuffix -the suffix of the images to be destroyed.
 *@param validationArray array to be destroyed.
 *@param numOfImages - the number of images.
 */

void destroy(SPPoint*** RGB, SPPoint*** SIFT, char* imagesPath,
		char*imagesPrefix, char*imagesSuffix, char** validationArray,
		int numOfImages);
/*
 * destroyValidationArrayBySize
 * frees memory of all the allocations made for the contents of the array.
 * @param validationArray -array to free
 * @param size - the size of the array.
 */
void destroyValidationArrayBySize(char** validationArray, int size);
/*
 * destroyHistOrSIFT
 * get a SPPoint** of histograms or sift, and frees all memory allocated for the contents of the point array
 * @param toDestroy -SPPoint** of histograms or sift.
 */
void destroyHistOrSIFT(SPPoint** toDestroy);
/*
 * destroyValidationArray
 * frees memory of all the allocations made for the contents of the array.
 * @param validationArray -array to free
 */
void destroyValidationArray(char** validationArray);
/*
 * getHistogramsAndSiftDatabase
 * creates sift and histograms databases
 * @param RGBHistograms - the histograms to be inserted to the database
 * @param SIFTDatabase - the sift database to be inserted to the database
 * @param imagesPath - the path to the images
 * @param imagesSuffix - the suffix of the images
 * @param imagesPrefix - the prefix of the images
 * @param numOfImages - the number of the images
 * @param numOfBins - the number of bins
 * @param numOfFeatures - the number of features
 * @return NULL if error occurred, otherwise return featuresPerImage
 */

int* getHistogramsAndSiftDatabase(SPPoint**** RGBHistograms,
		SPPoint**** SIFTDatabase, char* imagesPath, char* imagesSuffix,
		char* imagesPrefix, int numOfImages, int numOfBins, int numOfFeatures);
/**
 * Trimming the name of the image file from the path.
 * @param path - path to the file
 * @return path if allocation failed, otherwise the file name.
 */
char* trimImageNameFromPath(char* path);
#endif /* MAIN_AUX_H_ */


