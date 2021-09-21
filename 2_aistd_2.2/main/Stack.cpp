#include "Stack.h"
using namespace std;

template <class T>
size_t Stack<T>::getSize()
{
	return stack_size;
}

template <class T>
bool Stack<T>::isEmpty()
{
	if (head == NULL)
		return true;
	return false;
}

template <class T>
void Stack<T>::clear()
{
	if (head != NULL)
	{
		while (head->next != NULL)  //delete not last element
		{
			head = head->next; //next element is now head element
			delete head->previous; //delete previous head
		}
		stack_size = 0;  //all elements are deleted
		head = NULL;
		tail = NULL;
	}
}

template <class T>
void Stack<T>::push(T add)
{
	if (stack_size == 0) //if stack is empty
	{
		head = new Node(add); //create head
		tail = head;
	}
	else
	{
		Node* current = new Node(add);
		current->previous = tail;
		tail->next = current;
		tail = current;
	}

	stack_size++;
}

template <class T>
T Stack<T>::pop()
{
	T inf;
	if (stack_size == 1)
	{
		stack_size = 0;  //if stack contains one element
		inf = tail->node_info;
		head = NULL;
		tail = NULL;

	}
	else if (stack_size > 1) //if stack contains more than one element
	{
		inf = tail->node_info;
		tail = tail->previous;
		delete tail->next;
		tail->next = NULL;
		stack_size--;
	}
	else  //else return error
		throw out_of_range("Element not found");
	return inf;
}

template <class T>
T Stack<T>::get_last() {
	return tail->node_info;
}
