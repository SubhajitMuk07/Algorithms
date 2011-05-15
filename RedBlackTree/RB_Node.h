/*
 * RB-Node.h
 *
 *  Created on: Apr 18, 2011
 *      Author: kurtisthompson
 */

#ifndef RB_NODE_H_
#define RB_NODE_H_

using namespace std;

//Nodes are either RED or BLACK
enum COLOR{ RED, BLACK};

RB_Node* sentinal;

class RB_Node{

public:
	//Constructor
	RB_Node();
	//Destructor
	~RB_Node();
	//Right Child
	RB_Node* right;
	//Left Child
	RB_Node* left;
	//Parent Pointer
	RB_Node* p;
	//Simple integer key to correspond to the text
	int key;
	//Node color value
	COLOR color;


};




#endif /* RB_NODE_H_ */
