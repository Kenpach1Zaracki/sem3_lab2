#ifndef SET_H
#define SET_H

#include <string>

struct Set;

Set* createSet(int capacity);
void destroySet(Set* set);
void setAdd(Set* set, const std::string& key);
void setDelete(Set* set, const std::string& key);
int setAt(Set* set, const std::string& key);
void printSet(Set* set);
std::string* getAllElements(Set* set, int& count);
int setGetSize(Set* set);
void setGetElements(Set* set, std::string* elements, int& count);

#endif