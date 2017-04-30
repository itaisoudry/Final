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

typedef struct coorToComp {
	int pointIndex;
	double coorData;
} CTC;
/**
 * Split method
 * @param KDarr - KD-Array
 * @param coor- coordinate
 * @param left - left side kd array
 * @param right - right side kd array
 * @return result value, success / failed
 */
int Split(SPKDArray* KDarr, int coor, SPKDArray* left, SPKDArray* right);

/**
 * Init method
 * @param res - result
 * @param array - SPPoint array
 * @param size - size
 * @return result value, success/failed
 */
int init(SPKDArray* res, SPPoint** array, int size);

/**
 * Comparator
 */
int comp_by_coor(const void* a, const void* b);

/**
 * @param arr - KD-Array
 */
void print_KDArr(SPKDArray* arr);

int print_coor(CTC coor);

#endif /* KD_TREE_KDARRAY_H_ */
