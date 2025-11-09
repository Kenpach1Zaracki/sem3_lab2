#include <iostream>
#include <climits>
#include "lfu.h"
using namespace std;

LFUCache* createLFUCache(int capacity) {
    LFUCache* cache = new LFUCache;
    cache->capacity = capacity;
    cache->size = 0;
    cache->currentTime = 0;
    
    if (capacity > 0) {
        cache->nodes = new LFUNode[capacity];
        
        // Инициализация
        for (int i = 0; i < capacity; i++) {
            cache->nodes[i].isEmpty = true;
            cache->nodes[i].frequency = 0;
            cache->nodes[i].timestamp = 0;
        }
    } else {
        cache->nodes = nullptr;
    }
    
    return cache;
}

void destroyLFUCache(LFUCache* cache) {
    if (!cache) return;
    
    if (cache->nodes) {
        delete[] cache->nodes;
    }
    delete cache;
}

int lfuFindIndex(LFUCache* cache, int key) {
    if (!cache || !cache->nodes) return -1;
    
    for (int i = 0; i < cache->capacity; i++) {
        if (!cache->nodes[i].isEmpty && cache->nodes[i].key == key) {
            return i;
        }
    }
    
    return -1;
}

int lfuFindMinFrequency(LFUCache* cache) {
    if (!cache || !cache->nodes || cache->size == 0) return -1;
    
    int minIndex = -1;
    int minFrequency = INT_MAX;
    int minTimestamp = INT_MAX;
    
    for (int i = 0; i < cache->capacity; i++) {
        if (!cache->nodes[i].isEmpty) {
            // Ищем элемент с минимальной частотой
            // При равенстве частот - с минимальным timestamp (давно не использовался)
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
    
    // Проверяем, существует ли ключ
    int index = lfuFindIndex(cache, key);
    
    if (index != -1) {
        // Ключ существует - обновляем
        cache->nodes[index].value = value;
        cache->nodes[index].frequency++;
        cache->nodes[index].timestamp = cache->currentTime;
        return;
    }
    
    // Ключ не существует - нужно вставить
    
    // Если кэш заполнен - удаляем элемент с минимальной частотой
    if (cache->size >= cache->capacity) {
        int minIndex = lfuFindMinFrequency(cache);
        if (minIndex != -1) {
            // Удаляем элемент
            cache->nodes[minIndex].isEmpty = true;
            cache->size--;
        }
    }
    
    // Ищем пустую ячейку и вставляем
    for (int i = 0; i < cache->capacity; i++) {
        if (cache->nodes[i].isEmpty) {
            cache->nodes[i].key = key;
            cache->nodes[i].value = value;
            cache->nodes[i].frequency = 1;  // Начальная частота
            cache->nodes[i].timestamp = cache->currentTime;
            cache->nodes[i].isEmpty = false;
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
        return -1;  // Ключ не найден
    }
    
    // Ключ найден - обновляем статистику
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
