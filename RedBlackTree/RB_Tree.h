/*
 * RB_Tree.h
 *
 *  Created on: Apr 19, 2011
 *      Author: kurtisthompson
 */

#ifndef RB_TREE_H_
#define RB_TREE_H_

#include "RB_Node.h"
#include <exception>

class RB_Tree
{

public:

	RB_Tree(){
			root = NULL;
	}

	~RB_Tree(){
			delete root;
	}

	RB_Node* RB_GetRoot()
	{
		return root;
	}

	void RB_Insert(RB_Node* z)
	{
		//Create an empty Node
		RB_Node* y = NULL;
		//Get our root
		RB_Node* x = root;

		//While our root isn't null
		while(x != NULL)
		{
			//Set x to the root
			y = x;
			//Go left/Right maintaining BST conditions.
			if(z->key < x->key)
			{
				x = x->left;
			}
			else
				x = x->right;
		}
		//At this point, we have found a leaf where our node should go
		z->p = y;
		if(y == NULL)
		{
			root = z;
		}
		else if(z->key < y->key)
		{
			y->left = z;
		}
		else
		{
			y->right = z;
		}
		z->left = NULL;
		z->right = NULL;
		z->color = RED;
		RB_Insert_Fixup(z);
	}

	void Left_Rotate(RB_Node* x)
	{
		RB_Node* y = x->right;
		x->right = y->left;
		if(y->left != NULL)
		{
			y->left->p = x;
		}
		y->p = x->p;
		if(x->p == NULL)
			root = y;
		else if(x == x->p->left)
		{
			x->p->left = y;
		}
		else
			x->p->right = y;
		y->left = x;
		x->p = y;
	}

	void Right_Rotate(RB_Node* x)
		{
			RB_Node* y = x->left;
			x->right = y->right;
			if(y->right != NULL)
			{
				y->right->p = x;
			}
			y->p = x->p;
			if(x->p == NULL)
				root = y;
			else if(x == x->p->right)
			{
				x->p->right = y;
			}
			else
				x->p->left = y;
			y->right = x;
			x->p = y;
		}

	void ViewTree_PostOrder(RB_Node* current)
	{
		if(current == NULL)
			return;
		else
		{
			ViewTree_PostOrder(current->left);
			ViewTree_PostOrder(current->right);
			try{
			cout << current->key << endl;
			}catch(exception ex)
			{
				cout << "Current Key is Null" << endl;
			}
		}
	}

	void RB_Insert_Fixup(RB_Node* z)
	{
		RB_Node* y = NULL;
		while(z->p != NULL && (z->p->p != NULL) && z->p->color == RED)
		{
			if(z->p == z->p->p->left)
			{
				y = z->p->p->right;

				if(y!= NULL && y->color == RED)
				{
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					z = z->p->p;
				}
				else if(z == z->p->right)
				{
					z = z->p;
					Left_Rotate(z);
				}
				if(z->p != NULL && z->p->p != NULL)
				{
				z->p->color = BLACK;
				z->p->p->color = RED;
				Right_Rotate(z->p->p);
				}
			}
			else
			{
				y = z->p->p->left;
				if(y!=NULL &&  y->color == RED)
				{
					z->p->color = BLACK;
					y->color = BLACK;
					z->p->p->color = RED;
					z = z->p->p;
				}
				else if(z == z->p->left)
				{
					z = z->p;
					cout << "Calling Right Rotate" << endl;
					Right_Rotate(z);
				}
				if(z->p != NULL && z->p->p != NULL)
				{
					z->p->color = BLACK;
					z->p->p->color = RED;
					Left_Rotate(z->p->p);
				}
			}
		}

	}

private:
	RB_Node* root;
};

#endif /* RB_TREE_H_ */
