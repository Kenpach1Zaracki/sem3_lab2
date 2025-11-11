#include <iostream>
#include <cstring>
#include "zad1.h"
#include "zad2.h"
#include "zad3.h"
#include "zad4.h"
#include "zad5.h"
#include "zad6.h"
#include "zad7.h"

using namespace std;

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--task2") == 0) {
            processSetOperations(argc, argv);
            return 0;
        }
    }
    
    int choice;
    
    do {
        cout << "\n1. Вычисление выражения" << endl;
        cout << "2. АТД Множество" << endl;
        cout << "3. Максимальное количество пересечений" << endl;
        cout << "4. Пирамида из блоков" << endl;
        cout << "5. Бинарное дерево поиска" << endl;
        cout << "6. Хеш-таблицы" << endl;
        cout << "7. LFU кэш" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите задание: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                calculateExpression();
                break;
            case 2:
                processSetOperations(argc, argv);
                break;
            case 3:
                findMaxIntersection();
                break;
            case 4:
                buildMaxPyramid();
                break;
            case 5:
                processHuffman();
                break;
            case 6:
                processHashTables();
                break;
            case 7:
                processLFU();
                break;
            case 0:
                cout << "Выход из программы" << endl;
                break;
            default:
                cout << "Неверный выбор" << endl;
        }
    } while (choice != 0);
    
    return 0;
}