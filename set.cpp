#include <iostream>
#include "set.h"
#include "hash_simple.h"
using namespace std;

struct Set {
    SimpleHashTable* hashTable;
};

Set* createSet(int initialCapacity) {
    Set* set = new Set;
    set->hashTable = createSimpleHashTable(initialCapacity);
    return set;
}

void destroySet(Set* set) {
    if (!set) return;
    destroySimpleHashTable(set->hashTable);
    delete set;
}

bool setAdd(Set* set, const string& element) {
    if (!set || element.empty()) return false;
    return simpleHashInsert(set->hashTable, element, 1);
}

bool setDelete(Set* set, const string& element) {
    if (!set || element.empty()) return false;
    return simpleHashDelete(set->hashTable, element);
}

int setAt(Set* set, const string& element) {
    if (!set || element.empty()) return -1;
    return simpleHashSearch(set->hashTable, element);
}

void setGetElements(Set* set, string* elements, int& count) {
    if (!set) {
        count = 0;
        return;
    }
    simpleHashGetKeys(set->hashTable, elements, count);
}

int setGetSize(Set* set) {
    if (!set || !set->hashTable) return 0;
    return set->hashTable->size;
}

void printSet(Set* set) {
    if (!set) {
        cout << "Множество не существует" << endl;
        return;
    }
    
    int maxSize = setGetSize(set);
    if (maxSize == 0) {
        cout << "{ }" << endl;
        return;
    }
    
    string* elements = new string[maxSize + 1];
    int count = 0;
    setGetElements(set, elements, count);
    
    cout << "{ ";
    for (int i = 0; i < count; i++) {
        if (i > 0) cout << ", ";
        cout << elements[i];
    }
    cout << " }" << endl;
    
    delete[] elements;
}
