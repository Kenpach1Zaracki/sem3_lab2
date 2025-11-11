#include <iostream>
#include "set.h"
#include "hash_simple.h"
using namespace std;

struct Set {
    SimpleHashTable* table;
};

Set* createSet(int initialCapacity) {
    Set* set = new Set;
    set->table = createSimpleHashTable(initialCapacity);
    return set;
}

void destroySet(Set* set) {
    if (!set) return;
    destroySimpleHashTable(set->table);
    delete set;
}

void setAdd(Set* set, const string& element) {
    if (!set || element.empty()) return;
    simpleHashInsert(set->table, element, 1);
}

void setDelete(Set* set, const string& element) {
    if (!set || element.empty()) return;
    simpleHashDelete(set->table, element);
}

int setAt(Set* set, const string& element) {
    if (!set || element.empty()) return -1;
    return simpleHashSearch(set->table, element);
}

void setGetElements(Set* set, string* elements, int& count) {
    if (!set) {
        count = 0;
        return;
    }
    simpleHashGetKeys(set->table, elements, count);
}

int setGetSize(Set* set) {
    if (!set || !set->table) return 0;
    return set->table->size;
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

string* getAllElements(Set* set, int& count) {
    if (!set || !set->table) {
        count = 0;
        return nullptr;
    }
    
    count = 0;
    for (int i = 0; i < set->table->capacity; i++) {
        SimpleHashNode* node = set->table->table[i];
        while (node) {
            count++; 
            node = node->next;
        }
    }
    
    if (count == 0) {
        return nullptr;
    }
    
    string* elements = new string[count];
    int index = 0;
    
    for (int i = 0; i < set->table->capacity; i++) {
        SimpleHashNode* node = set->table->table[i];
        while (node) {
            elements[index++] = node->key; 
            node = node->next;
        }
    }
    
    return elements;
}