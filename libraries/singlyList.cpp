#include "node.h"
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

void SinglyList::pushFront(int d)
{
	Node *temp = new Node(d, head);
	head = temp;
	if(tail == 0)
	{
		tail = head;
	}
}

void SinglyList::pushBack(int d)
{
	if(tail != 0)
	{
		tail->next = new Node(d);
		tail = tail->next;
	}
	else
	{
		head = tail = new Node(d);
	}
}

int SinglyList::popFront()
{
	int info;
	if(head != 0)
	{
		if(head == tail)
		{
			info = head->data;
			delete head;
			head = tail = 0;
			return info;
		}
		else
		{
			info = head->data;
			Node *temp = head;
			head = head->next;
			delete temp;
			return info;
		}
	}
	//return "empty";
}

int SinglyList::popBack()
{
	int info;
	if(head != 0)
	{
		if(head == tail)
		{
			info = head->data;
			delete head;
			head = tail = 0;
			return info;
		}
		else
		{
			Node *temp;
			for(temp = head; temp->next != tail; temp = temp->next);
			info = tail->data;
			delete tail;
			tail = temp;
			tail->next = 0;
			return info;
		}
	}
	//return "empty";
}

void SinglyList::deleteNode(int d)
{
	int info;
	if(head != 0)
	{
		if((head == tail)&&(head->data == d))
		{
			info = head->data;
			//delete head;
			head = tail = 0;
			//return info;
		}
		else if(head->data == d)
		{
			info = head->data;
			Node *temp = head;
			head = head->next;
			delete temp;
			//return info;
		}
		else
		{
			Node *pred, *temp;
			for(pred = head, temp = head->next;
				(temp != 0) && !(temp->data == d);
				pred = pred->next, temp = temp->next);
			if(temp != 0)
			{
				pred->next = temp->next;
				if(temp == tail)
				{
					tail = pred;
				}
				info = temp->data;
				delete temp;
				//return info;
			}
		}
		//return "none";
	}
	//return "empty";
}

int SinglyList::isInList(int d) const
{
	Node *temp;
	for(temp = head; (temp != 0)&&(temp->data != d);temp = temp->next);
	return (temp != 0);
}

void SinglyList::printList()
{
	if(head == 0)
	{
		//cout << "Empty List" << endl;
	}
	Node *temp = head;
	if(temp != 0)
	{
		cout << temp->data << endl;
	}
	if(head != tail)
	{
		for(temp = head->next; temp != tail; temp = temp->next)
		{
			cout << temp->data << endl;
		}
		cout << tail->data << endl;
	}
}
