#include "PriorityQueue.h"
#include <iostream>
using namespace std;

template <class T>
void PriorityQueue<T>::push(T add_data, int priority) // add item to the end of the queue
{
	if (queue_size == 0) //if queue is empty
	{
		head = new Node(add_data, priority); //create head
		tail = head;
	}
	else
	{
		Node* newNode = new Node(add_data, priority);
		Node* current = head;
		while ((newNode->priority < current->priority) && (current->next != NULL)) //searching where to place element
			current = current->next;

		if ((newNode->priority <= current->priority) && (current->next == NULL)) {
			newNode->previous = current;
			newNode->next = NULL;
			current->next = newNode;
			tail = newNode;
		}
		else if ((newNode->priority >= current->priority) && (current->previous == NULL))
		{
			newNode->previous = NULL;
			newNode->next = current;
			current->previous = newNode;
			head = newNode;
		}
		else {
			newNode->next = current;
			newNode->previous = current->previous;
			current->previous->next = newNode;
			current->previous = newNode;

		}
	}

	queue_size++;
}

template <class T>
size_t PriorityQueue<T>::get_size() // get queue size
{
	return queue_size;
}

template <class T>
T PriorityQueue<T>::pop() // delete first item
{
	T data;
	if (queue_size == 1)
	{
		data = head->data;
		queue_size = 0;  //setup zero position
		head = NULL;
		tail = NULL;
	}
	else if (queue_size > 1) //if queue has more than one element
	{
		data = tail->data;
		tail = tail->previous; //set new head
		delete tail->next;
		tail->next = NULL;
		queue_size--;
	}
	else
		throw out_of_range("Segmentation fault");
	return data;
}

template <class T>
int PriorityQueue<T>::tail_priority() { //get tail priority
	return tail->priority;
}

template <class T>
void PriorityQueue<T>::clear() // clear queue
{
	if (head != NULL)
	{
		while (head->next != NULL)  //delete all elements
		{
			head = head->next;
			delete head->previous;
		}
		queue_size = 0;
		head = NULL;
		tail = NULL;
	}
}

template <class T>
bool PriorityQueue<T>::isEmpty() // test for emptiness
{
	if (head == NULL)
		return true;
	return false;
}