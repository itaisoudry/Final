/*
 * sp_image_proc_util_test.c
 *
 *  Created on: Jan 10, 2017
 *      Author: soudry
 */
#include "../sp_image_proc_util.h"
#include "../main_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IMG0 "/home/soudry/git/CAssignment3/CEx3/images/img0.png"
#define IMG1 "/home/soudry/git/CAssignment3/CEx3/images/img1.png"
#define IMG2 "/home/soudry/git/CAssignment3/CEx3/images/img2.png"
#define IMG3 "/home/soudry/git/CAssignment3/CEx3/images/img3.png"
#define QUERYA "/home/soudry/git/CAssignment3/CEx3/queryA.png"
#define QUERYB "/home/soudry/git/CAssignment3/CEx3/queryB.png"
#define QUERYC "/home/soudry/git/CAssignment3/CEx3/queryC.png"
void spGetSiftDescriptorsTest() {
	int* num = (int*) malloc(sizeof(int));
	*num = 0;
	SPPoint** result = spGetSiftDescriptors(IMG0, 0, 512, num);
	if (result == NULL)
		printf("FAILED\n");
	else
		printf("SUCCESS\n");
}
void spBestSIFTL2SquaredDistanceTest() {
	SPPoint*** databaseFeatures = (SPPoint***) malloc(sizeof(SPPoint**));
	int* nFeaturesPerImage = (int*) malloc(4 * sizeof(int));
	databaseFeatures[0] = spGetSiftDescriptors(IMG0, 0, 512,
			&nFeaturesPerImage[0]);
	databaseFeatures[1] = spGetSiftDescriptors(IMG1, 1, 512,
			&nFeaturesPerImage[1]);
	databaseFeatures[2] = spGetSiftDescriptors(IMG2, 2, 512,
			&nFeaturesPerImage[2]);
	databaseFeatures[3] = spGetSiftDescriptors(IMG3, 3, 512,
			&nFeaturesPerImage[3]);
	SPPoint* queryFeature = spPointCopy(databaseFeatures[0][0]);
	int* result = spBestSIFTL2SquaredDistance(3, queryFeature, databaseFeatures,
			4, nFeaturesPerImage);
	if (result != NULL)
		printf("SUCCESS\n");
	else
		printf("FAILED\n");
}
void getDatabasesTest() {
	int numOfImages = 17;
	int numOfBins = 16;
	int numOfFeatures = 100;
	int* featuresPerImage = NULL;
	char imagesPath[] = "/home/soudry/git/CAssignment3/CEx3/images/";
	char imagesSuffix[] = ".png";
	char imagesPrefix[] = "img";
	SPPoint*** RGBHistograms = (SPPoint***) malloc(
			numOfImages * sizeof(SPPoint**));
	SPPoint*** SIFTDatabase = (SPPoint***) malloc(
			numOfImages * sizeof(SPPoint**));
	;

	int result = getHistogramsAndSiftDatabase(&RGBHistograms, &SIFTDatabase,
			imagesPath, imagesSuffix, imagesPrefix, numOfImages, numOfBins,
			numOfFeatures, featuresPerImage);
	if (result == 1)
		printf("SUCCESS");
	else {
		printf("FAILED");
		return;
	}

	int nFeatures=-1;
	SPPoint** RGBQuery = spGetRGBHist(QUERYA, 0, numOfBins);
	SPPoint** SIFTQuery = spGetSiftDescriptors(QUERYA, 0, numOfFeatures,
			&nFeatures);
	if (RGBQuery == NULL || SIFTQuery == NULL) {
		printf("FAILED");
		return;
	}
	result = searchUsingGlobalFeatures(RGBQuery, RGBHistograms,
			numOfImages);
	if (result == -1) {
		printf("FAILED");
		return;
	}
	result = searchUsingLocalFeatures(SIFTQuery, SIFTDatabase, nFeatures,
			numOfImages, featuresPerImage);
	if (result != -1) {
		printf("SUCCESS");
	}

}
int main() {
//	spGetSiftDescriptorsTest();
//	spBestSIFTL2SquaredDistanceTest();
	getDatabasesTest();

//	for (int i = 1; i < 3; i++) {
//		char str[80]="";
//		char imageIndex = i+'0';
//		printf("%c\n",imageIndex);
//		strcpy(str, "img");
//		char buffer[2];
//		sprintf(buffer, "%d", i);
//		strcat(str,buffer);
//		printf("%s\n",str);
//}
}
