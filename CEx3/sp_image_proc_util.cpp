/*
 * sp_image_proc_util.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */
#include <stdio.h>
#include "sp_image_proc_util.h"

SPPoint** spGetRGBHist(const char* str,int imageIndex, int nBins){
	return NULL;
}
double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB){
	return 0;
}
SPPoint** spGetSiftDescriptors(const char* str, int imageIndex, int nFeaturesToExtract, int *nFeatures){
	return NULL;
}
int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
		SPPoint*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage){
	return NULL;
}
