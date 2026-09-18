#pragma once
#include <stdexcept> 

template <typename T>
struct Node
{
	T data;
	Node* next;
};

template <typename T>
class MyStack
{
private:
	Node<T>* head;

public:
	MyStack() : head(nullptr) {}

    void push(T val)
    {
        Node<T>* newNode = new Node<T>;
        newNode->data = val;
        newNode->next = head;
        head = newNode;
    }

    T pop()
    {
        if (isEmpty())
        {
            throw std::out_of_range("Стек пуст! Невозможно достать элемент!");
        }
        Node<T>* nodeToRemove = head;
        T val = nodeToRemove->data;
        head = head->next;
        delete nodeToRemove;

        return val;
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

	~MyStack()
    {
        while(!isEmpty())
        {
            pop();
        }
    }
};