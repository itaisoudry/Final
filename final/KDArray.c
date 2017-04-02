/*
 * KDArray.c
 *
 *  Created on: Mar 17, 2017
 *      Author: ifat

 */
#include "KDArray.h"

void print_coor(CTC coor){
	printf("data: %d ",coor.coorData);
	printf("idx: %d \n",coor.pointIndex);
}
void print_KDArr(SPKDArray* arr){
	printf("----array is:----\n");
	for(int i=0;i<arr->d;i++){
		for(int j=0;j<arr->n;j++){
			int p=(int) spPointGetAxisCoor(arr->arr[j], i);
			printf("|%d|",p);
		}
		printf("\n");
	}
	printf("--------------------\n");
	printf("----mat is:----\n");
		for(int i=0;i<arr->d;i++){
			for(int j=0;j<arr->n;j++){
				printf("|%d|",arr->mat[i][j]);
			}
			printf("\n");
		}
		printf("--------------------\n");
}

int comp_by_coor(const void* a, const void* b){
	CTC* A = (CTC*) a;
	CTC* B = (CTC*) b;
	if (A->coorData > B->coorData){
		return 1;
	}
	else{
		return  -1;
	}
}
int mallocCheck
(void* p){
	if (p==NULL){
		free (p);
		return -1;
	}
	return 1;
}


SPKDArray* init(SPPoint** array,int size) {
	 SPKDArray* res = NULL;
//	 int returnValue=0;
	 res = (SPKDArray*) malloc(sizeof(SPKDArray));
	 if(mallocCheck(res)==(-1))
		 {return NULL;}
	 CTC* helpArr = NULL;
	 helpArr = (CTC*) malloc(size*sizeof(CTC));
	 if(mallocCheck(helpArr)==(-1))
	 		 {return NULL;}
//	 SMART_MALLOC(CTC**,helpArr,size*sizeof(CTC**));

	 SPPoint** arr = NULL;
	 arr= (SPPoint**) malloc(sizeof(SPPoint*)*size);
	 if(mallocCheck(arr)==(-1))
		 		 {return NULL;}
	 //for(int i=0;i<size;i++){
//		 arr[i]=(SPPoint*) malloc(sizeof(SPPoint));
//		 if(mallocCheck(arr[i])==(-1))
//		 	 {return NULL;}
//	 }


	 memcpy(arr,array,sizeof(SPPoint*)*size);
	 int d = spPointGetDimension(array[0]);
	 res-> d =d;
	 res-> n = size;
//	 for (int i=0; i<size; i++){ // copying the array and also bulid the first row of mat
//		arr[i] = spPointCopy(array[i]);
//		helpArr[i].coorData = spPointGetAxisCoor(arr[i],0);
//		helpArr[i].pointIndex = i;
//	 }
//	 qsort(helpArr,size,sizeof(CTC),comp_by_coor); // sorts for the 0 coordinate
//	 for (int i=0;i<size;i++){
//		 print_coor(helpArr[i]);
//	 }
	 int** mat = (int**) malloc(sizeof(int*)*d);
	 if(mallocCheck(mat)==(-1))
	 	 {return NULL;}
	 for (int i=0; i<size;i++){
		 mat[i]= (int*) malloc(sizeof(int)*size);
		 if(mallocCheck(mat[i])==(-1))
		 	 	 {return NULL;}
	 }
//	 for (int i=0; i<size; i++){ // inserts the sorted indexes to the mat (in row 0)
//		int t= helpArr[i].pointIndex;
//		mat[0][i] = t;
//	 }
	 int j=0; // this is the coordinate var.
	 while (j<d){  //build an array of ctc for every coordinate, sorts it and inserts to the suitable row in mat.
		 	 for (int i=0; i<size; i++){
		 		helpArr[i].coorData = spPointGetAxisCoor(arr[i],j);
		 		helpArr[i].pointIndex = i;
		 	 }
		 	 qsort(helpArr,size,sizeof(CTC),comp_by_coor);

		 	 for (int i=0; i<size; i++){
		 		mat[j][i] = helpArr[i].pointIndex;
		 	 }
		 j++;
	 }
	 res->arr=arr;// needs malloc?
	 res-> mat=mat;
	 free(helpArr);

	 return res;
	 //TODO should return a SPKDArray!!!
 }
int Split(SPKDArray* KDarr, int coor,SPKDArray* left, SPKDArray* right){
	int n = KDarr->n;
	int middle;
	if(n%2==0){
		middle = n/2;
	}
	else
		middle =(n+1)/2;
	int* X =NULL;
	//SMART_MALLOC((int*),X,sizeof(int)*n);

	 X = (int*) malloc(n*sizeof(int));
	 if(mallocCheck(X)==(-1))
		 		 {return -1;}
	int* IthCoor = KDarr->mat[coor];
	for (int i=0; i<middle; i++){
		//int j = IthCoor[i];
		X[IthCoor[i]]=0;
	}
	for (int i=middle; i<n; i++){
		//int j= IthCoor[i];
		X[IthCoor[i]]=1;
	}
	SPPoint** arr = KDarr->arr;
	SPPoint** P1 =NULL;
	SPPoint** P2 = NULL;
	//SMART_MALLOC((SPPoint*),P1,sizeof(SPPoint)*middle);
	//SMART_MALLOC((SPPoint*),P2,sizeof(SPPoint)*(n-middle));

	P1 = (SPPoint**) malloc(sizeof(SPPoint**)*middle);
	if(mallocCheck(P1)==(-1))
		{return -1;}
	P2 = (SPPoint**) malloc(sizeof(SPPoint**)*middle);
	if(mallocCheck(P2)==(-1))
		{return -1;}
	int j=0; //index for P1
	int k=0; //index for P2
	for (int i=0; i<n; i++){
		if (X[i]==0){
			P1[j]= KDarr->arr[i];
			j++;
		}
		else{
			P2[k] = KDarr->arr[i];
			k++;
		}
	}
	int* map1 = NULL;
	int* map2 =NULL;
	map1 = (int*) malloc(sizeof(int)*n);
	map2 = (int*) malloc(sizeof(int)*n);
	if(mallocCheck(map1)==(-1)|| mallocCheck(map2)==(-1))
			{return -1;}
	//SMART_MALLOC((int*),P1,sizeof(int)*n);
	//SMART_MALLOC((int*),P2,sizeof(int)*n);

	j=0;
	k=0;
	for (int i=0;i<n;i++){
		if (arr[i]==P1[j]){
			map1[i]=j;
			j++;
			map2[i]= (-1);
		}
		else {
			map1[i]=(-1);
			map2[i]= k;
			k++;
		}

	}
	int** mat = KDarr->mat;
	int d = KDarr->d;

	int** mat1 =NULL;
	int** mat2 = NULL;

	mat1 = (int**) malloc(sizeof(int*)*d);
	mat2 = (int**) malloc(sizeof(int*)*d);
	if(mallocCheck(mat1)==(-1)|| mallocCheck(mat2)==(-1))
			{return -1;}
	for(int i=0;i<d;i++){
		mat1[i] = (int*) malloc(sizeof(int)*middle);
		mat2[i] = (int*) malloc(sizeof(int)*middle);
		if(mallocCheck(mat1[i])==(-1)|| mallocCheck(mat2[i])==(-1))
					{return -1;}
	}
	//SMART_MALLOC((int**),mat1,sizeof(int)*middle*d);
	//SMART_MALLOC((int**),mat2,sizeof(int)*(n-middle/)*d;
	j=0;
	k=0;

	for(int i=0;i<d;i++){
		for(int l=0;l<n;l++){
			int p = mat[i][l];
			if(X[p]==0){ //it means that the point is on P1 (left)
				int idx= map1[p];
				mat1[i][j]=idx;
				j++;
			}
			else{ //its in the right arr
				int idx =map2[p];
				mat2[i][k]=idx;
				k++;
			}
		}
		j=0;
		k=0;
	}

	left->arr = P1;
	right->arr =P2;
	left->n = middle;
	right->n = (n-middle);
	left-> d =d;
	right -> d = d;
	left-> mat=mat1;
	right->mat=mat2;


	free(map1);
	free(map2);
	free(X);


	int returnValue=0;
	return 	 returnValue=0;

}


