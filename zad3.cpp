#include <iostream>
#include <sstream>
#include "zad3.h"
#include "set.h"
#include "array.h"
using namespace std;

// Структура для хранения пары множеств
struct SetPair {
    int index1;
    int index2;
    int commonCount;
};

// Подсчитать количество общих элементов между двумя множествами
int countCommonElements(Set* set1, Set* set2) {
    if (!set1 || !set2) return 0;
    
    int size1 = setGetSize(set1);
    if (size1 == 0) return 0;
    
    // Получаем все элементы первого множества
    string* elements = new string[size1 + 1];
    int count = 0;
    setGetElements(set1, elements, count);
    
    // Проверяем каждый элемент во втором множестве
    int commonCount = 0;
    for (int i = 0; i < count; i++) {
        if (setAt(set2, elements[i]) != -1) {
            commonCount++;
        }
    }
    
    delete[] elements;
    return commonCount;
}

// Вывести общие элементы двух множеств
void printCommonElements(Set* set1, Set* set2) {
    if (!set1 || !set2) return;
    
    int size1 = setGetSize(set1);
    if (size1 == 0) {
        cout << "(нет общих элементов)";
        return;
    }
    
    string* elements = new string[size1 + 1];
    int count = 0;
    setGetElements(set1, elements, count);
    
    bool first = true;
    for (int i = 0; i < count; i++) {
        if (setAt(set2, elements[i]) != -1) {
            if (!first) cout << ", ";
            cout << elements[i];
            first = false;
        }
    }
    
    if (first) {
        cout << "(нет общих элементов)";
    }
    
    delete[] elements;
}

void findMaxIntersection() {
    cout << "=== Задание 3: Максимальное количество пересечений ===" << endl;
    
    int n;
    cout << "Введите количество множеств: ";
    cin >> n;
    cin.ignore();
    
    if (n <= 0) {
        cout << "Ошибка: Количество множеств должно быть положительным" << endl;
        return;
    }
    
    if (n < 2) {
        cout << "Ошибка: Необходимо минимум 2 множества для поиска пересечений" << endl;
        return;
    }
    
    // Создаем массив множеств
    Set** sets = new Set*[n];
    
    // Вводим элементы для каждого множества
    for (int i = 0; i < n; i++) {
        sets[i] = createSet();
        
        cout << "Введите элементы множества " << (i + 1) << " (через пробел): ";
        string line;
        getline(cin, line);
        
        if (line.empty()) {
            cout << "Предупреждение: Множество " << (i + 1) << " пустое" << endl;
            continue;
        }
        
        // Парсим строку
        istringstream iss(line);
        string element;
        while (iss >> element) {
            setAdd(sets[i], element);
        }
    }
    
    // Выводим введенные множества
    cout << "\n--- Введенные множества ---" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Множество " << (i + 1) << ": ";
        printSet(sets[i]);
    }
    cout << endl;
    
    // Ищем пары с максимальным пересечением
    int maxCommon = -1;
    
    // Сначала найдем максимальное количество общих элементов
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int common = countCommonElements(sets[i], sets[j]);
            if (common > maxCommon) {
                maxCommon = common;
            }
        }
    }
    
    // Теперь собираем все пары с максимальным количеством
    SetPair* pairs = new SetPair[n * n];  // Максимум возможных пар
    int pairCount = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int common = countCommonElements(sets[i], sets[j]);
            if (common == maxCommon) {
                pairs[pairCount].index1 = i;
                pairs[pairCount].index2 = j;
                pairs[pairCount].commonCount = common;
                pairCount++;
            }
        }
    }
    
    // Выводим результаты
    if (pairCount == 0 || maxCommon == 0) {
        cout << "Нет множеств с общими элементами" << endl;
    } else {
        cout << "--- Результат ---" << endl;
        cout << "Максимальное количество общих элементов: " << maxCommon << endl;
        cout << "Найдено пар с таким пересечением: " << pairCount << endl;
        cout << endl;
        
        for (int p = 0; p < pairCount; p++) {
            int i = pairs[p].index1;
            int j = pairs[p].index2;
            
            cout << "Пара " << (p + 1) << ":" << endl;
            cout << "  Множество " << (i + 1) << ": ";
            printSet(sets[i]);
            cout << "  Множество " << (j + 1) << ": ";
            printSet(sets[j]);
            cout << "  Общие элементы (" << pairs[p].commonCount << "): ";
            printCommonElements(sets[i], sets[j]);
            cout << endl << endl;
        }
    }
    
    // Освобождаем память
    delete[] pairs;
    for (int i = 0; i < n; i++) {
        destroySet(sets[i]);
    }
    delete[] sets;
}
