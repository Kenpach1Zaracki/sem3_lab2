#include <iostream>
#include <climits>
#include "lfu.h"
using namespace std;

// ========== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ХЕШ-ТАБЛИЦЫ ==========

// Простая хеш-функция
int lfuHash(int key, int capacity) {
    return (key % capacity + capacity) % capacity;  // Обрабатываем отрицательные ключи
}

// Создать хеш-таблицу
LFUHashTable* createLFUHashTable(int capacity) {
    LFUHashTable* ht = new LFUHashTable;
    ht->capacity = capacity * 2;  // В 2 раза больше для низкого load factor
    ht->table = new LFUHashNode[ht->capacity];
    
    for (int i = 0; i < ht->capacity; i++) {
        ht->table[i].isEmpty = true;
    }
    
    return ht;
}

// Уничтожить хеш-таблицу
void destroyLFUHashTable(LFUHashTable* ht) {
    if (!ht) return;
    delete[] ht->table;
    delete ht;
}

// Вставить в хеш-таблицу (линейное пробирование)
void lfuHashInsert(LFUHashTable* ht, int key, int index) {
    if (!ht) return;
    
    int hash = lfuHash(key, ht->capacity);
    
    // Линейное пробирование
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

// Найти в хеш-таблице
int lfuHashFind(LFUHashTable* ht, int key) {
    if (!ht) return -1;
    
    int hash = lfuHash(key, ht->capacity);
    
    // Линейное пробирование
    for (int i = 0; i < ht->capacity; i++) {
        int pos = (hash + i) % ht->capacity;
        
        if (ht->table[pos].isEmpty) {
            return -1;  // Не нашли
        }
        
        if (ht->table[pos].key == key) {
            return ht->table[pos].value;  // Нашли индекс
        }
    }
    
    return -1;
}

// Удалить из хеш-таблицы
void lfuHashRemove(LFUHashTable* ht, int key) {
    if (!ht) return;
    
    int hash = lfuHash(key, ht->capacity);
    
    // Линейное пробирование
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

// ========== ОСНОВНЫЕ ФУНКЦИИ LFU ==========

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
        
        // Создаём хеш-таблицу для быстрого поиска
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
    
    // O(1) поиск через свою хеш-таблицу!
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
    
    // Проверяем, существует ли ключ (O(1) через хеш!)
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
            // Удаляем из хеш-таблицы
            lfuHashRemove(cache->hashTable, cache->nodes[minIndex].key);
            
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
            cache->nodes[i].frequency = 1;
            cache->nodes[i].timestamp = cache->currentTime;
            cache->nodes[i].isEmpty = false;
            
            // Добавляем в хеш-таблицу (O(1))
            lfuHashInsert(cache->hashTable, key, i);
            
            cache->size++;
            break;
        }
    }
}

int lfuGet(LFUCache* cache, int key) {
    if (!cache || !cache->nodes) return -1;
    
    cache->currentTime++;
    
    // O(1) поиск через хеш!
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