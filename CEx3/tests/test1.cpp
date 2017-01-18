/*
 * sp_image_proc_util_test.c
 *
 *  Created on: Jan 10, 2017
 *      Author: soudry
 */
#include "../sp_image_proc_util.h"
#include "../main_aux.h"
#define IMG0 "/home/soudry/git/CAssignment3/CEx3/images/img0.png"
#define IMG1 "/home/soudry/git/CAssignment3/CEx3/images/img1.png"
#define IMG2 "/home/soudry/git/CAssignment3/CEx3/images/img2.png"
#define IMG3 "/home/soudry/git/CAssignment3/CEx3/images/img3.png"

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
int main1() {
	spGetSiftDescriptorsTest();
	spBestSIFTL2SquaredDistanceTest();
}
