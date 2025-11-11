#include <iostream>
#include <fstream>
#include <sstream>
#include "zad2.h"
#include "set.h"
#include "map_simple.h"

using namespace std;

void executeCommand(SimpleMap* setMap, const string& line, bool silent = false) {
    if (line.empty()) return;
    
    stringstream ss(line);
    string command;
    ss >> command;
    
    if (command == "SETADD") {
        string setName, element;
        ss >> setName >> element;
        
        Set* currentSet = simpleMapFind(setMap, setName);
        if (!currentSet) {
            currentSet = createSet(16);
            simpleMapInsert(setMap, setName, currentSet);
        }
        
        setAdd(currentSet, element);
        if (!silent) {
            cout << "SETADD " << setName << " " << element << " - выполнено" << endl;
        }
    }
    else if (command == "SETDEL") {
        string setName, element;
        ss >> setName >> element;
        
        Set* currentSet = simpleMapFind(setMap, setName);
        if (currentSet) {
            setDelete(currentSet, element);
            if (!silent) {
                cout << "SETDEL " << setName << " " << element << " - выполнено" << endl;
            }
        }
    }
    else if (command == "SET_AT") {
        string setName, element;
        ss >> setName >> element;
        
        Set* currentSet = simpleMapFind(setMap, setName);
        if (currentSet) {
            int found = setAt(currentSet, element);
            cout << (found != -1 ? "YES" : "NO") << endl;
        } else {
            cout << "NO" << endl;
        }
    }
}

void interactiveSetMode() {
    cout << "\n=== Интерактивный режим ===" << endl;
    cout << "Команды:" << endl;
    cout << "  SETADD <name> <element> - добавить элемент" << endl;
    cout << "  SETDEL <name> <element> - удалить элемент" << endl;
    cout << "  SET_AT <name> <element> - проверить наличие" << endl;
    cout << "  PRINT <name> - показать множество" << endl;
    cout << "  EXIT - выход" << endl;
    
    SimpleMap* setMap = createSimpleMap();
    string command;
    
    while (true) {
        cout << "\n> ";
        cin >> command;
        
        if (command == "EXIT") {
            break;
        }
        else if (command == "SETADD") {
            string setName, element;
            cin >> setName >> element;
            
            Set* currentSet = simpleMapFind(setMap, setName);
            if (!currentSet) {
                currentSet = createSet(16);
                simpleMapInsert(setMap, setName, currentSet);
            }
            
            setAdd(currentSet, element);
            cout << "Добавлено: " << element << " в '" << setName << "'" << endl;
        }
        else if (command == "SETDEL") {
            string setName, element;
            cin >> setName >> element;
            
            Set* currentSet = simpleMapFind(setMap, setName);
            if (currentSet) {
                setDelete(currentSet, element);
                cout << "Удалено: " << element << " из '" << setName << "'" << endl;
            } else {
                cout << "Множество '" << setName << "' не существует" << endl;
            }
        }
        else if (command == "SET_AT") {
            string setName, element;
            cin >> setName >> element;
            
            Set* currentSet = simpleMapFind(setMap, setName);
            if (currentSet) {
                int found = setAt(currentSet, element);
                if (found != -1) {
                    cout << "Найдено: " << element << " в '" << setName << "'" << endl;
                } else {
                    cout << "Не найдено: " << element << " в '" << setName << "'" << endl;
                }
            } else {
                cout << "Множество '" << setName << "' не существует" << endl;
            }
        }
        else if (command == "PRINT") {
            string setName;
            cin >> setName;
            
            Set* currentSet = simpleMapFind(setMap, setName);
            if (currentSet) {
                cout << "Множество '" << setName << "': ";
                printSet(currentSet);
            } else {
                cout << "Множество '" << setName << "' не существует" << endl;
            }
        }
        else {
            cout << "Неизвестная команда" << endl;
        }
    }
    
    destroySimpleMap(setMap);
}

void processSetOperations(int argc, char* argv[]) {
    string filename = "";
    string query = "";
    bool isTask2Mode = false;
    
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--task2") {
            isTask2Mode = true;
        }
        else if (string(argv[i]) == "--file" && i + 1 < argc) {
            filename = argv[i + 1];
            i++;
        }
        else if (string(argv[i]) == "--query" && i + 1 < argc) {
            query = argv[i + 1];
            i++;
        }
    }
    
    if (isTask2Mode && !filename.empty() && !query.empty()) {
        Set* set = createSet(16);
        
        ifstream infile(filename);
        if (infile.is_open()) {
            string element;
            while (infile >> element) {
                setAdd(set, element);
            }
            infile.close();
        }
        
        stringstream ss(query);
        string command, element;
        ss >> command >> element;
        
        if (command == "SETADD") {
            setAdd(set, element);
        }
        else if (command == "SETDEL") {
            setDelete(set, element);
        }
        else if (command == "SET_AT") {
            int found = setAt(set, element);
            cout << (found != -1 ? "YES" : "NO") << endl;
        }
        
        ofstream outfile(filename);
        if (outfile.is_open()) {
            int count = 0;
            string* elements = getAllElements(set, count);
            
            if (elements && count > 0) {
                for (int i = 0; i < count; i++) {
                    outfile << elements[i];
                    if (i < count - 1) {
                        outfile << " ";
                    }
                }
                delete[] elements;
            }
            outfile.close();
        }
        
        destroySet(set);
        return;
    }
    
    if (!isTask2Mode) {
        cout << "=== Задание 2: АТД Множество ===" << endl;
        
        if (!filename.empty()) {
            SimpleMap* setMap = createSimpleMap();
            
            ifstream file(filename);
            if (!file.is_open()) {
                cout << "Ошибка: не удалось открыть файл " << filename << endl;
                destroySimpleMap(setMap);
                return;
            }
            
            cout << "Чтение из файла: " << filename << endl << endl;
            
            string line;
            while (getline(file, line)) {
                executeCommand(setMap, line);
            }
            file.close();
            
            if (!query.empty()) {
                cout << "\n--- Выполнение запроса ---" << endl;
                executeCommand(setMap, query);
            }
            
            cout << "\n--- Созданные множества ---" << endl;
            MapNode* current = setMap->head;
            if (!current) {
                cout << "Нет множеств" << endl;
            } else {
                while (current) {
                    cout << "Множество '" << current->key << "': ";
                    printSet(current->value);
                    current = current->next;
                }
            }
            
            destroySimpleMap(setMap);
        } else {
            interactiveSetMode();
        }
    }
}