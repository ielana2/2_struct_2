#pragma once
#include <iostream>

template <class T>
class Stack{ //class for stack
private:
    class Node{ //class for each element
    public:
        T node_info;
        Node* next;
        Node* previous;

        Node(T node_info = 0, Node* previous = NULL, Node* next = NULL){
            this->node_info = node_info; //element's content
            this->previous = previous; //link to previous element
            this->next = next; //link to next element
        };

        ~Node() { }
    };

    Node* head; //head of the stack
    Node* tail;  //tail of the stack
    size_t stack_size;  //size of the stack

public:
    Stack(Node* head = NULL, Node* tail = NULL) {
        this->head = head;
        this->tail = tail;
        stack_size = 0;
    };

    size_t getSize(); //get size of the stack
    bool isEmpty(); //test for emptiness
    void clear(); //clear stack
    void push(T); //add element to the end of the stack
    T pop(); //delete last element
    T get_last();

    ~Stack(){
        if (head != NULL){
            while (head->next != NULL){ //while not last element
                head = head->next;
                delete head->previous;
            }

            stack_size = 0;
            delete head;
        }
    };
};