#include <iostream>
#include <ctime>
#include <cstdlib>
#include "zad6.h"
using namespace std;

// Хеш-функция методом свертки
int hashFoldingMethod(const string& key, int capacity) {
    if (capacity <= 0) return 0;
    
    // Проверяем, является ли ключ числом
    bool isNumber = true;
    for (size_t i = 0; i < key.length(); i++) {
        if (!isdigit(key[i])) {
            isNumber = false;
            break;
        }
    }
    
    if (isNumber && key.length() > 3) {
        // Применяем метод свертки
        long long sum = 0;
        
        // Разбиваем на части по 3 цифры
        for (size_t i = 0; i < key.length(); i += 3) {
            size_t end = (i + 3 < key.length()) ? i + 3 : key.length();
            string part = key.substr(i, end - i);
            sum += stoll(part);
        }
        
        return sum % capacity;
    } else {
        // Для не-числовых ключей используем стандартный метод
        unsigned long hash = 5381;
        for (size_t i = 0; i < key.length(); i++) {
            hash = ((hash << 5) + hash) + key[i];
        }
        return hash % capacity;
    }
}

// Простая хеш-функция
int hashSimple(const string& key, int capacity) {
    if (capacity <= 0) return 0;
    
    unsigned long hash = 0;
    for (size_t i = 0; i < key.length(); i++) {
        hash = hash * 31 + key[i];
    }
    return hash % capacity;
}

//ОТКРЫТАЯ АДРЕСАЦИЯ

HashTableOpen* createHashTableOpen(int capacity) {
    HashTableOpen* table = new HashTableOpen;
    table->capacity = capacity;
    table->size = 0;
    table->table = new HashNode[capacity];
    
    // Инициализация
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
    
    // Создаем новую таблицу в 2 раза больше
    table->capacity = oldCapacity * 2;
    table->size = 0;
    table->table = new HashNode[table->capacity];
    
    // Инициализация новой таблицы
    for (int i = 0; i < table->capacity; i++) {
        table->table[i].isEmpty = true;
        table->table[i].isDeleted = false;
    }
    
    // Перехешируем все элементы
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
    
    // Проверка на реструктуризацию (90%)
    if (getLoadFactorOpen(table) >= 0.9) {
        cout << "[Реструктуризация] Коэффициент заполнения >= 90%, увеличиваем таблицу..." << endl;
        rehashHashTableOpen(table);
    }
    
    int hash = hashFoldingMethod(key, table->capacity);
    int index = hash;
    
    // Линейное пробирование
    for (int i = 0; i < table->capacity; i++) {
        index = (hash + i) % table->capacity;
        
        // Нашли пустую или удаленную ячейку
        if (table->table[index].isEmpty || table->table[index].isDeleted) {
            table->table[index].key = key;
            table->table[index].value = value;
            table->table[index].isEmpty = false;
            table->table[index].isDeleted = false;
            table->size++;
            return true;
        }
        
        // Обновление существующего ключа
        if (table->table[index].key == key) {
            table->table[index].value = value;
            return true;
        }
    }
    
    return false;  // Таблица заполнена
}

int searchHashTableOpen(HashTableOpen* table, const string& key) {
    if (!table || key.empty()) return -1;
    
    int hash = hashFoldingMethod(key, table->capacity);
    int index = hash;
    
    // Линейное пробирование
    for (int i = 0; i < table->capacity; i++) {
        index = (hash + i) % table->capacity;
        
        // Пустая ячейка - элемента нет
        if (table->table[index].isEmpty && !table->table[index].isDeleted) {
            return -1;
        }
        
        // Нашли элемент
        if (!table->table[index].isEmpty && !table->table[index].isDeleted &&
            table->table[index].key == key) {
            return table->table[index].value;
        }
    }
    
    return -1;
}

bool deleteHashTableOpen(HashTableOpen* table, const string& key) {
    if (!table || key.empty()) return false;
    
    int hash = hashFoldingMethod(key, table->capacity);
    int index = hash;
    
    // Линейное пробирование
    for (int i = 0; i < table->capacity; i++) {
        index = (hash + i) % table->capacity;
        
        // Пустая ячейка - элемента нет
        if (table->table[index].isEmpty && !table->table[index].isDeleted) {
            return false;
        }
        
        // Нашли элемент
        if (!table->table[index].isEmpty && !table->table[index].isDeleted &&
            table->table[index].key == key) {
            table->table[index].isDeleted = true;
            table->size--;
            return true;
        }
    }
    
    return false;
}

void printHashTableOpen(HashTableOpen* table) {
    if (!table) return;
    
    cout << "\n=== Хеш-таблица (Открытая адресация) ===" << endl;
    cout << "Размер: " << table->size << " / " << table->capacity << endl;
    cout << "Коэффициент заполнения: " << (getLoadFactorOpen(table) * 100) << "%" << endl;
    cout << "\nСодержимое:" << endl;
    
    for (int i = 0; i < table->capacity; i++) {
        if (!table->table[i].isEmpty && !table->table[i].isDeleted) {
            cout << "[" << i << "] '" << table->table[i].key << "' → " 
                 << table->table[i].value << endl;
        }
    }
}

//МЕТОД ЦЕПОЧЕК

HashTableChain* createHashTableChain(int capacity) {
    HashTableChain* table = new HashTableChain;
    table->capacity = capacity;
    table->size = 0;
    table->table = new ChainNode*[capacity];
    
    // Инициализация
    for (int i = 0; i < capacity; i++) {
        table->table[i] = nullptr;
    }
    
    return table;
}

void destroyHashTableChain(HashTableChain* table) {
    if (!table) return;
    
    // Удаляем все цепочки
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
    
    // Создаем новую таблицу в 2 раза больше
    table->capacity = oldCapacity * 2;
    table->size = 0;
    table->table = new ChainNode*[table->capacity];
    
    // Инициализация новой таблицы
    for (int i = 0; i < table->capacity; i++) {
        table->table[i] = nullptr;
    }
    
    // Перехешируем все элементы
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
    
    // Проверка на реструктуризацию (90%)
    if (getLoadFactorChain(table) >= 0.9) {
        cout << "[Реструктуризация] Коэффициент заполнения >= 90%, увеличиваем таблицу..." << endl;
        rehashHashTableChain(table);
    }
    
    int index = hashFoldingMethod(key, table->capacity);
    
    // Проверяем, существует ли ключ
    ChainNode* current = table->table[index];
    while (current) {
        if (current->key == key) {
            current->value = value;  // Обновляем значение
            return true;
        }
        current = current->next;
    }
    
    // Добавляем новый элемент в начало цепочки
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
    
    int index = hashFoldingMethod(key, table->capacity);
    
    ChainNode* current = table->table[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return -1;
}

bool deleteHashTableChain(HashTableChain* table, const string& key) {
    if (!table || key.empty()) return false;
    
    int index = hashFoldingMethod(key, table->capacity);
    
    ChainNode* current = table->table[index];
    ChainNode* prev = nullptr;
    
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table->table[index] = current->next;
            }
            delete current;
            table->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

void printHashTableChain(HashTableChain* table) {
    if (!table) return;
    
    cout << "\n=== Хеш-таблица (Метод цепочек) ===" << endl;
    cout << "Размер: " << table->size << " / " << table->capacity << endl;
    cout << "Коэффициент заполнения: " << (getLoadFactorChain(table) * 100) << "%" << endl;
    cout << "\nСодержимое:" << endl;
    
    for (int i = 0; i < table->capacity; i++) {
        if (table->table[i]) {
            cout << "[" << i << "] → ";
            ChainNode* current = table->table[i];
            bool first = true;
            while (current) {
                if (!first) cout << " → ";
                cout << "'" << current->key << "':" << current->value;
                current = current->next;
                first = false;
            }
            cout << endl;
        }
    }
}
//ЭМПИРИЧЕСКИЙ АНАЛИЗ

void empiricalAnalysis() {
    cout << "\n=== ЭМПИРИЧЕСКИЙ АНАЛИЗ ХЕШ-ТАБЛИЦ ===" << endl;
    
    int N, M;
    cout << "Введите количество элементов для вставки (N): ";
    cin >> N;
    
    if (cin.fail() || N <= 0 || N > 100000) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: Неверное значение N" << endl;
        return;
    }
    
    cout << "Введите количество поисков (M): ";
    cin >> M;
    
    if (cin.fail() || M <= 0 || M > 100000) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: Неверное значение M" << endl;
        return;
    }
    
    cout << "\n--- Параметры теста ---" << endl;
    cout << "Количество элементов (N): " << N << endl;
    cout << "Количество поисков (M): " << M << endl;
    
    // Начальный размер таблиц
    int initialCapacity = N / 2;  // Начнем с половины, чтобы увидеть реструктуризацию
    
    cout << "Начальная емкость таблиц: " << initialCapacity << endl;
    
    // Создаем таблицы
    HashTableOpen* tableOpen = createHashTableOpen(initialCapacity);
    HashTableChain* tableChain = createHashTableChain(initialCapacity);
    
    // Генерируем случайные ключи
    cout << "\n[1] Генерация случайных ключей..." << endl;
    string* keys = new string[N];
    srand(time(nullptr));
    
    for (int i = 0; i < N; i++) {
        // Генерируем число из 9 цифр для демонстрации метода свертки
        long long num = 100000000LL + (rand() % 900000000LL);
        keys[i] = to_string(num);
    }
    cout << "✓ Ключи сгенерированы" << endl;
    
    // Тест 1: Вставка в открытую адресацию
    cout << "\n[2] Вставка в хеш-таблицу (Открытая адресация)..." << endl;
    clock_t start = clock();
    
    for (int i = 0; i < N; i++) {
        insertHashTableOpen(tableOpen, keys[i], i);
    }
    
    clock_t end = clock();
    double timeOpen = (double)(end - start) / CLOCKS_PER_SEC * 1000;  // в миллисекундах
    
    cout << "✓ Вставка завершена" << endl;
    cout << "Время вставки: " << timeOpen << " мс" << endl;
    cout << "Финальный размер таблицы: " << tableOpen->size << " / " << tableOpen->capacity << endl;
    cout << "Коэффициент заполнения: " << (getLoadFactorOpen(tableOpen) * 100) << "%" << endl;
    
    // Тест 2: Вставка в метод цепочек
    cout << "\n[3] Вставка в хеш-таблицу (Метод цепочек)..." << endl;
    start = clock();
    
    for (int i = 0; i < N; i++) {
        insertHashTableChain(tableChain, keys[i], i);
    }
    
    end = clock();
    double timeChain = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "✓ Вставка завершена" << endl;
    cout << "Время вставки: " << timeChain << " мс" << endl;
    cout << "Финальный размер таблицы: " << tableChain->size << " / " << tableChain->capacity << endl;
    cout << "Коэффициент заполнения: " << (getLoadFactorChain(tableChain) * 100) << "%" << endl;
    
    // Тест 3: Поиск в открытой адресации
    cout << "\n[4] Выполнение " << M << " поисков (Открытая адресация)..." << endl;
    start = clock();
    
    int foundOpen = 0;
    for (int i = 0; i < M; i++) {
        int randomIndex = rand() % N;
        int result = searchHashTableOpen(tableOpen, keys[randomIndex]);
        if (result != -1) foundOpen++;
    }
    
    end = clock();
    double searchTimeOpen = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "✓ Поиск завершен" << endl;
    cout << "Найдено элементов: " << foundOpen << " из " << M << endl;
    cout << "Время поиска: " << searchTimeOpen << " мс" << endl;
    cout << "Среднее время на 1 поиск: " << (searchTimeOpen / M) << " мс" << endl;
    
    // Тест 4: Поиск в методе цепочек
    cout << "\n[5] Выполнение " << M << " поисков (Метод цепочек)..." << endl;
    start = clock();
    
    int foundChain = 0;
    for (int i = 0; i < M; i++) {
        int randomIndex = rand() % N;
        int result = searchHashTableChain(tableChain, keys[randomIndex]);
        if (result != -1) foundChain++;
    }
    
    end = clock();
    double searchTimeChain = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "✓ Поиск завершен" << endl;
    cout << "Найдено элементов: " << foundChain << " из " << M << endl;
    cout << "Время поиска: " << searchTimeChain << " мс" << endl;
    cout << "Среднее время на 1 поиск: " << (searchTimeChain / M) << " мс" << endl;
    
    // Итоговое сравнение
    cout << "\n=== ИТОГОВОЕ СРАВНЕНИЕ ===" << endl;
    cout << "\n+------------------------+------------------+------------------+" << endl;
    cout << "| Операция               | Открытая адрес.  | Метод цепочек    |" << endl;
    cout << "+------------------------+------------------+------------------+" << endl;
    
    printf("| Вставка %d элементов   | %12.3f мс | %12.3f мс |\n", N, timeOpen, timeChain);
    printf("| Поиск %d раз           | %12.3f мс | %12.3f мс |\n", M, searchTimeOpen, searchTimeChain);
    printf("| Среднее время поиска   | %12.6f мс | %12.6f мс |\n", 
           searchTimeOpen/M, searchTimeChain/M);
    
    cout << "+------------------------+------------------+------------------+" << endl;
    
    // Анализ
    cout << "\n--- Анализ ---" << endl;
    if (searchTimeOpen < searchTimeChain) {
        cout << "• Открытая адресация быстрее на поиске на " 
             << ((searchTimeChain - searchTimeOpen) / searchTimeChain * 100) << "%" << endl;
    } else {
        cout << "• Метод цепочек быстрее на поиске на " 
             << ((searchTimeOpen - searchTimeChain) / searchTimeOpen * 100) << "%" << endl;
    }
    
    cout << "• Открытая адресация лучше при низком коэффициенте заполнения" << endl;
    cout << "• Метод цепочек устойчивее к коллизиям при высоком заполнении" << endl;
    
    // Освобождаем память
    delete[] keys;
    destroyHashTableOpen(tableOpen);
    destroyHashTableChain(tableChain);
}

//ДЕМОНСТРАЦИЯ МЕТОДА СВЕРТКИ

void demonstrateFoldingMethod() {
    cout << "\n=== ДЕМОНСТРАЦИЯ МЕТОДА СВЕРТКИ ===" << endl;
    cout << "\nМетод свертки разбивает ключ-число на части и суммирует их." << endl;
    cout << "Пример из ТЗ: 523456795 → 523+456+795 = 1774" << endl;
    
    string key;
    cout << "\nВведите число (ключ): ";
    cin >> key;
    cin.ignore();
    
    // Проверка на число
    bool isNumber = true;
    for (size_t i = 0; i < key.length(); i++) {
        if (!isdigit(key[i])) {
            isNumber = false;
            break;
        }
    }
    
    if (!isNumber) {
        cout << "Ошибка: Ключ должен быть числом" << endl;
        return;
    }
    
    cout << "\n--- Процесс свертки ---" << endl;
    cout << "Исходный ключ: " << key << endl;
    cout << "Разбиение на части по 3 цифры:" << endl;
    
    long long sum = 0;
    for (size_t i = 0; i < key.length(); i += 3) {
        size_t end = (i + 3 < key.length()) ? i + 3 : key.length();
        string part = key.substr(i, end - i);
        long long partValue = stoll(part);
        sum += partValue;
        
        cout << "  Часть " << ((i/3)+1) << ": " << part << " = " << partValue << endl;
    }
    
    cout << "\nСумма частей: " << sum << endl;
    
    int capacity;
    cout << "\nВведите размер таблицы для взятия модуля: ";
    cin >> capacity;
    
    if (cin.fail() || capacity <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: Неверный размер таблицы" << endl;
        return;
    }
    
    int hash = sum % capacity;
    cout << "\nИтоговый хеш: " << sum << " mod " << capacity << " = " << hash << endl;
    cout << "Элемент будет размещен в ячейке [" << hash << "]" << endl;
}

//ИНТЕРАКТИВНЫЙ РЕЖИМ 

void interactiveMode() {
    cout << "\n=== ИНТЕРАКТИВНЫЙ РЕЖИМ ===" << endl;
    cout << "Выберите тип хеш-таблицы:" << endl;
    cout << "1. Открытая адресация" << endl;
    cout << "2. Метод цепочек" << endl;
    cout << "Ваш выбор: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice != 1 && choice != 2) {
        cout << "Неверный выбор" << endl;
        return;
    }
    
    int capacity;
    cout << "Введите начальный размер таблицы: ";
    cin >> capacity;
    cin.ignore();
    
    if (capacity <= 0) {
        cout << "Ошибка: Размер должен быть положительным" << endl;
        return;
    }
    
    HashTableOpen* tableOpen = nullptr;
    HashTableChain* tableChain = nullptr;
    
    if (choice == 1) {
        tableOpen = createHashTableOpen(capacity);
        cout << "\n✓ Создана хеш-таблица с открытой адресацией" << endl;
    } else {
        tableChain = createHashTableChain(capacity);
        cout << "\n✓ Создана хеш-таблица с методом цепочек" << endl;
    }
    
    string command;
    while (true) {
        cout << "\n--- Команды ---" << endl;
        cout << "INSERT <key> <value> - вставить элемент" << endl;
        cout << "SEARCH <key> - найти элемент" << endl;
        cout << "DELETE <key> - удалить элемент" << endl;
        cout << "PRINT - вывести таблицу" << endl;
        cout << "EXIT - выход" << endl;
        cout << "\n> ";
        
        getline(cin, command);
        
        if (command.empty()) continue;
        
        // Парсим команду
        size_t space1 = command.find(' ');
        string cmd = command.substr(0, space1);
        
        if (cmd == "EXIT" || cmd == "exit") {
            break;
        }
        else if (cmd == "INSERT") {
            if (space1 == string::npos) {
                cout << "Ошибка: Требуется ключ и значение" << endl;
                continue;
            }
            
            size_t space2 = command.find(' ', space1 + 1);
            if (space2 == string::npos) {
                cout << "Ошибка: Требуется значение" << endl;
                continue;
            }
            
            string key = command.substr(space1 + 1, space2 - space1 - 1);
            int value = stoi(command.substr(space2 + 1));
            
            if (choice == 1) {
                if (insertHashTableOpen(tableOpen, key, value)) {
                    cout << "✓ Элемент '" << key << "' вставлен" << endl;
                } else {
                    cout << "✗ Ошибка вставки" << endl;
                }
            } else {
                if (insertHashTableChain(tableChain, key, value)) {
                    cout << "✓ Элемент '" << key << "' вставлен" << endl;
                } else {
                    cout << "✗ Ошибка вставки" << endl;
                }
            }
        }
        else if (cmd == "SEARCH") {
            if (space1 == string::npos) {
                cout << "Ошибка: Требуется ключ" << endl;
                continue;
            }
            
            string key = command.substr(space1 + 1);
            
            int result;
            if (choice == 1) {
                result = searchHashTableOpen(tableOpen, key);
            } else {
                result = searchHashTableChain(tableChain, key);
            }
            
            if (result != -1) {
                cout << "✓ Найдено: '" << key << "' → " << result << endl;
            } else {
                cout << "✗ Элемент '" << key << "' не найден" << endl;
            }
        }
        else if (cmd == "DELETE") {
            if (space1 == string::npos) {
                cout << "Ошибка: Требуется ключ" << endl;
                continue;
            }
            
            string key = command.substr(space1 + 1);
            
            bool success;
            if (choice == 1) {
                success = deleteHashTableOpen(tableOpen, key);
            } else {
                success = deleteHashTableChain(tableChain, key);
            }
            
            if (success) {
                cout << "✓ Элемент '" << key << "' удален" << endl;
            } else {
                cout << "✗ Элемент '" << key << "' не найден" << endl;
            }
        }
        else if (cmd == "PRINT") {
            if (choice == 1) {
                printHashTableOpen(tableOpen);
            } else {
                printHashTableChain(tableChain);
            }
        }
        else {
            cout << "Неизвестная команда: " << cmd << endl;
        }
    }
    
    // Освобождаем память
    if (tableOpen) destroyHashTableOpen(tableOpen);
    if (tableChain) destroyHashTableChain(tableChain);
}

//ГЛАВНАЯ ФУНКЦИЯ 

void processHashTables() {
    cout << "=== Задание 6: Хеш-таблицы ===" << endl;
    cout << "\nРеализовано:" << endl;
    cout << "• Хеш-таблица с открытой адресацией (линейное пробирование)" << endl;
    cout << "• Хеш-таблица с методом цепочек" << endl;
    cout << "• Хеш-функция методом свертки" << endl;
    cout << "• Реструктуризация при заполнении 90%" << endl;
    
    cout << "\nВыберите режим:" << endl;
    cout << "1. Демонстрация метода свертки" << endl;
    cout << "2. Интерактивный режим" << endl;
    cout << "3. Эмпирический анализ" << endl;
    cout << "Ваш выбор: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch (choice) {
        case 1:
            demonstrateFoldingMethod();
            break;
        case 2:
            interactiveMode();
            break;
        case 3:
            empiricalAnalysis();
            break;
        default:
            cout << "Неверный выбор" << endl;
    }
}
