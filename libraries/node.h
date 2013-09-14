#ifndef _NODE_H_
#define _NODE_H_
#include <iostream>

using namespace std;

class Node
{
public:
	int row;
	int col;
	Node *next;
	Node(int r, int c, Node *ptr = 0)
	{
		row = r;
		col = c;
		next = ptr;
	}
};


#endif /*_NODE_H_*/
