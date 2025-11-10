#include <iostream>
#include <sstream>
#include "zad7.h"
#include "lfu.h"
using namespace std;

void processLFU() {
    cout << "=== Задание 7: LFU кэш ===" << endl;
    
    int capacity;
    cout << "Введите емкость кэша (capacity): ";
    cin >> capacity;
    
    if (cin.fail() || capacity <= 0 || capacity > 1000) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: неверная емкость" << endl;
        return;
    }
    
    int Q;
    cout << "Введите количество запросов (Q): ";
    cin >> Q;
    
    if (cin.fail() || Q <= 0 || Q > 10000) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: неверное количество запросов" << endl;
        return;
    }
    
    cin.ignore();
    
    LFUCache* cache = createLFUCache(capacity);
    
    cout << "\nВведите запросы (SET x y / GET x):" << endl;
    
    string results = "";
    
    for (int i = 0; i < Q; i++) {
        string line;
        getline(cin, line);
        
        if (line.empty()) {
            i--;
            continue;
        }
        
        stringstream ss(line);
        string command;
        ss >> command;
        
        if (command == "SET") {
            int key, value;
            ss >> key >> value;
            lfuSet(cache, key, value);
            cout << "SET(" << key << ", " << value << ") выполнено" << endl;
        }
        else if (command == "GET") {
            int key;
            ss >> key;
            int result = lfuGet(cache, key);
            
            if (!results.empty()) results += " ";
            results += to_string(result);
            
            cout << "GET(" << key << ") = " << result << endl;
        }
        else {
            cout << "Ошибка: неизвестная команда '" << command << "'" << endl;
            i--;
        }
    }
    
    // Вывод результатов GET запросов
    if (!results.empty()) {
        cout << "\n--- Результаты GET запросов ---" << endl;
        cout << results << endl;
    }
    
    // Вывод состояния кэша
    cout << "\n--- Состояние кэша ---" << endl;
    cout << "Емкость: " << cache->capacity << endl;
    cout << "Текущий размер: " << cache->size << endl;
    
    if (cache->size > 0) {
        cout << "\nСодержимое:" << endl;
        for (int i = 0; i < cache->capacity; i++) {
            if (!cache->nodes[i].isEmpty) {
                cout << "Ключ: " << cache->nodes[i].key 
                     << ", Значение: " << cache->nodes[i].value
                     << ", Частота: " << cache->nodes[i].frequency
                     << ", Время: " << cache->nodes[i].timestamp << endl;
            }
        }
    } else {
        cout << "Кэш пуст" << endl;
    }
    
    destroyLFUCache(cache);
}