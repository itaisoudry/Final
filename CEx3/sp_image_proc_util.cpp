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
#include <opencv2/core.hpp>// Mat
#include <opencv2/highgui.hpp>  //imshow
#include <opencv2/imgcodecs.hpp>//imread
#include <opencv2/features2d.hpp>

#include <cstdio>

using namespace cv;
SPPoint** spGetSiftDescriptors(const char* str, int imageIndex,
		int nFeaturesToExtract, int *nFeatures) {
	cv::Mat src;
	SPPoint** result;
	//Key points will be stored in kp1;
	std::vector<cv::KeyPoint> keyPoints;
	//Feature values will be stored in ds1;
	cv::Mat descriptors;
	int index = 0;
	if (str == NULL || nFeatures == NULL || nFeaturesToExtract <= 0) {
		printf(ERROR_GENERAL);
		return NULL;
	}
	src = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if (src.empty()) {
		//TODO-change to couldn't load img+str
		printf(ERROR_LOAD_IMAGE, str);
		return NULL;
	}

	//Creating  a Sift Descriptor extractor
	cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
			cv::xfeatures2d::SIFT::create(nFeaturesToExtract);

	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(src, keyPoints, cv::Mat());
	detect->compute(src, keyPoints, descriptors);
	*nFeatures = descriptors.rows;
	result = (SPPoint**) malloc(descriptors.rows * sizeof(SPPoint*));
	if (result == NULL) {
		printf(ERROR_ALLOCAT);
		return NULL;
	}

	for (int i = 0; i < descriptors.rows; i++) {
		double data[descriptors.cols];
		for (int j = 0; j < descriptors.cols; j++) {
			data[j] = descriptors.at<double>(i, j);
		}
		result[i] = spPointCreate(data, descriptors.cols, imageIndex);
	}
	return result;
}
int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
		SPPoint*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage) {

	if (queryFeature == NULL || databaseFeatures == NULL
			|| nFeaturesPerImage == NULL || numberOfImages <= 1) {
		printf(ERROR_GENERAL);
		return NULL;
	}
	int* num = (int*)malloc(sizeof(int));
	*num=1;
	return num;



}

