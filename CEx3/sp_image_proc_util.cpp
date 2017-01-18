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
#define NORMALIZE_FACTOR 0.33
#define HIST_NUM 3
#define PIXELS 256

using namespace cv;
SPPoint** spGetRGBHist(const char* str, int imageIndex, int nBins){
		cv::Mat src;
		SPPoint** result =(SPPoint**) malloc(HIST_NUM * sizeof(SPPoint*));
		if (str == NULL || nBins <= 0 || imageIndex <= 0) {
			printf(ERROR_GENERAL);
			return NULL;
		}
		src = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
		if (src.empty()) {
			//TODO-change to couldn't load img+str
			printf(ERROR_LOAD_IMAGE, str);
			return NULL;
		}
		//separate the img in 3 places (b,g,r)
		std::vector<Mat> bgr_planes;
		split(src,bgr_planes);
		float range[] = {0,PIXELS};
		const float* histRange = {range};
		Mat b_hist, g_hist, r_hist;
		calcHist(&bgr_planes[0], 1, 0, Mat(),  b_hist, 1, &nBins, &histRange);
		calcHist(&bgr_planes[1], 1, 0, Mat(),  g_hist, 1, &nBins, &histRange);
		calcHist(&bgr_planes[2], 1, 0, Mat(),  r_hist, 1, &nBins, &histRange);
		double* data = (double*)malloc(nBins*sizeof(double));
		for (int i=0; i<nBins;i++){
			data[i] = r_hist.at<double>(i);
		}
		result[0]= spPointCreate(data,nBins,imageIndex);
		if (result[0]==NULL) {
			free(result);
			return NULL;
		}
		for (int i=0; i<nBins;i++){
					data[i] = g_hist.at<double>(i);
		}
		result[1]= spPointCreate(data,nBins,imageIndex);
		if (result[1]==NULL) {
					free(result[0]);
					free(result);
					return NULL;
				}

		for (int i=0; i<nBins;i++){
					data[i] = b_hist.at<double>(i);
		}
		result[2]= spPointCreate(data,nBins,imageIndex);
		if (result[2]==NULL) {
					free(result[0]);
					free(result[1]);
					free(result);
					return NULL;
				}
		free(data);
		return result;
}
double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB){

		double result;
		double sumR,sumG,sumB;
		sumR = spPointL2SquaredDistance(rgbHistA[0],rgbHistB[0]);
		sumG = spPointL2SquaredDistance(rgbHistA[1],rgbHistB[1]);
		sumB = spPointL2SquaredDistance(rgbHistA[2],rgbHistB[2]);

		result = NORMALIZE_FACTOR*sumR +NORMALIZE_FACTOR*sumG + NORMALIZE_FACTOR*sumB;
		return result;
}


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

