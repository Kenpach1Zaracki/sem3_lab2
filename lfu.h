#ifndef LFU_H
#define LFU_H

struct LFUNode {
    int key;
    int value;
    int frequency;    
    int timestamp;     
    bool isEmpty;      
};

// Простая хеш-таблица для маппинга key → index
struct LFUHashNode {
    int key;
    int value;  // индекс в массиве nodes
    bool isEmpty;
};

struct LFUHashTable {
    LFUHashNode* table;
    int capacity;
};

struct LFUCache {
    LFUNode* nodes;    
    int capacity;      
    int size;          
    int currentTime;   
    LFUHashTable* hashTable;  // Своя хеш-таблица для O(1) поиска
};

LFUCache* createLFUCache(int capacity);

void destroyLFUCache(LFUCache* cache);

void lfuSet(LFUCache* cache, int key, int value);

int lfuGet(LFUCache* cache, int key);

int lfuFindIndex(LFUCache* cache, int key);

int lfuFindMinFrequency(LFUCache* cache);

void printLFUCache(LFUCache* cache);

#endif