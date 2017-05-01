/*
 * sp_kd_tree_unit_test.c
 *
 *  Created on: Apr 30, 2017
 *      Author: soudry
 */

#include "unit_test_util.h"
#include "../KDArray.h"
#include "../KDTree.h"

bool kdTreeTest() {
	KDTreeNode* root;

	double p1Data[3] = { 1, 2, 3 };
	double p2Data[3] = { 3, 2, 1 };
	double p3Data[3] = { 1.5, 2.5, 3.5 };
	double p4Data[3] = { 3.5, 2.5, 1.5 };

	SPPoint* p1 = spPointCreate(p1Data, 3, 0);
	SPPoint* p2 = spPointCreate(p2Data, 3, 1);
	SPPoint* p3 = spPointCreate(p3Data, 3, 2);
	SPPoint* p4 = spPointCreate(p4Data, 3, 3);

	SPPoint* pointsArr[4];
	pointsArr[0] = p1;
	pointsArr[1] = p2;
	pointsArr[2] = p3;
	pointsArr[3] = p4;

	SPKDArray* kdArray = (SPKDArray*) malloc(sizeof(SPKDArray));

	init(kdArray, pointsArr, 4);

	//validate init
	ASSERT_FALSE(kdArray==NULL);

	root = (KDTreeNode*) malloc(sizeof(root));
	KDTreeInit(root, pointsArr, 4, MAX_SPREAD);

	ASSERT_FALSE(root==NULL);

	KDTreeBuild(kdArray, MAX_SPREAD, 1);

	ASSERT_FALSE(root==NULL);

	KDTreeBuild(kdArray, INCREMENTAL, 2);

	ASSERT_FALSE(root==NULL);

	int res = 0;
	KDTreeSearch(&res, root, 1, p2);

	ASSERT_TRUE(res == 2);

	KDTreeSearch(&res, root, 1, p3);

	ASSERT_TRUE(res == 0);

	spPointDestroy(p1);
	spPointDestroy(p2);
	spPointDestroy(p3);
	spPointDestroy(p4);

	return true;
}

int main12() {
	RUN_TEST(kdTreeTest);
	return 0;
}
