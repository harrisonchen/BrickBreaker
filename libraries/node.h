#ifndef _NODE_H_
#define _NODE_H_
#include <iostream>

using namespace std;

class Node
{
public:
	int data;
	Node *next;
	Node(int d, Node *ptr = 0)
	{
		data = d;
		next = ptr;
	}
};
	

#endif /*_NODE_H_*/
