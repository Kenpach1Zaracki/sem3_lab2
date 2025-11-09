#include <iostream>
#include "structures.h"
#include "hash_simple.h"
using namespace std;

// Хеш-функция
int simpleHash(const string& key, int capacity) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % capacity;
}

SimpleHashTable* createSimpleHashTable(int capacity) {
    SimpleHashTable* ht = new SimpleHashTable;
    ht->capacity = capacity;
    ht->size = 0;
    ht->table = new SimpleHashNode*[capacity];
    
    for (int i = 0; i < capacity; i++) {
        ht->table[i] = nullptr;
    }
    
    return ht;
}

void destroySimpleHashTable(SimpleHashTable* ht) {
    if (!ht) return;
    
    for (int i = 0; i < ht->capacity; i++) {
        SimpleHashNode* current = ht->table[i];
        while (current != nullptr) {
            SimpleHashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    delete[] ht->table;
    delete ht;
}

bool simpleHashInsert(SimpleHashTable* ht, const string& key, int value) {
    if (!ht || key.empty()) return false;
    
    int index = simpleHash(key, ht->capacity);
    SimpleHashNode* current = ht->table[index];
    
    // Проверяем, существует ли уже такой ключ
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;  // Обновляем значение
            return true;
        }
        current = current->next;
    }
    
    // Добавляем новый узел в начало цепочки
    SimpleHashNode* newNode = new SimpleHashNode;
    newNode->key = key;
    newNode->value = value;
    newNode->next = ht->table[index];
    ht->table[index] = newNode;
    ht->size++;
    
    return true;
}

bool simpleHashDelete(SimpleHashTable* ht, const string& key) {
    if (!ht || key.empty()) return false;
    
    int index = simpleHash(key, ht->capacity);
    SimpleHashNode* current = ht->table[index];
    SimpleHashNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                // Удаляем первый элемент цепочки
                ht->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            ht->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;  // Ключ не найден
}

int simpleHashSearch(SimpleHashTable* ht, const string& key) {
    if (!ht || key.empty()) return -1;
    
    int index = simpleHash(key, ht->capacity);
    SimpleHashNode* current = ht->table[index];
    
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return -1;  // Ключ не найден
}

void simpleHashGetKeys(SimpleHashTable* ht, string* keys, int& count) {
    if (!ht) {
        count = 0;
        return;
    }
    
    count = 0;
    for (int i = 0; i < ht->capacity; i++) {
        SimpleHashNode* current = ht->table[i];
        while (current != nullptr) {
            keys[count++] = current->key;
            current = current->next;
        }
    }
}
