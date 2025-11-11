#ifndef LFU_H
#define LFU_H

struct LFUNode {
    int key;
    int value;
    int frequency;    
    int timestamp;     
    bool isEmpty;      
};

struct LFUHashNode {
    int key;
    int value;  
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
    LFUHashTable* hashTable;  
};

LFUCache* createLFUCache(int capacity);

void destroyLFUCache(LFUCache* cache);

void lfuSet(LFUCache* cache, int key, int value);

int lfuGet(LFUCache* cache, int key);

int lfuFindIndex(LFUCache* cache, int key);

int lfuFindMinFrequency(LFUCache* cache);

void printLFUCache(LFUCache* cache);

#endif