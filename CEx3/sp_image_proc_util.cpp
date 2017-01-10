/*
 * sp_image_proc_util.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */

#include "sp_image_proc_util.h"
#include "main_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/core.hpp>// Mat
#include <opencv2/highgui.hpp>  //imshow
#include <opencv2/imgcodecs.hpp>//imread
#include <opencv2/features2d.hpp>

#include <cstdio>
#define MAX_NUMBER_OF_FEATURES 512
#define DIMENSION 128

using namespace cv;
SPPoint** spGetSiftDescriptors(const char* str, int imageIndex,
		int nFeaturesToExtract, int *nFeatures) {
	cv::Mat src;
	SPPoint** result;
	int index = 0;
	if (str == NULL || nFeatures == NULL || nFeaturesToExtract <= 0) {
		printf(ERROR_GENERAL);
		return NULL;
	}
	src = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if (src.empty()) {
		printf(ERROR_GENERAL);
		return NULL;
	}

	//Key points will be stored in kp1;
	std::vector<cv::KeyPoint> kp1;
	//Feature values will be stored in ds1;
	cv::Mat ds1;
	//Creating  a Sift Descriptor extractor
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
			cv::xfeatures2d::SIFT::create(MAX_NUMBER_OF_FEATURES);

	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(src, kp1, cv::Mat());
	detect->compute(src, kp1, ds1);

	result = (SPPoint**) malloc(kp1.size() * sizeof(SPPoint*));
	if (result == NULL) {
		printf(ERROR_ALLOCAT);
		return NULL;
	}
	//TODO - insert kp1.size to nFeatures
	for (; index < kp1.size(); index++) {
		SPPoint* point = spPointCreate(NULL, DIMENSION, imageIndex);

	}
	return NULL;
}

int main(){
 char* str="/home/git/CAssignment3/images/img0.png";
 int* num = (int*)malloc(sizeof(int));
 spGetSiftDescriptors(str,0,512,num);
}
