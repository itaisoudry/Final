/*
 * KD-TREE.c
 *
 *  Created on: Apr 6, 2017
 *      Author: ifat
 */
#include "KDTree.h"

#include "SPBPriorityQueue.h"
#define LEFT 0
#define RIGHT 1

struct KD_Tree_NODE {
	int dim;
	double val;
	struct KD_Tree_NODE* left;
	struct KD_Tree_NODE* right;
	SPKDArray* data;
};
int treeSize (){
	return sizeof(KDTreeNode);
}
int isLeaf(KDTreeNode* node){
	if (node->dim == -1){
		return 1;
	}
	return 0;
}
KDTreeNode* NodeInit(int dim, double val, KDTreeNode* left, KDTreeNode* right, SPKDArray* data) {
	KDTreeNode* node = (KDTreeNode*) malloc(sizeof(KDTreeNode));
	if (node == NULL)
		//TODO error
		return NULL;
	node->dim = dim;
	node->val = val;
	node->left = left;
	node->right = right;
	node->data = data;

	return node;
}
 int KDTreeInit(KDTreeNode* tree, SPPoint** p, int n, int splitMethod) {
	int resultValue = SUCCESS;

	SPKDArray* kdarr =NULL;
	SMART_MALLOC(SPKDArray*, kdarr, sizeof(SPKDArray)*n);
	SMART_FUNCTION_CALL(init(kdarr,p, n));
	KDTreeNode* root = KDTreeBuild(kdarr, splitMethod, (-1));
	tree->data =root->data;
	tree->dim =root->dim;
	tree->left =root->left;
	tree->right =root->right;
	tree->val =root->val;

	return resultValue;
	error:
				SMART_FREE(kdarr);
				return resultValue;

}
int maxSpread(SPKDArray* kdarray) {
	int  j = 0;
	double min, max;
	double maxS = 0;

	for (int i = 0; i < kdarray->d; i++) {
		min = spPointGetAxisCoor((kdarray->arr[kdarray->mat[i][0]]), i);
		max = spPointGetAxisCoor(kdarray->arr[kdarray->mat[i][kdarray->n - 1]], i);
		//TODO if min||max <0 then error
		if ((max - min) > maxS) {
			maxS = max - min;
			j = i;
		}

	}
	return j;
}
	KDTreeNode* KDTreeBuild(SPKDArray* kdarr, int splitMethod, int i_Split) {
		int isplit;
		double val=0;
		int mid=0;
		int idx=0;
		int n = kdarr->n;
		int d = kdarr->d;

		KDTreeNode* left= (KDTreeNode*) malloc (sizeof(KDTreeNode));
		if (!left){
			return NULL;
		}
		KDTreeNode* right= (KDTreeNode*) malloc (sizeof(KDTreeNode));
		if(!right){
			free(left);
			return NULL;
		}
		SPKDArray* leftArr = (SPKDArray*) malloc (sizeof(SPKDArray));
		if (!leftArr){
			free(left);
			free(right);
			//TODO logger
			return NULL;
		}
		SPKDArray* rightArr= (SPKDArray*) malloc (sizeof(SPKDArray));
		if (!rightArr){
			free(leftArr);
			free(left);
			free(right);
			//TODO logger
			return NULL;
		}
		if (n == 1) {
			free(leftArr);
			free(rightArr);
			free(left);
			free(right);
			return NodeInit(-1, -1, NULL, NULL, kdarr);
		}
		if (splitMethod == RANDOM) {
			isplit = rand() % d;
		} else if (splitMethod == INCREMENTAL) {
			isplit = (i_Split + 1) % d;
		} else if (splitMethod == MAX_SPREAD) {
			isplit = maxSpread(kdarr);

		} else {
			free(leftArr);
			free(rightArr);
			free(left);
			free(right);
			return NULL;

		}

		if (n % 2 == 0) {
			mid = n / 2;
		} else {
			mid = (n + 1) / 2;
		}
		idx = kdarr->mat[isplit][mid];

		val = spPointGetAxisCoor(kdarr->arr[idx], isplit);

		if (Split(kdarr, isplit, leftArr, rightArr) < 0) {
			free(leftArr);
			free(rightArr);
			return NULL;
		}
		left = KDTreeBuild(leftArr, splitMethod, isplit);
		right = KDTreeBuild(rightArr, splitMethod, isplit);


		if (!left || !right) {
			free(leftArr);
			free(rightArr);
			free(left);
			free(right);
			return NULL;
		}

		return NodeInit(isplit, val, left, right, NULL);

		//TODO logger errors
	}

	int searchUtil(KDTreeNode* curr, SPBPQueue* bpq, SPPoint* p) {
		int resultValue = SUCCESS;
		double l2dist, bpqMax;
		char side;
		int index;
		double value;
		if (curr==NULL)
			return -1;


		if (isLeaf(curr)) {
			index =spPointGetIndex(curr->data->arr[0]);
			value = spPointL2SquaredDistance(curr->data->arr[0], p);

			//we try to insert it to the bpq
			int insert = spBPQueueEnqueue(bpq, index,value);
			if (insert != SP_BPQUEUE_SUCCESS) {
				//TODO free the malloc
				return -1;
			}
			//TODO free the malloc
		} else {
			int axis =spPointGetAxisCoor(p, curr->dim);
			if ( axis <= curr->val) {
				SMART_FUNCTION_CALL(searchUtil(curr->left, bpq, p));
				side = LEFT;
			} else {
				SMART_FUNCTION_CALL(searchUtil(curr->right, bpq, p));
				side = RIGHT;
			}
			bpqMax = SMART_FUNCTION_CALL(spBPQueueMaxValue(bpq));
			int dista =curr->val - spPointGetAxisCoor(p, curr->dim);
			l2dist = abs(dista);

			if (!spBPQueueIsFull(bpq) || l2dist < bpqMax) {
				if (side == LEFT)
					searchUtil(curr->right, bpq, p);
				else
					searchUtil(curr->left, bpq, p);
			}
		}
		return resultValue;

		error:
		return resultValue;

	}

	int KDTreeSearch(int* res, KDTreeNode* root, int SPKNN, SPPoint* p) {
		int resultValue = SUCCESS;
		BPQueueElement* curr = NULL;

		SMART_MALLOC(BPQueueElement*, curr, sizeof(BPQueueElement));
//		BPQueueElement curr = (BPQueueElement) malloc(sizeof(BPQueueElement));

		if (!root || SPKNN < 1){
			resultValue= -1;
			return resultValue;
		}
		SPBPQueue* bpq = spBPQueueCreate(SPKNN);
		if (searchUtil(root, bpq, p) < 0) {
			resultValue= -1;
			return resultValue;
		}


		for (int i = 0; i < SPKNN; i++) {
			spBPQueuePeek(bpq,curr);

			res[i] = curr->index;
		}

		SMART_FREE(curr);
		SMART_FREE(bpq);
		return resultValue;
		error:
			SMART_FREE(curr);
			SMART_FREE(bpq);
			return resultValue;

	}


