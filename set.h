#ifndef SET_H
#define SET_H
#include <string>
using namespace std;

struct Set;

Set* createSet(int initialCapacity = 1000);

void destroySet(Set* set);

bool setAdd(Set* set, const string& element);

bool setDelete(Set* set, const string& element);

int setAt(Set* set, const string& element);

void setGetElements(Set* set, string* elements, int& count);

int setGetSize(Set* set);

void printSet(Set* set);

#endif
