/*
 * sp_image_proc_util.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: soudry
 */

#include "sp_image_proc_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

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
using namespace cv;
//SPPoint** spGetRGBHist(const char* str,int imageIndex, int nBins){
//	str=NULL;
//	imageIndex=0;
//	nBins = 1;
//	nBins++;
//	return NULL;
//}
//double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB){
//	rgbHistA*=NULL*;
//	rgbHistB*=NULL*;
//	return 0;
//}
SPPoint** spGetSiftDescriptors(const char* str, int imageIndex, int nFeaturesToExtract, int *nFeatures){
	cv::Mat src;
	SPPoint** result;

	src = cv::imread(str,CV_LOAD_IMAGE_GRAYSCALE);
	if(src.empty()){
		//type error msg
		return NULL;
	}

	result = (SPPoint**) malloc(nFeaturesToExtract * sizeof(SPPoint*));

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






	return NULL;
}
//int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
//		SPPoint*** databaseFeatures, int numberOfImages,
//		int* nFeaturesPerImage){
//	kClosest=0;
//	queryFeature=NULL;
//	databaseFeatures=NULL;
//	numberOfImages=0;
//	nFeaturesPerImage=NULL;
//	return NULL;
//}
