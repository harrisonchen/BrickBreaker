#ifndef _SINGLYLIST_H_
#define _SINGLYLIST_H_
#include "node.h"
#include <iostream>

using namespace std;

class SinglyList
{
public:
	SinglyList();
	~SinglyList();
	int isEmpty();
	void pushFront(int);
	void pushBack(int);
 	int popFront();
	int popBack();
	void deleteNode(int);
	int isInList(int) const;
	void printList();
private:
	Node *head, *tail;
};

#endif /*_SINGLYLIST_H_*/
