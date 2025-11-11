#include <iostream>
#include <climits>
#include "lfu.h"
using namespace std;


int lfuHash(int key, int capacity) {
    return (key % capacity + capacity) % capacity; 
}

LFUHashTable* createLFUHashTable(int capacity) {
    LFUHashTable* ht = new LFUHashTable;
    ht->capacity = capacity * 2; 
    ht->table = new LFUHashNode[ht->capacity];
    
    for (int i = 0; i < ht->capacity; i++) {
        ht->table[i].isEmpty = true;
    }
    
    return ht;
}

void destroyLFUHashTable(LFUHashTable* ht) {
    if (!ht) return;
    delete[] ht->table;
    delete ht;
}

void lfuHashInsert(LFUHashTable* ht, int key, int index) {
    if (!ht) return;
    
    int hash = lfuHash(key, ht->capacity);
    
    for (int i = 0; i < ht->capacity; i++) {
        int pos = (hash + i) % ht->capacity;
        
        if (ht->table[pos].isEmpty || ht->table[pos].key == key) {
            ht->table[pos].key = key;
            ht->table[pos].value = index;
            ht->table[pos].isEmpty = false;
            return;
        }
    }
}

int lfuHashFind(LFUHashTable* ht, int key) {
    if (!ht) return -1;
    
    int hash = lfuHash(key, ht->capacity);
    
    for (int i = 0; i < ht->capacity; i++) {
        int pos = (hash + i) % ht->capacity;
        
        if (ht->table[pos].isEmpty) {
            return -1; 
        }
        
        if (ht->table[pos].key == key) {
            return ht->table[pos].value; 
        }
    }
    
    return -1;
}

void lfuHashRemove(LFUHashTable* ht, int key) {
    if (!ht) return;
    
    int hash = lfuHash(key, ht->capacity);
    
    for (int i = 0; i < ht->capacity; i++) {
        int pos = (hash + i) % ht->capacity;
        
        if (ht->table[pos].isEmpty) {
            return;
        }
        
        if (ht->table[pos].key == key) {
            ht->table[pos].isEmpty = true;
            return;
        }
    }
}


LFUCache* createLFUCache(int capacity) {
    LFUCache* cache = new LFUCache;
    cache->capacity = capacity;
    cache->size = 0;
    cache->currentTime = 0;
    
    if (capacity > 0) {
        cache->nodes = new LFUNode[capacity];
        

        for (int i = 0; i < capacity; i++) {
            cache->nodes[i].isEmpty = true;
            cache->nodes[i].frequency = 0;
            cache->nodes[i].timestamp = 0;
        }
        
        cache->hashTable = createLFUHashTable(capacity);
    } else {
        cache->nodes = nullptr;
        cache->hashTable = nullptr;
    }
    
    return cache;
}

void destroyLFUCache(LFUCache* cache) {
    if (!cache) return;
    
    if (cache->nodes) {
        delete[] cache->nodes;
    }
    
    if (cache->hashTable) {
        destroyLFUHashTable(cache->hashTable);
    }
    
    delete cache;
}

int lfuFindIndex(LFUCache* cache, int key) {
    if (!cache || !cache->nodes) return -1;
    
    return lfuHashFind(cache->hashTable, key);
}

int lfuFindMinFrequency(LFUCache* cache) {
    if (!cache || !cache->nodes || cache->size == 0) return -1;
    
    int minIndex = -1;
    int minFrequency = INT_MAX;
    int minTimestamp = INT_MAX;
    
    for (int i = 0; i < cache->capacity; i++) {
        if (!cache->nodes[i].isEmpty) {
            if (cache->nodes[i].frequency < minFrequency ||
                (cache->nodes[i].frequency == minFrequency && 
                 cache->nodes[i].timestamp < minTimestamp)) {
                minFrequency = cache->nodes[i].frequency;
                minTimestamp = cache->nodes[i].timestamp;
                minIndex = i;
            }
        }
    }
    
    return minIndex;
}

void lfuSet(LFUCache* cache, int key, int value) {
    if (!cache || cache->capacity == 0) return;
    
    cache->currentTime++;
    
    int index = lfuFindIndex(cache, key);
    
    if (index != -1) {
        cache->nodes[index].value = value;
        cache->nodes[index].frequency++;
        cache->nodes[index].timestamp = cache->currentTime;
        return;
    }
    
    
    if (cache->size >= cache->capacity) {
        int minIndex = lfuFindMinFrequency(cache);
        if (minIndex != -1) {
            lfuHashRemove(cache->hashTable, cache->nodes[minIndex].key);
            
            cache->nodes[minIndex].isEmpty = true;
            cache->size--;
        }
    }
    
    for (int i = 0; i < cache->capacity; i++) {
        if (cache->nodes[i].isEmpty) {
            cache->nodes[i].key = key;
            cache->nodes[i].value = value;
            cache->nodes[i].frequency = 1;
            cache->nodes[i].timestamp = cache->currentTime;
            cache->nodes[i].isEmpty = false;
            
            lfuHashInsert(cache->hashTable, key, i);
            
            cache->size++;
            break;
        }
    }
}

int lfuGet(LFUCache* cache, int key) {
    if (!cache || !cache->nodes) return -1;
    
    cache->currentTime++;
    
    int index = lfuFindIndex(cache, key);
    
    if (index == -1) {
        return -1; 
    }
    
    cache->nodes[index].frequency++;
    cache->nodes[index].timestamp = cache->currentTime;
    
    return cache->nodes[index].value;
}

void printLFUCache(LFUCache* cache) {
    if (!cache) {
        cout << "Кэш не существует" << endl;
        return;
    }
    
    cout << "\n=== LFU Кэш ===" << endl;
    cout << "Емкость: " << cache->capacity << endl;
    cout << "Текущий размер: " << cache->size << endl;
    cout << "Текущее время: " << cache->currentTime << endl;
    
    if (cache->size == 0) {
        cout << "Кэш пуст" << endl;
        return;
    }
    
    cout << "\nСодержимое:" << endl;
    cout << "+-----+-------+-------+-----------+-----------+" << endl;
    cout << "| Idx | Key   | Value | Frequency | Timestamp |" << endl;
    cout << "+-----+-------+-------+-----------+-----------+" << endl;
    
    for (int i = 0; i < cache->capacity; i++) {
        if (!cache->nodes[i].isEmpty) {
            printf("| %3d | %5d | %5d | %9d | %9d |\n",
                   i,
                   cache->nodes[i].key,
                   cache->nodes[i].value,
                   cache->nodes[i].frequency,
                   cache->nodes[i].timestamp);
        }
    }
    
    cout << "+-----+-------+-------+-----------+-----------+" << endl;
}