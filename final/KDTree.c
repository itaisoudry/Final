/*
 * KD-TREE.c
 *
 *  Created on: Apr 6, 2017
 *      Author: ifat
 */
#include "KDTree.h"

#include "SPBPriorityQueue.h"
#define isLeaf(x) (((x->dim) == -1)?  1 : 0)//if the dim of node is (-1) its a leaf

#define LEFT 0
#define RIGHT 1


struct KD_Tree_NODE{
	int dim;
	double val;
	struct KD_Tree_NODE* left;
	struct KD_Tree_NODE* right;
	SPKDArray data;
};

void PrintTree (KDTreeNode root){

}

KDTreeNode NodeInit(int dim, double val, KDTreeNode left, KDTreeNode right, SPKDArray data) {
	KDTreeNode node = (KDTreeNode) malloc(sizeof(KDTreeNode));
	if (node == NULL)
		return NULL;
	node->dim = dim;
	node->val = val;
	node->left = left;
	node->right = right;
	node->data = data;

	return node;
}
KDTreeNode KDTreeInit(SPPoint** p, int n, int d, int splitMethod) {
	SPKDArray array;
	SPKDArray* kdarr = init(p, n);
	if (kdarr == NULL || kdarr < 0) {
		return NULL;
	}
	return KDTreeBuild(array, splitMethod, -1);
}
int maxSpread(SPKDArray kdarray) {
	int i, j = 0;
	double min, max;
	double maxS=0;
	for (i = 0; i < kdarray.d; i++) {
		min = spPointGetAxisCoor((kdarray.arr[kdarray.mat[i][0]]), i);
		max = spPointGetAxisCoor(kdarray.arr[kdarray.mat[i][kdarray.n - 1]],i);
		if ((max - min) > maxS) {
			maxS = max - min;
			j = i;
		}

	}
	return j;

	KDTreeNode KDTreeBuild(SPKDArray kdarr, int splitMethod, int iSplit) {
		int isplit;
		double val;
		int mid;
		int idx;
		int n = kdarr.n;
		int d = kdarr.d;

		KDTreeNode left, right;
		SPKDArray leftArr, rightArr;
		if (n == 1) {
			return NodeInit(-1, -1, NULL, NULL, kdarr);
		}
		if (splitMethod == RANDOM) {
			isplit = rand() % d;
		} else if (splitMethod == INCREMENTAL) {
			isplit = (iSplit + 1) % d;
		}
		else if (splitMethod==MAX_SPREAD){
			isplit =maxSpread(kdarr);

		}
		else{
			return NULL;

		}
		val = spPointGetAxisCoor(kdarr.arr[idx], isplit);
		idx = kdarr.mat[isplit][mid];
		if(n%2==0){
				mid = n/2;
			}
			else{
				mid =(n+1)/2;}

		if (Split(&kdarr, isplit, &leftArr, &rightArr) < 0) {
			return NULL;
		}

		left = KDTreeBuild(leftArr, splitMethod, isplit);
		right = KDTreeBuild(rightArr, splitMethod, isplit);

		if (!left || !right){
			return NULL;}
		return NodeInit(isplit, val, left, right, NULL);
	}


	int searchUtil(KDTreeNode curr, SPBPQueue bpq, SPPoint* p) {

		BPQueueElement out = (BPQueueElement) malloc (sizeof(int)+sizeof(double))  ; //represents output point
		if(!out){
			return -1; //smart malloc needed! :)
		}
		double l2dist,bpqMax;

		char side;

		if (!curr)
			return -1;

		if (isLeaf(curr)) {
			out ->index = spPointGetIndex(curr->data->arr[0]);
			out-> value =	spPointL2SquaredDistance(curr->data->arr[0], *p);
			if (!out) {
				//TODO free the malloc
				return -1;

			}
			//we try to insert it to the bpq
			int insert = spBPQueueEnqueue(bpq, out);
			if ( insert!= SP_BPQUEUE_SUCCESS) {
				//TODO free the malloc
				return -1;
			}
			//TODO free the malloc
		} else {

			if (spPointGetAxisCoor(p, curr->dim) <= curr->val) {
				searchUtil(curr->left, bpq, p);
				side = LEFT;
			} else {
				searchUtil(curr->right, bpq, p);
				side = RIGHT;
			}
			bpqMax = spBPQueueMaxValue(bpq);

			l2dist = abs(curr->val - spPointGetAxisCoor(p, curr->dim));

			if (!spBPQueueIsFull(bpq) || l2dist < bpqMax) {
				if (side == LEFT)
					searchUtil(curr->right, bpq, p);
				else
					searchUtil(curr->left, bpq, p);
			}
		}
		return 0;
	}

	int* SPKDTreeSearch(KDTreeNode root, int SPKNN, SPPoint* p) {

		BPQueueElement curr = (BPQueueElement) malloc(sizeof(BPQueueElement)) ;
		SPBPQueue bpq;
		if (root==NULL || SPKNN < 1)
			return NULL;
		bpq = spBPQueueCreate(SPKNN);
		if (searchUtil(root, bpq, p) < 0) {
			return NULL;
		}
		int* res =(int*) malloc(sizeof(int) * SPKNN);


		for (i = 0; i < SPKNN; i++) {
			curr = spBPQueuePeek(bpq);
			if (curr==NULL||spBPQueueDequeue(bpq) != SP_BPQUEUE_SUCCESS)
				return NULL;
			res[i] = curr->index;
		}

		return res;
	}



}
