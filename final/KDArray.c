/*
 * KDArray.c
 *
 *  Created on: Mar 17, 2017
 *      Author: ifat

 */
#include "KDArray.h"
// coordinate printer for test
int print_coor(CTC coor) {
	char logInfoMsg[100];
	int resultValue = SUCCESS;
	sprintf(logInfoMsg,"data: %f ", coor.coorData);
	spLoggerPrintInfo(logInfoMsg);

	sprintf(logInfoMsg,"idx: %d \n", coor.pointIndex);
	spLoggerPrintInfo(logInfoMsg);

	return resultValue;
}

//for test
void print_KDArr(SPKDArray* arr) {
	char logInfoMsg[100];
	spLoggerPrintInfo("----array is:----\n");
	for (int i = 0; i < arr->d; i++) {
		for (int j = 0; j < arr->n; j++) {
			int p = (int) spPointGetAxisCoor(arr->arr[j], i);
			sprintf(logInfoMsg,"|%d|",p);
			spLoggerPrintInfo(logInfoMsg);
		}
		spLoggerPrintInfo("\n");
	}
	spLoggerPrintInfo("--------------------\n");
	spLoggerPrintInfo("----Matrix:----\n");
	for (int i = 0; i < arr->d; i++) {
		for (int j = 0; j < arr->n; j++) {
			sprintf(logInfoMsg,"|%d|", arr->mat[i][j]);
			spLoggerPrintInfo(logInfoMsg);
		}
		spLoggerPrintInfo("\n");
	}
	spLoggerPrintInfo("--------------------\n");
}

// a comparator  for the matrix sort
int comp_by_coor(const void* a, const void* b) {
	CTC* A = (CTC*) a;
	CTC* B = (CTC*) b;

	if (A->coorData > B->coorData) {
		return 1;
	} else {
		return -1;
	}
}

int init(SPKDArray* res, SPPoint** array, int size) {
	int resultValue = SUCCESS;
	SPPoint** pointsArray = NULL;
	CTC* helpArr = NULL;
	int** mat = NULL;

	spLoggerPrintInfo("KDArray init");

	SMART_MALLOC(CTC*, helpArr, size * sizeof(CTC));

	SMART_MALLOC(SPPoint**, pointsArray, sizeof(SPPoint*) * size);
	for (int i=0;i<size;i++){
		pointsArray[i]=spPointCopy(array[i]);
	}
	int d = spPointGetDimension(array[0]);
	res->d = d;
	res->n = size;

	SMART_MALLOC(int**, mat, d * sizeof(int*));


	for (int i = 0; i < size; i++) {
		SMART_MALLOC(int*, mat[i], size * sizeof(int));

		//in case of allocation failure
		if (mat[i] == NULL) {
			for (int j = 0; j < i; j++) {
				SMART_FREE(mat[j]);
			}
			break;
		}

	}
	int j = 0; // this is the coordinate var.
	while (j < d) { //build an array of ctc for every coordinate, sorts it and inserts to the suitable row in mat.
		for (int i = 0; i < size; i++) {
			helpArr[i].coorData =spPointGetAxisCoor(pointsArray[i], j);
			helpArr[i].pointIndex = i;
		}
		qsort(helpArr, size, sizeof(CTC), comp_by_coor); //sorts the temporary by the nearest ith coordinate

		for (int i = 0; i < size; i++) {
			mat[j][i] = helpArr[i].pointIndex;
		}
		j++;
	}
	res->arr = pointsArray;
	res->mat = mat;
	SMART_FREE(helpArr);

	return resultValue;
	error:
		SMART_FREE(helpArr);
		SMART_FREE(pointsArray);
		for (int i=0;i<size;i++){
			SMART_FREE(mat[i]);
		}
		SMART_FREE(mat);
		return resultValue;

}
int Split(SPKDArray* KDarr, int coor, SPKDArray* left, SPKDArray* right) {
	int resultValue=SUCCESS;
	int n = KDarr->n;
	int* X = NULL;
	int middle;
	int* IthCoor = NULL;
	int j = 0; //index for P1
	int k = 0; //index for P2
	int* map1 = NULL;
	int* map2 = NULL;
	int** mat1 = NULL;
	int** mat2 = NULL;
	SPPoint** P1 = NULL;
	SPPoint** P2 = NULL;

	spLoggerPrintInfo("Split");

	if (n % 2 == 0) {
		middle = n / 2;
	} else
		middle = (n + 1) / 2;

	SMART_MALLOC(int*,X,n*sizeof(int));


	IthCoor = KDarr->mat[coor];
	for (int i = 0; i < middle; i++) { // if i is on the left then X[i]=0
		X[IthCoor[i]] = 0;
	}

	for (int i = middle; i < n; i++) { // if i is on the right then X[i]=1
		X[IthCoor[i]] = 1;
	}
	SPPoint** arr = KDarr->arr;

	SMART_MALLOC(SPPoint**,P1,middle*sizeof(SPPoint*));
	SMART_MALLOC(SPPoint**,P2,middle*sizeof(SPPoint*));

	for (int i = 0; i < n; i++) { //P1 is the left arr and P2 is the right arr
		if (X[i] == 0) {
			P1[j] = KDarr->arr[i];
			j++;
		} else {
			P2[k] = KDarr->arr[i];
			k++;
		}
	}


	SMART_MALLOC(int*,map1,n*sizeof(int));
	SMART_MALLOC(int*,map2,n*sizeof(int));

	j = 0;
	k = 0;
	for (int i = 0; i < n; i++) { //mapping the indexes
		if (arr[i] == P1[j]) {
			map1[i] = j;
			j++;
			map2[i] = (-1);
		} else {
			map1[i] = (-1);
			map2[i] = k;
			k++;
		}

	}
	int** mat = KDarr->mat;
	int d = KDarr->d;

	SMART_MALLOC(int**,mat1,d*sizeof(int*));
	SMART_MALLOC(int**,mat2,d*sizeof(int*));

	for (int i = 0; i < d; i++) {
		SMART_MALLOC(int*,mat1[i],middle*sizeof(int));
		SMART_MALLOC(int*,mat2[i],middle*sizeof(int));
	}

	j = 0;
	k = 0;

	for (int i = 0; i < d; i++) {
		for (int l = 0; l < n; l++) {
			int p = mat[i][l];
			if (X[p] == 0) { //it means that the point is on P1 (left)
				int idx = map1[p];
				mat1[i][j] = idx;
				j++;
			} else { //its in the right arr
				int idx = map2[p];
				mat2[i][k] = idx;
				k++;
			}
		}
		j = 0;
		k = 0;
	}


	//update values
	left->arr = P1;
	right->arr = P2;
	left->n = middle;
	right->n = (n - middle);
	left->d = d;
	right->d = d;
	left->mat = mat1;
	right->mat = mat2;

	SMART_FREE(map1);
	SMART_FREE(map2);
	SMART_FREE(X);


	return resultValue;

	error:
		SMART_FREE(map1);
		SMART_FREE(map2);
		SMART_FREE(P1);
		SMART_FREE(P2);
		for (int i=0;i<n-middle;i++){
			SMART_FREE(mat1[i]);
			SMART_FREE(mat2[i]);
		}
		if (middle>(n-middle)){
			SMART_FREE(mat1[middle]);
		}
		SMART_FREE(mat1);
		SMART_FREE(mat2);

		SMART_FREE(X);
		return resultValue;


}

