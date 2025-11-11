#include <iostream>
#include <sstream>
#include "zad3.h"
#include "set.h"
#include "array.h"
using namespace std;

struct SetPair {
    int index1;
    int index2;
    int commonCount;
};

int countCommonElements(Set* set1, Set* set2) {
    if (!set1 || !set2) return 0;
    
    int size1 = setGetSize(set1);
    if (size1 == 0) return 0;
    
    string* elements = new string[size1 + 1];
    int count = 0;
    setGetElements(set1, elements, count);
    
    int commonCount = 0;
    for (int i = 0; i < count; i++) {
        if (setAt(set2, elements[i]) != -1) {
            commonCount++;
        }
    }
    
    delete[] elements;
    return commonCount;
}

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
    
    if (n <= 0) {
        cout << "Ошибка: Количество множеств должно быть положительным" << endl;
        return;
    }
    
    if (n < 2) {
        cout << "Ошибка: Необходимо минимум 2 множества для поиска пересечений" << endl;
        return;
    }
    
    Set** sets = new Set*[n];
    
    for (int i = 0; i < n; i++) {
        sets[i] = createSet(16);  
        
        int elemCount;
        cout << "Множество " << (i + 1) << ". Количество элементов: ";
        cin >> elemCount;
        
        if (elemCount <= 0) {
            cout << "Предупреждение: Множество " << (i + 1) << " пустое" << endl;
            continue;
        }
        
        cout << "Введите элементы: ";
        for (int j = 0; j < elemCount; j++) {
            string element;
            cin >> element;
            setAdd(sets[i], element);
        }
    }
    
    cout << "\n--- Введенные множества ---" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Множество " << (i + 1) << ": ";
        printSet(sets[i]);
    }
    cout << endl;
    
    int maxCommon = -1;
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int common = countCommonElements(sets[i], sets[j]);
            if (common > maxCommon) {
                maxCommon = common;
            }
        }
    }
    
    SetPair* pairs = new SetPair[n * n];
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
    
    cout << "\nМаксимальное количество общих элементов: " << maxCommon << endl;
    
    if (pairCount == 0 || maxCommon == 0) {
        cout << "Нет множеств с общими элементами" << endl;
    } else {
        cout << "Пары множеств с максимальным пересечением:" << endl;
        
        for (int p = 0; p < pairCount; p++) {
            int i = pairs[p].index1;
            int j = pairs[p].index2;
            
            cout << "  Множество " << (i + 1) << " и Множество " << (j + 1) 
                 << ": общих элементов = " << pairs[p].commonCount << " (";
            printCommonElements(sets[i], sets[j]);
            cout << ")" << endl;
        }
    }
    
    delete[] pairs;
    for (int i = 0; i < n; i++) {
        destroySet(sets[i]);
    }
    delete[] sets;
}