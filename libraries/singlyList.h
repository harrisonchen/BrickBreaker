#ifndef _SINGLYLIST_H_
#define _SINGLYLIST_H_
#include "node.h"

using namespace std;

class SinglyList
{
public:
	SinglyList();
	~SinglyList();
	int isEmpty();
	void pushBack(int, int);
	void deleteNode(int, int);
	int isInList(int, int) const;
	void printList();
private:
	Node *head, *tail;
};

#endif /*_SINGLYLIST_H_*/
