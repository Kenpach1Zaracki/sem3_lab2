#include <iostream>
#include <ctime>
#include <cstdlib>
#include "zad6.h"
using namespace std;

int hashSimple(const string& key, int capacity) {
    if (capacity <= 0) return 0;
    
    unsigned long hash = 0;
    for (size_t i = 0; i < key.length(); i++) {
        hash = hash * 31 + key[i];
    }
    return hash % capacity;
}

HashTableOpen* createHashTableOpen(int capacity) {
    HashTableOpen* table = new HashTableOpen;
    table->capacity = capacity;
    table->size = 0;
    table->table = new HashNode[capacity];
    
    for (int i = 0; i < capacity; i++) {
        table->table[i].isEmpty = true;
        table->table[i].isDeleted = false;
    }
    
    return table;
}

void destroyHashTableOpen(HashTableOpen* table) {
    if (!table) return;
    delete[] table->table;
    delete table;
}

double getLoadFactorOpen(HashTableOpen* table) {
    if (!table || table->capacity == 0) return 0.0;
    return (double)table->size / table->capacity;
}

bool rehashHashTableOpen(HashTableOpen* table) {
    if (!table) return false;
    
    int oldCapacity = table->capacity;
    HashNode* oldTable = table->table;
    
    table->capacity = oldCapacity * 2;
    table->size = 0;
    table->table = new HashNode[table->capacity];
    
    for (int i = 0; i < table->capacity; i++) {
        table->table[i].isEmpty = true;
        table->table[i].isDeleted = false;
    }
    
    for (int i = 0; i < oldCapacity; i++) {
        if (!oldTable[i].isEmpty && !oldTable[i].isDeleted) {
            insertHashTableOpen(table, oldTable[i].key, oldTable[i].value);
        }
    }
    
    delete[] oldTable;
    return true;
}

bool insertHashTableOpen(HashTableOpen* table, const string& key, int value) {
    if (!table || key.empty()) return false;
    
    if (getLoadFactorOpen(table) >= 0.9) {
        rehashHashTableOpen(table);
    }
    
    int hash = hashSimple(key, table->capacity);
    int index = hash;
    
    for (int i = 0; i < table->capacity; i++) {
        index = (hash + i) % table->capacity;
        
        if (table->table[index].isEmpty || table->table[index].isDeleted) {
            table->table[index].key = key;
            table->table[index].value = value;
            table->table[index].isEmpty = false;
            table->table[index].isDeleted = false;
            table->size++;
            return true;
        }
        
        if (table->table[index].key == key) {
            table->table[index].value = value;
            return true;
        }
    }
    
    return false;
}

int searchHashTableOpen(HashTableOpen* table, const string& key) {
    if (!table || key.empty()) return -1;
    
    int hash = hashSimple(key, table->capacity);
    int index = hash;
    
    for (int i = 0; i < table->capacity; i++) {
        index = (hash + i) % table->capacity;
        
        if (table->table[index].isEmpty && !table->table[index].isDeleted) {
            return -1;
        }
        
        if (!table->table[index].isEmpty && !table->table[index].isDeleted &&
            table->table[index].key == key) {
            return table->table[index].value;
        }
    }
    
    return -1;
}

HashTableChain* createHashTableChain(int capacity) {
    HashTableChain* table = new HashTableChain;
    table->capacity = capacity;
    table->size = 0;
    table->table = new ChainNode*[capacity];
    
    for (int i = 0; i < capacity; i++) {
        table->table[i] = nullptr;
    }
    
    return table;
}

void destroyHashTableChain(HashTableChain* table) {
    if (!table) return;
    
    for (int i = 0; i < table->capacity; i++) {
        ChainNode* current = table->table[i];
        while (current) {
            ChainNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    delete[] table->table;
    delete table;
}

double getLoadFactorChain(HashTableChain* table) {
    if (!table || table->capacity == 0) return 0.0;
    return (double)table->size / table->capacity;
}

bool rehashHashTableChain(HashTableChain* table) {
    if (!table) return false;
    
    int oldCapacity = table->capacity;
    ChainNode** oldTable = table->table;
    
    table->capacity = oldCapacity * 2;
    table->size = 0;
    table->table = new ChainNode*[table->capacity];
    
    for (int i = 0; i < table->capacity; i++) {
        table->table[i] = nullptr;
    }
    
    for (int i = 0; i < oldCapacity; i++) {
        ChainNode* current = oldTable[i];
        while (current) {
            insertHashTableChain(table, current->key, current->value);
            ChainNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    
    delete[] oldTable;
    return true;
}

bool insertHashTableChain(HashTableChain* table, const string& key, int value) {
    if (!table || key.empty()) return false;
    
    if (getLoadFactorChain(table) >= 0.9) {
        rehashHashTableChain(table);
    }
    
    int index = hashSimple(key, table->capacity);
    
    ChainNode* current = table->table[index];
    while (current) {
        if (current->key == key) {
            current->value = value;
            return true;
        }
        current = current->next;
    }
    
    ChainNode* newNode = new ChainNode;
    newNode->key = key;
    newNode->value = value;
    newNode->next = table->table[index];
    table->table[index] = newNode;
    table->size++;
    
    return true;
}

int searchHashTableChain(HashTableChain* table, const string& key) {
    if (!table || key.empty()) return -1;
    
    int index = hashSimple(key, table->capacity);
    
    ChainNode* current = table->table[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return -1;
}

void processHashTables() {
    cout << "=== Задание 6: Хеш-таблицы ===" << endl;
    
    int N, M;
    cout << "Введите количество элементов: ";
    cin >> N;
    cout << "Введите количество поисков: ";
    cin >> M;
    
    int initialCapacity = N / 2;
    
    HashTableOpen* tableOpen = createHashTableOpen(initialCapacity);
    HashTableChain* tableChain = createHashTableChain(initialCapacity);
    
    string* keys = new string[N];
    srand(time(0));
    
    for (int i = 0; i < N; i++) {
        long long num = 100000000LL + (rand() % 900000000LL);
        keys[i] = to_string(num);
    }
    
    cout << "\nВставка в открытую адресацию..." << endl;
    int rehashOpen = 0;
    clock_t start = clock();
    
    for (int i = 0; i < N; i++) {
        int oldCap = tableOpen->capacity;
        insertHashTableOpen(tableOpen, keys[i], i);
        if (tableOpen->capacity > oldCap) {
            rehashOpen++;
            cout << "Реструктуризация " << oldCap << " -> " << tableOpen->capacity << endl;
        }
    }
    
    clock_t end = clock();
    double timeOpen = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    cout << "Время: " << timeOpen << " мс" << endl;
    cout << "Размер таблицы: " << tableOpen->size << "/" << tableOpen->capacity << endl;
    
    cout << "\nВставка в метод цепочек..." << endl;
    int rehashChain = 0;
    start = clock();
    
    for (int i = 0; i < N; i++) {
        int oldCap = tableChain->capacity;
        insertHashTableChain(tableChain, keys[i], i);
        if (tableChain->capacity > oldCap) {
            rehashChain++;
            cout << "Реструктуризация " << oldCap << " -> " << tableChain->capacity << endl;
        }
    }
    
    end = clock();
    double timeChain = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    cout << "Время: " << timeChain << " мс" << endl;
    cout << "Размер таблицы: " << tableChain->size << "/" << tableChain->capacity << endl;
    
    cout << "\nПоиск в открытой адресации..." << endl;
    start = clock();
    
    int foundOpen = 0;
    for (int i = 0; i < M; i++) {
        int idx = rand() % N;
        if (searchHashTableOpen(tableOpen, keys[idx]) != -1) foundOpen++;
    }
    
    end = clock();
    double searchOpen = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    cout << "Найдено: " << foundOpen << "/" << M << endl;
    cout << "Время: " << searchOpen << " мс" << endl;
    
    cout << "\nПоиск в методе цепочек..." << endl;
    start = clock();
    
    int foundChain = 0;
    for (int i = 0; i < M; i++) {
        int idx = rand() % N;
        if (searchHashTableChain(tableChain, keys[idx]) != -1) foundChain++;
    }
    
    end = clock();
    double searchChain = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    cout << "Найдено: " << foundChain << "/" << M << endl;
    cout << "Время: " << searchChain << " мс" << endl;
    
    cout << "\nСравнение:" << endl;
    cout << "Вставка: открытая=" << timeOpen << "мс, цепочки=" << timeChain << "мс" << endl;
    cout << "Поиск: открытая=" << searchOpen << "мс, цепочки=" << searchChain << "мс" << endl;
    
    delete[] keys;
    destroyHashTableOpen(tableOpen);
    destroyHashTableChain(tableChain);
}