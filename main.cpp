#include <iostream>
#include <limits>
#include "zad1.h"
#include "zad2.h"
#include "zad3.h"
#include "zad4.h"
#include "zad5.h"
#include "zad6.h"
#include "zad7.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            if (arg == "--file" || arg == "--query") {
                processSetOperations(argc, argv);
                return 0;
            }
        }
    }
    
    int choice;
    do {
        cout << "\n1. Вычисление выражения" << endl;
        cout << "2. АТД Множество" << endl;
        cout << "3. Максимальное количество пересечений" << endl;
        cout << "4. Пирамида из блоков" << endl;
        cout << "5. Алгоритм Хаффмана" << endl;
        cout << "6. Хеш-таблицы" << endl;
        cout << "7. LFU кэш" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите задание: ";
        
        try {
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Неверный ввод");
            }
            cin.ignore();

            switch (choice) {
                case 1:
                    calculateExpression();
                    break;
                case 2:
                    interactiveSetMode();
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
                    cout << "Выход..." << endl;
                    break;
                default:
                    cout << "Неверный выбор" << endl;
            }
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}
