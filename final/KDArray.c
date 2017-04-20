/*
 * KDArray.c
 *
 *  Created on: Mar 17, 2017
 *      Author: ifat

 */
#include "KDArray.h"

void print_coor(CTC coor) {
	printf("data: %d ", coor.coorData);
	printf("idx: %d \n", coor.pointIndex);
}

void print_KDArr(SPKDArray* arr) {
	printf("----array is:----\n");

	for (int i = 0; i < arr->d; i++) {
		for (int j = 0; j < arr->n; j++) {
			int p = (int) spPointGetAxisCoor(arr->arr[j], i);
			printf("|%d|", p);
		}
		printf("\n");
	}
	printf("--------------------\n");
	printf("----mat is:----\n");
	for (int i = 0; i < arr->d; i++) {
		for (int j = 0; j < arr->n; j++) {
			printf("|%d|", arr->mat[i][j]);
		}
		printf("\n");
	}
	printf("--------------------\n");
}

int comp_by_coor(const void* a, const void* b) {
	CTC* A = (CTC*) a;
	CTC* B = (CTC*) b;
	if (A->coorData > B->coorData) {
		return 1;
	} else {
		return -1;
	}
}
int mallocCheck(void* p) {
	if (p == NULL) {
		free(p);
		return -1;
	}

	return 1;
}

SPKDArray* init(SPPoint** array, int size) {
	int resultValue = SUCCESS;
	SPKDArray* res = NULL;
	SPPoint** pointsArray = NULL;
	CTC* helpArr = NULL;
	int** mat = NULL;

	SMART_MALLOC(SPKDArray*, res, sizeof(SPKDArray));

//	res = (SPKDArray*) malloc(sizeof(SPKDArray));
	if (mallocCheck(res) == (-1)) {
		return NULL;
	}

	SMART_MALLOC(CTC*, helpArr, size * sizeof(CTC));
//	helpArr = (CTC*) malloc(size * sizeof(CTC));
	if (mallocCheck(helpArr) == (-1)) {
		return NULL;
	}

	SMART_MALLOC(SPPoint**, pointsArray, sizeof(SPPoint*) * size);
//	arr = (SPPoint**) malloc(sizeof(SPPoint*) * size);
	if (mallocCheck(pointsArray) == (-1)) {
		return NULL;
	}

	memcpy(pointsArray, array, sizeof(SPPoint*) * size);
	int d = spPointGetDimension(array[0]);
	res->d = d;
	res->n = size;

	SMART_MALLOC(int**, mat, d * sizeof(int*));
//	int** mat = (int**) malloc(sizeof(int*) * d);
	if (mallocCheck(mat) == (-1)) {
		return NULL;
	}

	for (int i = 0; i < size; i++) {
		SMART_MALLOC(int*, mat[i], size * sizeof(int));

		//in case of allocation failure
		if (mat[i] == NULL) {
			for (int j = 0; j < i; j++) {
				SMART_FREE(mat[j]);
			}
		}
//		mat[i] = (int*) malloc(sizeof(int) * size);
		if (mallocCheck(mat[i]) == (-1)) {
			return NULL;
		}
	}

	int j = 0; // this is the coordinate var.
	while (j < d) { //build an array of ctc for every coordinate, sorts it and inserts to the suitable row in mat.
		for (int i = 0; i < size; i++) {
			helpArr[i].coorData = spPointGetAxisCoor(pointsArray[i], j);
			helpArr[i].pointIndex = i;
		}
		qsort(helpArr, size, sizeof(CTC), comp_by_coor);

		for (int i = 0; i < size; i++) {
			mat[j][i] = helpArr[i].pointIndex;
		}
		j++;
	}
	res->arr = pointsArray;  // needs malloc?
	res->mat = mat;
	free(helpArr);

	return res;
	//TODO should return a SPKDArray!!!

	error:
	SMART_FREE(res);
	SMART_FREE(helpArr);
	SMART_FREE(pointsArray);
	SMART_FREE(mat);
	//add return

}
int Split(SPKDArray* KDarr, int coor, SPKDArray* left, SPKDArray* right) {
	int resultValue=0;
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

	if (n % 2 == 0) {
		middle = n / 2;
	} else
		middle = (n + 1) / 2;

	SMART_MALLOC(int*,X,n*sizeof(int));
	//X = (int*) malloc(n * sizeof(int));
	if (mallocCheck(X) == (-1)) {
		return -1;
	}

	IthCoor = KDarr->mat[coor];
	for (int i = 0; i < middle; i++) {
		//int j = IthCoor[i];
		X[IthCoor[i]] = 0;
	}

	for (int i = middle; i < n; i++) {
		//int j= IthCoor[i];
		X[IthCoor[i]] = 1;
	}
	SPPoint** arr = KDarr->arr;

	SMART_MALLOC(SPPoint**,P1,middle*sizeof(SPPoint*));
	//P1 = (SPPoint**) malloc(sizeof(SPPoint**) * middle);
	if (mallocCheck(P1) == (-1)) {
		return -1;
	}

	SMART_MALLOC(SPPoint**,P2,middle*sizeof(SPPoint*));
	//P2 = (SPPoint**) malloc(sizeof(SPPoint**) * middle);
	if (mallocCheck(P2) == (-1)) {
		return -1;
	}

	for (int i = 0; i < n; i++) {
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

//	map1 = (int*) malloc(sizeof(int) * n);
//	map2 = (int*) malloc(sizeof(int) * n);
	if (mallocCheck(map1) == (-1) || mallocCheck(map2) == (-1)) {
		return -1;
	}

	j = 0;
	k = 0;
	for (int i = 0; i < n; i++) {
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

//	mat1 = (int**) malloc(sizeof(int*) * d);
//	mat2 = (int**) malloc(sizeof(int*) * d);
	if (mallocCheck(mat1) == (-1) || mallocCheck(mat2) == (-1)) {
		return -1;
	}

	for (int i = 0; i < d; i++) {
		SMART_MALLOC(int*,mat1[i],middle*sizeof(int));
		SMART_MALLOC(int*,mat2[i],middle*sizeof(int));
		//TODO - add allocation check for array

//		mat1[i] = (int*) malloc(sizeof(int) * middle);
//		mat2[i] = (int*) malloc(sizeof(int) * middle);
		if (mallocCheck(mat1[i]) == (-1) || mallocCheck(mat2[i]) == (-1)) {
			return -1;
		}
	}
	//SMART_MALLOC((int**),mat1,sizeof(int)*middle*d);
	//SMART_MALLOC((int**),mat2,sizeof(int)*(n-middle/)*d;
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

	left->arr = P1;
	right->arr = P2;
	left->n = middle;
	right->n = (n - middle);
	left->d = d;
	right->d = d;
	left->mat = mat1;
	right->mat = mat2;

	free(map1);
	free(map2);
	free(X);


	//wtf?
	int returnValue = 0;
	return returnValue = 0;

	error:
		SMART_FREE(map1);
		SMART_FREE(map2);
		SMART_FREE(P1);
		SMART_FREE(P2);
		SMART_FREE(mat1);
		SMART_FREE(mat2);
		SMART_FREE(X);
		return resultValue;

}

