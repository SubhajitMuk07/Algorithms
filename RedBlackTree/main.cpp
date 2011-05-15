/*
 * main.cpp
 *
 *  Created on: Apr 19, 2011
 *      Author: kurtisthompson
 */
#include <iostream>
#include "RB_Tree.h"

int main(int argc, char *argv[])
{
	RB_Node* node = new RB_Node();
	RB_Node* node2 = new RB_Node();
	RB_Node* node3 = new RB_Node();
	RB_Node* node4 = new RB_Node();
	RB_Node* node5 = new RB_Node();
	RB_Node* node6 = new RB_Node();
	RB_Node* node7 = new RB_Node();
	RB_Node* node8 = new RB_Node();
	RB_Node* node9 = new RB_Node();

	node->key = 1;
	node2->key = 2;
	node3->key = 11;
	node4->key = 14;
	node5->key = 15;
	node6->key = 7;
	node7->key = 4;
	node8->key = 5;
	node9->key = 8;

	RB_Tree* tree = new RB_Tree();
	tree->RB_Insert(node);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node2);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node3);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node4);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node5);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node6);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node7);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node8);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	tree->RB_Insert(node9);
	tree->ViewTree_PostOrder(tree->RB_GetRoot());

	return 0;
}
