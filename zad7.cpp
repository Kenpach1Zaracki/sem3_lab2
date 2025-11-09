#include <iostream>
#include <sstream>
#include "zad7.h"
#include "lfu.h"
#include "array.h"
using namespace std;

void demonstrateLFUExample() {
    cout << "\n=== ДЕМОНСТРАЦИЯ ПРИМЕРА ИЗ ТЗ ===" << endl;
    cout << "\nПараметры:" << endl;
    cout << "Емкость (cap) = 2" << endl;
    cout << "Запросы (Q) = 10" << endl;
    cout << "\nЗапросы: SET(5,7) SET(4,6) SET(3,5) SET(2,4) SET(1,3) GET(1) GET(2) GET(3) GET(4) GET(5)" << endl;
    cout << "\nОжидаемый вывод: 3 4 5 -1 -1" << endl;
    
    // Создаем кэш емкостью 2
    LFUCache* cache = createLFUCache(2);
    
    Array* results = create_array("results");
    
    cout << "\n--- Пошаговое выполнение ---" << endl;
    
    // SET(5,7)
    cout << "\n[1] SET(5, 7)" << endl;
    lfuSet(cache, 5, 7);
    cout << "    Кэш: {5:7}" << endl;
    printLFUCache(cache);
    
    // SET(4,6)
    cout << "\n[2] SET(4, 6)" << endl;
    lfuSet(cache, 4, 6);
    cout << "    Кэш: {5:7, 4:6}" << endl;
    printLFUCache(cache);
    
    // SET(3,5)
    cout << "\n[3] SET(3, 5)" << endl;
    cout << "    Кэш заполнен! Удаляем элемент с минимальной частотой..." << endl;
    cout << "    5 имеет frequency=1, timestamp=1" << endl;
    cout << "    4 имеет frequency=1, timestamp=2" << endl;
    cout << "    Удаляем 5 (меньший timestamp)" << endl;
    lfuSet(cache, 3, 5);
    cout << "    Кэш: {4:6, 3:5}" << endl;
    printLFUCache(cache);
    
    // SET(2,4)
    cout << "\n[4] SET(2, 4)" << endl;
    cout << "    Кэш заполнен! Удаляем элемент с минимальной частотой..." << endl;
    cout << "    Удаляем 4" << endl;
    lfuSet(cache, 2, 4);
    cout << "    Кэш: {3:5, 2:4}" << endl;
    printLFUCache(cache);
    
    // SET(1,3)
    cout << "\n[5] SET(1, 3)" << endl;
    cout << "    Кэш заполнен! Удаляем элемент с минимальной частотой..." << endl;
    cout << "    Удаляем 3" << endl;
    lfuSet(cache, 1, 3);
    cout << "    Кэш: {2:4, 1:3}" << endl;
    printLFUCache(cache);
    
    // GET(1)
    cout << "\n[6] GET(1)" << endl;
    int result1 = lfuGet(cache, 1);
    array_push_back(results, to_string(result1));
    cout << "    Возвращаем: " << result1 << endl;
    cout << "    Обновляем frequency(1) = 2" << endl;
    printLFUCache(cache);
    
    // GET(2)
    cout << "\n[7] GET(2)" << endl;
    int result2 = lfuGet(cache, 2);
    array_push_back(results, to_string(result2));
    cout << "    Возвращаем: " << result2 << endl;
    cout << "    Обновляем frequency(2) = 2" << endl;
    printLFUCache(cache);
    
    // GET(3)
    cout << "\n[8] GET(3)" << endl;
    int result3 = lfuGet(cache, 3);
    array_push_back(results, to_string(result3));
    cout << "    Возвращаем: " << result3 << " (не в кэше)" << endl;
    
    // GET(4)
    cout << "\n[9] GET(4)" << endl;
    int result4 = lfuGet(cache, 4);
    array_push_back(results, to_string(result4));
    cout << "    Возвращаем: " << result4 << " (не в кэше)" << endl;
    
    // GET(5)
    cout << "\n[10] GET(5)" << endl;
    int result5 = lfuGet(cache, 5);
    array_push_back(results, to_string(result5));
    cout << "    Возвращаем: " << result5 << " (не в кэше)" << endl;
    
    // Выводим результаты
    cout << "\n=== РЕЗУЛЬТАТ ===" << endl;
    cout << "Вывод: ";
    for (int i = 0; i < array_length(results); i++) {
        if (i > 0) cout << " ";
        cout << array_get(results, i);
    }
    cout << endl;
    
    cout << "Ожидалось: 3 4 5 -1 -1" << endl;
    
    // Проверка
    string expected = "3 4 5 -1 -1";
    string actual = array_read(results);
    
    if (actual == expected) {
        cout << "✓ Тест пройден успешно!" << endl;
    } else {
        cout << "✗ Тест не пройден" << endl;
    }
    
    destroy_array(results);
    destroyLFUCache(cache);
}

void interactiveLFUMode() {
    cout << "\n=== ИНТЕРАКТИВНЫЙ РЕЖИМ LFU ===" << endl;
    
    int capacity;
    cout << "Введите емкость кэша: ";
    cin >> capacity;
    cin.ignore();
    
    if (capacity <= 0) {
        cout << "Ошибка: Емкость должна быть положительной" << endl;
        return;
    }
    
    LFUCache* cache = createLFUCache(capacity);
    cout << "✓ LFU кэш создан с емкостью " << capacity << endl;
    
    string line;
    while (true) {
        cout << "\n--- Команды ---" << endl;
        cout << "SET <key> <value> - установить значение" << endl;
        cout << "GET <key> - получить значение" << endl;
        cout << "PRINT - вывести кэш" << endl;
        cout << "EXIT - выход" << endl;
        cout << "\n> ";
        
        getline(cin, line);
        
        if (line.empty()) continue;
        
        istringstream iss(line);
        string command;
        iss >> command;
        
        if (command == "EXIT" || command == "exit") {
            break;
        }
        else if (command == "SET") {
            int key, value;
            if (!(iss >> key >> value)) {
                cout << "Ошибка: Требуется ключ и значение" << endl;
                continue;
            }
            
            lfuSet(cache, key, value);
            cout << "✓ SET(" << key << ", " << value << ") выполнено" << endl;
        }
        else if (command == "GET") {
            int key;
            if (!(iss >> key)) {
                cout << "Ошибка: Требуется ключ" << endl;
                continue;
            }
            
            int result = lfuGet(cache, key);
            if (result != -1) {
                cout << "✓ GET(" << key << ") = " << result << endl;
            } else {
                cout << "✗ GET(" << key << ") = -1 (не найдено)" << endl;
            }
        }
        else if (command == "PRINT") {
            printLFUCache(cache);
        }
        else {
            cout << "Неизвестная команда: " << command << endl;
        }
    }
    
    destroyLFUCache(cache);
}

void batchLFUMode() {
    cout << "\n=== ПАКЕТНЫЙ РЕЖИМ LFU ===" << endl;
    
    int capacity, Q;
    cout << "Введите емкость кэша: ";
    cin >> capacity;
    
    if (cin.fail() || capacity <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: Неверная емкость" << endl;
        return;
    }
    
    cout << "Введите количество запросов (Q): ";
    cin >> Q;
    cin.ignore();
    
    if (cin.fail() || Q <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: Неверное количество запросов" << endl;
        return;
    }
    
    LFUCache* cache = createLFUCache(capacity);
    Array* results = create_array("results");
    
    cout << "\nВведите " << Q << " запросов (SET <key> <value> или GET <key>):" << endl;
    
    for (int i = 0; i < Q; i++) {
        cout << "Запрос " << (i+1) << ": ";
        string line;
        getline(cin, line);
        
        istringstream iss(line);
        string command;
        iss >> command;
        
        if (command == "SET") {
            int key, value;
            if (!(iss >> key >> value)) {
                cout << "Ошибка: Неверный формат SET" << endl;
                i--;
                continue;
            }
            lfuSet(cache, key, value);
        }
        else if (command == "GET") {
            int key;
            if (!(iss >> key)) {
                cout << "Ошибка: Неверный формат GET" << endl;
                i--;
                continue;
            }
            int result = lfuGet(cache, key);
            array_push_back(results, to_string(result));
        }
        else {
            cout << "Ошибка: Неизвестная команда" << endl;
            i--;
        }
    }
    
    // Выводим результаты GET операций
    if (array_length(results) > 0) {
        cout << "\n--- Результаты GET операций ---" << endl;
        cout << "Вывод: ";
        for (int i = 0; i < array_length(results); i++) {
            if (i > 0) cout << " ";
            cout << array_get(results, i);
        }
        cout << endl;
    } else {
        cout << "\nНет GET операций для вывода" << endl;
    }
    
    cout << "\n--- Финальное состояние кэша ---" << endl;
    printLFUCache(cache);
    
    destroy_array(results);
    destroyLFUCache(cache);
}

void processLFU() {
    cout << "=== Задание 7: LFU Кэш ===" << endl;
    cout << "\nLFU (Least Frequently Used) - кэш, который удаляет элемент" << endl;
    cout << "с наименьшей частотой использования при заполнении." << endl;
    
    cout << "\nВыберите режим:" << endl;
    cout << "1. Демонстрация примера из ТЗ" << endl;
    cout << "2. Интерактивный режим" << endl;
    cout << "3. Пакетный режим" << endl;
    cout << "Ваш выбор: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch (choice) {
        case 1:
            demonstrateLFUExample();
            break;
        case 2:
            interactiveLFUMode();
            break;
        case 3:
            batchLFUMode();
            break;
        default:
            cout << "Неверный выбор" << endl;
    }
}
