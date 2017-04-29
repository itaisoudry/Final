/*
 * main_aux.c
 *
 *  Created on: Apr 26, 2017
 *      Author: ifat
 */
#include "main_aux.h"
#include "Utils/AllocationHandler.h"
#include "Utils/ResponseCodes.h"
#include <stdbool.h>
#include <string.h>
//config msg


int saveOrLoadFeatsToFile(SPPoint*** featsArr, int numOfImages, SPConfig config,
		int* featsArrNum, bool spExtractionMode, SP_CONFIG_MSG* msg) {
	int resultValue = SUCCESS;
	FILE* file;
	int  d;
	double data;
	char filename[1024], imagePrefix[1024] , directory[1024];
	//SMART_MALLOC(char**,filename,1024*sizeof(char));
	//char directory[1024];
				   //, imagepefix[1024];
	//SMART_MALLOC(char**,directory,1024*sizeof(char));
	//SMART_MALLOC(char*,imagePrefix,1024*sizeof(char));
	memcpy(directory, config->spImagesDirectory,strlen(config->spImagesDirectory));
	if (*msg!=SP_CONFIG_SUCCESS){
		//TODO error
	}
	memcpy(imagePrefix, config->spImagesPrefix,strlen(config->spImagesPrefix));

	if (spExtractionMode) { //save
		for (int i; i < numOfImages; i++) {
			sprintf(&filename[0], "%s%s%d.%s", directory, imagePrefix, i, "feats");
			file = fopen(filename, "wb");
			if (!file) {
				*msg = SP_CONFIG_CANNOT_OPEN_FILE;
				return CFG_CANNOT_OPEN_FILE;
			}

			fwrite(&featsArrNum[i], sizeof(int), 1, file);
			for (int j = 0; j < featsArrNum[i]; j++) {
				d = spPointGetDimension(featsArr[i][j]);
				fwrite(&d, sizeof(int), 1, file);
				for (int k=0; k < d; k++) {
					data = spPointGetAxisCoor(featsArr[i][j],k);
					fwrite(&data, sizeof(double), 1, file);
				}
			}
			fclose(file);

		}
	} else { //load
		int curr;
		for (int i = 0; i < numOfImages; i++) {
			sprintf(filename, "%s%s%d.%s", directory, imagePrefix, i, "feats");
			file = fopen(filename, "rb");
			if (!file) {
				*msg = SP_CONFIG_CANNOT_OPEN_FILE;
				return CFG_CANNOT_OPEN_FILE;
			}
			fread(&curr, sizeof(int), 1, file);
			featsArrNum[i] = curr;
			int malloc_size = sizeof(SPPoint*) * curr;
			SMART_MALLOC(SPPoint**, featsArr[i], malloc_size);
			for (int j = 0; j < featsArrNum[i]; j++) {
				fread(&curr, sizeof(int), 1, file);
				d = curr;
				fread(&data, sizeof(double), d, file);
				featsArr[i][j] = spPointCreate(&data, d, i);
			}
			fclose(file);
		}
	}
	*msg = SP_CONFIG_SUCCESS;
	return resultValue;
	error:


		if (!spExtractionMode) {
		for (int i = 0; i < numOfImages; i++) {
			SMART_FREE(featsArr[i]);
		}
		}
		return resultValue;
}

const char* configMsgToString( SP_CONFIG_MSG msg){
	switch(msg){

	case 0:
			return MSG0;
	case 1:
			return MSG1;
	case 2:
			return MSG2;
	case 3:
			return MSG3;
	case 4:
			return MSG4;
	case 5:
			return MSG5;
	case 6:
			return MSG6;
	case 7:
			return MSG7;
	case 8:
			return MSG8;
	case 9:
			return MSG9;

	default : return MSG10;

	}
}
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
