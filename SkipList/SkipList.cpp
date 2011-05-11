/*
 * SkipList.cpp
 *
 *  Created on: Apr 24, 2011
 *      Author: Kurtis Thompson
 */

#include <iostream>
#include <stdlib.h>

using namespace std;

class SkipListNode{

	private:
		int level;
		int value;


	public:
		SkipListNode()
		{
				setLevel(32);
				forward = new SkipListNode* [level+1];
			        for (int i=0; i<=level; i++)
			            forward[i] = NULL;
		}

		SkipListNode(int v, int l)
		{
			setValue(v);
			setLevel(l);
			forward = new SkipListNode* [level+1];
			for (int i=0; i<=level; i++)
				forward[i] = NULL;
		}
		~SkipListNode()
		{
			delete[] forward;
		}
		void setValue(int v){value = v;}
		int getLevel(){return level;}
		int setLevel(int l){level = l;}
		int getValue(){return value;}
		SkipListNode** forward;


};

class SkipList{

	private:
		SkipListNode* head;
		SkipListNode* tail;
		SkipListNode* front;
		SkipListNode* rear;
		int level;
		int randomLevel() {
			 int newLevel = 0;
			 for(newLevel=0; rand() % 2 == 0; newLevel++)
				 ;
			 if(newLevel > head->getLevel() + 1)
				 newLevel = head->getLevel() + 1;

			 if(newLevel > 32)
				 return 32;

			 return newLevel;

		}

	public:
		SkipList() {
		          head = new SkipListNode(-1, 0);
		      }



			bool search(int key)
			{
				 if(head->forward[0] == NULL)
					 return false;
				else
				{
					SkipListNode* temp = head;
					for(int i = head->getLevel(); i >=0; i--)
					{
						while((temp->forward[i] != NULL) && (temp->forward[i]->getValue() < key))
						{
							temp = temp->forward[i];
						}

					}
					temp = temp->forward[0];
					if((temp != NULL) && (temp->getValue() == key))
					{
						return true;
					}
					return false;
				}
			}

			 void remove(int key)
			 {
				 if(head->forward[0] == NULL)
					 return;
				 else
				 {
					 SkipListNode* temp = head;
					 SkipListNode** update = new SkipListNode*[32];
					 for(int i = head->getLevel(); i >=0; i--)
					 {
						 while((temp->forward[i] != NULL) && (temp->forward[i]->getValue() < key))
						 {
							 temp = temp->forward[i];
						 }
							 update[i] = temp;
					 }
						 temp = temp->forward[0];
						 if((temp != NULL) && (temp->getValue() == key))
						 {
							 for(int i = 0; i <= head->getLevel(); i++)
							 {
								 if(update[i]->forward[i] == temp)
									 update[i]->forward[i] = temp->forward[i];
							 }
							 delete temp;

							 while((head->getLevel() > 0) && head->forward[head->getLevel()] == NULL)
							 {
								 head->setLevel(head->getLevel()-1);
						 }
					 }
				 }
			 }

		      void insert(int key)
		      {
		    	  SkipListNode* Update[32];
		    	  SkipListNode* x = head;
		    	  for(int i = head->getLevel(); i >= 0; i--)
		    	  {
		    		  while(x->forward[i] != NULL && x->forward[i]->getValue() < key)
		    		  {
		    			  x = x->forward[i];
		    		  }
		    		  Update[i] = x;
		    	  }

		    	  x = x->forward[0];
		    	  if(x != NULL && x->getValue() == key)
		    	  {
		    		  //Do nothing on duplicate
		    		  return;
		    	  }
		    	  else
		    	  {
		    		  int l = randomLevel();

		    		  if(l > head->getLevel())
		    		  {
		    			  for(int i = (head->getLevel())+1; i<= l; i++)
		    			  {
		    				  Update[i] = head;
		    				  Update[i]->setLevel(i);
		    			  }
		    		  }

		    	  x = new SkipListNode(key, l);
		    	  for(int i = 0; i <= l; i++)
		    	  {
		    		  	  x->forward[i] = Update[i]->forward[i];
		    		  	  Update[i]->forward[i] = x;
		    	  }
		    	  }

		      }

};


