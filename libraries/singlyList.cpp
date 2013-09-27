#include "singlyList.h"

using namespace std;

SinglyList::SinglyList()
{
	head = tail = 0;
}

SinglyList::~SinglyList()
{
	Node *temp;
	while(!isEmpty())
	{
		temp = head->next;
		delete head;
		head = temp;
	}
}

int SinglyList::isEmpty()
{
	if(head == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void SinglyList::pushBack(int r, int c)
{
	if(tail != 0)
	{
		tail->next = new Node(r, c);
		tail = tail->next;
	}
	else
	{
		head = tail = new Node(r, c);
	}
}

void SinglyList::deleteNode(int r, int c)
{
	int info;
	if(head != 0)
	{
		if((head == tail)&&(head->row == r)&&(head->col == c))
		{
			head = tail = 0;
		}
		else if((head->row == r)&&(head->col == c))
		{
			Node *temp = head;
			head = head->next;
			delete temp;
		}
		else
		{
			Node *pred, *temp;
			for(pred = head, temp = head->next;
				(temp != 0) && !((temp->row == r)&&(temp->col == c));
				pred = pred->next, temp = temp->next);
			if(temp != 0)
			{
				pred->next = temp->next;
				if(temp == tail)
				{
					tail = pred;
				}
				delete temp;
			}
		}
	}
}

int SinglyList::isInList(int r, int c) const
{
	Node *temp;
	for(temp = head; (temp != 0) && !((temp->row == r)&&(temp->col == c)); temp = temp->next);	
	return (temp != 0);
}

void SinglyList::printList()
{
	if(head == 0)
	{
//		cout << "Empty List" << endl;
	}
	Node *temp = head;
	if(temp != 0)
	{
	//	cout << temp->row << " " << temp->col << endl;
	}
	if(head != tail)
	{
		for(temp = head->next; temp != tail; temp = temp->next)
		{
		//	cout << temp->row << " " << temp->col << endl;
		}
	//	cout << tail->row << " " << tail->col << endl;
	}
}
