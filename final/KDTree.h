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
#define MAX_SPREAD 1
#define INCREMENTAL 2

typedef struct KD_Tree_NODE KDTreeNode;

//Initialize KD-Tree
/**
 * @param tree - KD-Tree (result)
 * @param p - point
 * @param n - size
 * @param splitMethod
 *
 * @return result value with success / error code
 */
int KDTreeInit(KDTreeNode* tree, SPPoint** p, int n, int splitMethod);

//Initialize node
/**
 * @param dim - dimenstion
 * @param val - tree value
 * @param left - left node
 * @param right - right node
 * @param data - KD-Array data
 * @return result value with success / error code
 */
KDTreeNode* NodeInit(int dim, double val, KDTreeNode* left, KDTreeNode* right, SPKDArray* data);

/**
 * @param kdarr - KD-Array
 * @param splitMethod
 * @param i_Splie point of split
 * @return result value with success /error code
 */
KDTreeNode* KDTreeBuild(SPKDArray* kdarr, int splitMethod, int i_Split);

//Tree search
/**
 * @param res - result
 * @param root - tree root node
 * @param SPKNN - SPKNN from config
 * @param p - point
 * @return result vaule with success / error code
 */
int KDTreeSearch(int* res, KDTreeNode* root, int SPKNN, SPPoint* p);

/**
 * Returns max spread
 * @param kdarray - KD-Array
 * @return max spread
 */
int maxSpread(SPKDArray* kdarray);

//returns tree size;
int treeSize();

//check if node is a leaf
/**
 * @param node - KD-Tree node to check if leaf
 */
int isLeaf(KDTreeNode* node);

#endif /* KDTREE_H_ */
