/*
 * RB_Node.cpp
 *
 *  Created on: Apr 18, 2011
 *      Author: kurtisthompson
 */

#include <iostream.h>
#include "RB_Node.h"

RB_Node::RB_Node()
{
	right = NULL;
	left = NULL;
	p = NULL;
	color = RED;
	key = NULL;
}

RB_Node::~RB_Node()
{
	delete right;
	delete left;
	delete p;
}

