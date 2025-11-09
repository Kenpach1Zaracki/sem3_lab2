#ifndef HASH_SIMPLE_H
#define HASH_SIMPLE_H
#include <string>
#include "structures.h"
using namespace std;

SimpleHashTable* createSimpleHashTable(int capacity = 1000);

void destroySimpleHashTable(SimpleHashTable* ht);

bool simpleHashInsert(SimpleHashTable* ht, const string& key, int value = 1);

bool simpleHashDelete(SimpleHashTable* ht, const string& key);

int simpleHashSearch(SimpleHashTable* ht, const string& key);

void simpleHashGetKeys(SimpleHashTable* ht, string* keys, int& count);

#endif
