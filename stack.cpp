#include <iostream>
#include "stack.h"
using namespace std;

Stack* createStack() {
    Stack* stack = new Stack;
    stack->top = nullptr;
    return stack;
}

void destroyStack(Stack* stack) {
    if (!stack) return;
    
    while (stack->top != nullptr) {
        Node* temp = stack->top;
        stack->top = stack->top->next;
        delete temp;
    }
    delete stack;
}

void push(Stack* stack, const string& data) {
    if (!stack) return;
    
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

void pop(Stack* stack) {
    if (!stack || !stack->top) return;
    
    Node* temp = stack->top;
    stack->top = stack->top->next;
    delete temp;
}

string top(Stack* stack) {
    if (!stack || !stack->top) return "";
    return stack->top->data;
}

bool isEmpty(Stack* stack) {
    return (!stack || stack->top == nullptr);
}

void printStack(Stack* stack) {
    if (!stack) return;
    
    Node* current = stack->top;
    cout << "Стек: ";
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}
