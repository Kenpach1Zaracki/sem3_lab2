#include <iostream>
#include <fstream>
#include <sstream>
#include "set.h"
#include "zad2.h"
#include "array.h"
#include "map_simple.h"
using namespace std;

SimpleMap* allSets = nullptr;
string dataFilePath;

// Чтение данных из файла
bool readDataFromFile() {
    if (dataFilePath.empty()) return false;
    
    ifstream file(dataFilePath);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл: " << dataFilePath << endl;
        return false;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        string command, setId;
        iss >> command >> setId;
        
        if (command == "SET") {
            if (!simpleMapExists(allSets, setId)) {
                simpleMapInsert(allSets, setId, createSet());
            }
            
            string element;
            while (iss >> element) {
                Set* set = simpleMapFind(allSets, setId);
                setAdd(set, element);
            }
        }
    }
    
    file.close();
    return true;
}

// Запись данных в файл
bool writeDataToFile() {
    if (dataFilePath.empty()) return false;
    
    ofstream file(dataFilePath);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл для записи: " << dataFilePath << endl;
        return false;
    }
    
    // Получаем все ключи
    string* keys = new string[allSets->size + 1];
    int keyCount = 0;
    simpleMapGetKeys(allSets, keys, keyCount);
    
    for (int i = 0; i < keyCount; i++) {
        const string& setId = keys[i];
        Set* set = simpleMapFind(allSets, setId);
        
        int maxSize = setGetSize(set);
        string* elements = new string[maxSize + 1];
        int count = 0;
        setGetElements(set, elements, count);
        
        file << "SET " << setId;
        for (int j = 0; j < count; j++) {
            file << " " << elements[j];
        }
        file << endl;
        
        delete[] elements;
    }
    
    delete[] keys;
    file.close();
    return true;
}

// Выполнение команды
bool executeCommand(Array* tokens) {
    if (!tokens || array_length(tokens) == 0) return false;
    
    string command = array_get(tokens, 0);
    
    // SETADD <set_id> <element>
    if (command == "SETADD") {
        if (array_length(tokens) < 3) {
            cerr << "Ошибка: Требуется имя множества и элемент" << endl;
            return false;
        }
        
        string setId = array_get(tokens, 1);
        string element = array_get(tokens, 2);
        
        if (!simpleMapExists(allSets, setId)) {
            simpleMapInsert(allSets, setId, createSet());
        }
        
        Set* set = simpleMapFind(allSets, setId);
        bool success = setAdd(set, element);
        if (success) {
            cout << "Элемент '" << element << "' добавлен в множество '" << setId << "'" << endl;
        } else {
            cout << "Элемент '" << element << "' уже существует в множестве '" << setId << "'" << endl;
        }
        
        writeDataToFile();
        return success;
    }
    
    // SETDEL <set_id> <element>
    else if (command == "SETDEL") {
        if (array_length(tokens) < 3) {
            cerr << "Ошибка: Требуется имя множества и элемент" << endl;
            return false;
        }
        
        string setId = array_get(tokens, 1);
        string element = array_get(tokens, 2);
        
        if (!simpleMapExists(allSets, setId)) {
            cerr << "Ошибка: Множество '" << setId << "' не найдено" << endl;
            return false;
        }
        
        Set* set = simpleMapFind(allSets, setId);
        bool success = setDelete(set, element);
        if (success) {
            cout << "Элемент '" << element << "' удален из множества '" << setId << "'" << endl;
        } else {
            cout << "Элемент '" << element << "' не найден в множестве '" << setId << "'" << endl;
        }
        
        writeDataToFile();
        return success;
    }
    
    // SET_AT <set_id> <element>
    else if (command == "SET_AT") {
        if (array_length(tokens) < 3) {
            cerr << "Ошибка: Требуется имя множества и элемент" << endl;
            return false;
        }
        
        string setId = array_get(tokens, 1);
        string element = array_get(tokens, 2);
        
        if (!simpleMapExists(allSets, setId)) {
            cout << "Множество '" << setId << "' не найдено" << endl;
            return false;
        }
        
        Set* set = simpleMapFind(allSets, setId);
        int result = setAt(set, element);
        if (result != -1) {
            cout << "Элемент '" << element << "' найден в множестве '" << setId << "'" << endl;
        } else {
            cout << "Элемент '" << element << "' не найден в множестве '" << setId << "'" << endl;
        }
        
        return (result != -1);
    }
    
    // PRINT [set_id]
    else if (command == "PRINT") {
        if (array_length(tokens) >= 2) {
            string setId = array_get(tokens, 1);
            if (!simpleMapExists(allSets, setId)) {
                cout << "Множество '" << setId << "' не найдено" << endl;
                return false;
            }
            cout << "Множество '" << setId << "': ";
            Set* set = simpleMapFind(allSets, setId);
            printSet(set);
        } else {
            if (simpleMapEmpty(allSets)) {
                cout << "Нет созданных множеств" << endl;
            } else {
                cout << "=== Все множества ===" << endl;
                
                string* keys = new string[allSets->size + 1];
                int keyCount = 0;
                simpleMapGetKeys(allSets, keys, keyCount);
                
                for (int i = 0; i < keyCount; i++) {
                    cout << "Множество '" << keys[i] << "': ";
                    Set* set = simpleMapFind(allSets, keys[i]);
                    printSet(set);
                }
                
                delete[] keys;
            }
        }
        return true;
    }
    
    else {
        cerr << "Неизвестная команда: " << command << endl;
        return false;
    }
}

// Обработка аргументов командной строки
void processSetOperations(int argc, char* argv[]) {
    allSets = createSimpleMap();
    
    Array* queries = create_array("queries");
    bool scriptMode = false;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "--file" && i + 1 < argc) {
            dataFilePath = argv[++i];
            scriptMode = true;
        } else if (arg == "--query" && i + 1 < argc) {
            array_push_back(queries, argv[++i]);
            scriptMode = true;
        }
    }
    
    // Читаем данные из файла
    if (!dataFilePath.empty()) {
        readDataFromFile();
    }
    
    // Выполняем запросы
    if (scriptMode) {
        for (int q = 0; q < array_length(queries); q++) {
            string query = array_get(queries, q);
            
            istringstream iss(query);
            Array* tokens = create_array("tokens");
            string token;
            
            while (iss >> token) {
                array_push_back(tokens, token);
            }
            
            executeCommand(tokens);
            destroy_array(tokens);
        }
        
        // Сохраняем данные
        if (!dataFilePath.empty()) {
            writeDataToFile();
        }
    }
    
    // Освобождаем память
    string* keys = new string[allSets->size + 1];
    int keyCount = 0;
    simpleMapGetKeys(allSets, keys, keyCount);
    
    for (int i = 0; i < keyCount; i++) {
        Set* set = simpleMapFind(allSets, keys[i]);
        destroySet(set);
    }
    
    delete[] keys;
    destroySimpleMap(allSets);
    destroy_array(queries);
}

// Интерактивный режим
void interactiveSetMode() {
    allSets = createSimpleMap();
    
    cout << "=== Задание 2: АТД Множество ===" << endl;
    cout << "Доступные команды:" << endl;
    cout << "  SETADD <set_id> <element> - добавить элемент" << endl;
    cout << "  SETDEL <set_id> <element> - удалить элемент" << endl;
    cout << "  SET_AT <set_id> <element> - проверить наличие элемента" << endl;
    cout << "  PRINT [set_id] - вывести множество (или все множества)" << endl;
    cout << "  EXIT - выход" << endl;
    cout << endl;
    
    string line;
    while (true) {
        cout << "> ";
        getline(cin, line);
        
        if (line.empty()) continue;
        
        istringstream iss(line);
        Array* tokens = create_array("tokens");
        string token;
        
        while (iss >> token) {
            array_push_back(tokens, token);
        }
        
        if (array_length(tokens) == 0) {
            destroy_array(tokens);
            continue;
        }
        
        string firstToken = array_get(tokens, 0);
        if (firstToken == "EXIT" || firstToken == "exit") {
            destroy_array(tokens);
            break;
        }
        
        executeCommand(tokens);
        destroy_array(tokens);
    }
    
    // Освобождаем память
    string* keys = new string[allSets->size + 1];
    int keyCount = 0;
    simpleMapGetKeys(allSets, keys, keyCount);
    
    for (int i = 0; i < keyCount; i++) {
        Set* set = simpleMapFind(allSets, keys[i]);
        destroySet(set);
    }
    
    delete[] keys;
    destroySimpleMap(allSets);
}
