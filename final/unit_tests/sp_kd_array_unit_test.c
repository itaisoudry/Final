/*
 * mainKDArrays.c
 *
 *  Created on: Mar 23, 2017
 *      Author: ifat
 */

#include <stdio.h>

#include "../KDArray.h"
#include <stdlib.h>

#include "../KDTree.h"


int main2( int argc, const char* argv[] )
{
	int resultValue =SUCCESS;
	double dataA[3]={1,2,3};
	double dataB[3]={4,1,7};
	double dataC[3]={0,8,9};
	double dataD[3]={3,28,6};
	double dataE[3]={2,3,0};

	double* data= &dataA[0];

	SPPoint* a = spPointCreate(data,3,0);

	data= dataB;
	SPPoint* b = spPointCreate(data,3,1);
	data= dataC;

	SPPoint* c = spPointCreate(data,3,2);
	data= dataD;

	SPPoint* d = spPointCreate(data,3,3);
	data= dataE;

	SPPoint* e = spPointCreate(data,3,4);



	SPPoint* arr[5]= {a,b,c,d,e};
	SPPoint** ar=&arr[0];
	SPKDArray* kdarr;
	SMART_MALLOC(SPKDArray*,kdarr,sizeof(SPKDArray));
	SMART_FUNCTION_CALL(init(kdarr,ar,5));
	printf("this is the initial array:\n");
	print_KDArr(kdarr);
	SPKDArray* left =(SPKDArray*) malloc (sizeof(SPKDArray*));
	SPKDArray* right =(SPKDArray*) malloc (sizeof(SPKDArray*));

	Split(kdarr,0,left,right);
	printf("this is the left array with respect to 0th dim\n");
	print_KDArr(left);


	printf("this is the right array with respect to 0th dim\n");
	print_KDArr(right);

	printf("this is the initial array:\n");
	print_KDArr(kdarr);

	Split(kdarr,1,left,right);
	printf("this is the left array with respect to 1th dim\n");
	print_KDArr(left);


	printf("this is the right array with respect to 1th dim\n");
	print_KDArr(right);

	printf("this is the initial array:\n");
	print_KDArr(kdarr);

	Split(kdarr,2,left,right);
	printf("this is the left array with respect to 2th dim\n");
	print_KDArr(left);


	printf("this is the right array with respect to 2th dim\n");
	print_KDArr(right);

	printf("this is the initial array:\n");
	print_KDArr(kdarr);
	//KDTKDTreeInit(ar,  5,  3,  MAX_SPREAD);
	SMART_FREE(kdarr);

	return resultValue;
	error:
	SMART_FREE(kdarr);
	return resultValue;
}
