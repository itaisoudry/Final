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
#define RANDOM 0
#define INCREMENTAL 1
#define MAX_SPREAD 2

typedef struct KD_Tree_NODE* KDTreeNode;

KDTreeNode NodeInit(int dim, double val, KDTreeNode left,KDTreeNode right, SPKDArray data);
KDTreeNode KDTreeInit(SPPoint** p, int n, int d, int splitMethod);
KDTreeNode KDTreeBuild(SPKDArray kdarr, int splitMethod, int last_isplit);



#endif /* KDTREE_H_ */
