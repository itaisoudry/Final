/*
 * KD-TREE.h
 *
 *  Created on: Apr 6, 2017
 *      Author: ifat
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPPoint.h"
#include "KDArray.h"
#include "Utils/AllocationHandler.h"
#include "Utils/ResponseCodes.h"

#define RANDOM 0
#define INCREMENTAL 1
#define MAX_SPREAD 2

typedef struct KD_Tree_NODE KDTreeNode;

KDTreeNode* NodeInit(int dim, double val, KDTreeNode* left, KDTreeNode* right, SPKDArray* data) ;
int KDTreeInit(KDTreeNode* tree, SPPoint** p, int n, int d, int splitMethod) ;
KDTreeNode* KDTreeBuild(SPKDArray* kdarr, int splitMethod, int iSplit) ;
int KDTreeSearch(int* res, KDTreeNode* root, int SPKNN, SPPoint* p);

int maxSpread(SPKDArray* kdarray) ;
int treeSize ();




#endif /* KDTREE_H_ */
