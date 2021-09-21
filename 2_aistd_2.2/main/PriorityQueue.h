#pragma once
#include <iostream>
template <class T>
class PriorityQueue
{
private:
    class Node
    {
    public:
        Node(T data, int priority = 0, Node* previous = NULL, Node* next = NULL)
        {
            this->data = data;
            this->priority = priority;
            this->previous = previous;
            this->next = next;
        };

        ~Node()
        {  }

        T data;
        int priority;
        Node* next;
        Node* previous;
    };

    Node* head; //head list
    Node* tail;  //tail list
    size_t queue_size;  //list size

public:
    PriorityQueue(Node* head = NULL, Node* tail = NULL) {
        this->head = head;
        this->tail = tail;
        queue_size = 0;
    };

    void push(T, int); // add item to the end of the queue
    size_t get_size(); // get queue size
    T pop(); // delete first item of the queue
    int tail_priority(); //get tail priority
    void clear(); // clear queue
    bool isEmpty(); // test for emptiness

    ~PriorityQueue()
    {
        while ((head != NULL) && (head->next != NULL)) //while there are elements in the queue
        {
            head = head->next;
            delete head->previous;
        }
        queue_size = 0;
        delete head;
    };
};