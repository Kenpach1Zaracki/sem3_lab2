#ifndef STACK_H
#define STACK_H
#include <string>
#include "structures.h"
using namespace std;

Stack* createStack();

void destroyStack(Stack* stack);

void push(Stack* stack, const string& data);

void pop(Stack* stack);

string top(Stack* stack);

bool isEmpty(Stack* stack);

void printStack(Stack* stack);

#endif
