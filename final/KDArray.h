/*
 * KDArray.h
 *
 *  Created on: Mar 17, 2017
 *      Author: ifat
 */

#ifndef KD_TREE_KDARRAY_H_
#define KD_TREE_KDARRAY_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLogger.h"
#include "Utils/AllocationHandler.h"
#include "Utils/ResponseCodes.h"
#include "SPPoint.h"
#include <math.h>

typedef struct KD_Array {
	SPPoint** arr;
	int d;
	int n;
	int** mat;
} SPKDArray;

int Split(SPKDArray* KDarr, int coor, SPKDArray* left, SPKDArray* right);
int init(SPKDArray* res, SPPoint** array, int size);
int comp_by_coor(const void* a, const void* b);

typedef struct coorToComp {
	int pointIndex;
	int coorData;
} CTC;
void print_KDArr(SPKDArray* arr);

#endif /* KD_TREE_KDARRAY_H_ */
