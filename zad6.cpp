#include <iostream>
#include <ctime>
#include <cstdlib>
#include "zad6.h"
using namespace std;

// Хеш-функция методом свертки
int hashFoldingMethod(const string& key, int capacity) {
    if (capacity <= 0) return 0;
    
    bool isNumber = true;
    for (size_t i = 0; i < key.length(); i++) {
        if (!isdigit(key[i])) {
            isNumber = false;
            break;
        }
    }
    
    if (isNumber && key.length() > 3) {
        long long sum = 0;
        
        for (size_t i = 0; i < key.length(); i += 3) {
            size_t end = (i + 3 < key.length()) ? i + 3 : key.length();
            string part = key.substr(i, end - i);
            sum += stoll(part);
        }
        
        return sum % capacity;
    } else {
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

// ОТКРЫТАЯ АДРЕСАЦИЯ

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
        cout << "Реструктуризация: коэффициент заполнения >= 90%" << endl;
        rehashHashTableOpen(table);
    }
    
    int hash = hashFoldingMethod(key, table->capacity);
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
    
    int hash = hashFoldingMethod(key, table->capacity);
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

bool deleteHashTableOpen(HashTableOpen* table, const string& key) {
    if (!table || key.empty()) return false;
    
    int hash = hashFoldingMethod(key, table->capacity);
    int index = hash;
    
    for (int i = 0; i < table->capacity; i++) {
        index = (hash + i) % table->capacity;
        
        if (table->table[index].isEmpty && !table->table[index].isDeleted) {
            return false;
        }
        
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
    
    cout << "\nХеш-таблица (открытая адресация)" << endl;
    cout << "Размер: " << table->size << " / " << table->capacity << endl;
    cout << "Коэффициент заполнения: " << (getLoadFactorOpen(table) * 100) << "%" << endl;
    
    for (int i = 0; i < table->capacity; i++) {
        if (!table->table[i].isEmpty && !table->table[i].isDeleted) {
            cout << "[" << i << "] " << table->table[i].key << " -> " 
                 << table->table[i].value << endl;
        }
    }
}

// МЕТОД ЦЕПОЧЕК

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
        cout << "Реструктуризация: коэффициент заполнения >= 90%" << endl;
        rehashHashTableChain(table);
    }
    
    int index = hashFoldingMethod(key, table->capacity);
    
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
    
    cout << "\nХеш-таблица (метод цепочек)" << endl;
    cout << "Размер: " << table->size << " / " << table->capacity << endl;
    cout << "Коэффициент заполнения: " << (getLoadFactorChain(table) * 100) << "%" << endl;
    
    for (int i = 0; i < table->capacity; i++) {
        if (table->table[i]) {
            cout << "[" << i << "] -> ";
            ChainNode* current = table->table[i];
            while (current) {
                cout << current->key << ":" << current->value;
                if (current->next) cout << " -> ";
                current = current->next;
            }
            cout << endl;
        }
    }
}

// ЭМПИРИЧЕСКИЙ АНАЛИЗ

void empiricalAnalysis() {
    cout << "\n=== Эмпирический анализ хеш-таблиц ===" << endl;
    
    int N, M;
    cout << "Введите количество элементов для вставки (N): ";
    cin >> N;
    
    if (cin.fail() || N <= 0 || N > 100000) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: неверное значение N" << endl;
        return;
    }
    
    cout << "Введите количество поисков (M): ";
    cin >> M;
    
    if (cin.fail() || M <= 0 || M > 100000) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: неверное значение M" << endl;
        return;
    }
    
    cout << "\nПараметры теста:" << endl;
    cout << "Количество элементов: " << N << endl;
    cout << "Количество поисков: " << M << endl;
    
    int initialCapacity = N / 2;
    cout << "Начальная емкость таблиц: " << initialCapacity << endl;
    
    HashTableOpen* tableOpen = createHashTableOpen(initialCapacity);
    HashTableChain* tableChain = createHashTableChain(initialCapacity);
    
    cout << "\nГенерация случайных ключей..." << endl;
    string* keys = new string[N];
    srand(time(nullptr));
    
    for (int i = 0; i < N; i++) {
        long long num = 100000000LL + (rand() % 900000000LL);
        keys[i] = to_string(num);
    }
    
    // Тест открытой адресации
    cout << "\nВставка в открытую адресацию..." << endl;
    clock_t start = clock();
    
    for (int i = 0; i < N; i++) {
        insertHashTableOpen(tableOpen, keys[i], i);
    }
    
    clock_t end = clock();
    double timeOpen = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "Время вставки: " << timeOpen << " мс" << endl;
    cout << "Размер таблицы: " << tableOpen->size << " / " << tableOpen->capacity << endl;
    
    // Тест метода цепочек
    cout << "\nВставка в метод цепочек..." << endl;
    start = clock();
    
    for (int i = 0; i < N; i++) {
        insertHashTableChain(tableChain, keys[i], i);
    }
    
    end = clock();
    double timeChain = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "Время вставки: " << timeChain << " мс" << endl;
    cout << "Размер таблицы: " << tableChain->size << " / " << tableChain->capacity << endl;
    
    // Тест поиска в открытой адресации
    cout << "\nПоиск в открытой адресации (" << M << " операций)..." << endl;
    start = clock();
    
    int foundOpen = 0;
    for (int i = 0; i < M; i++) {
        int randomIndex = rand() % N;
        int result = searchHashTableOpen(tableOpen, keys[randomIndex]);
        if (result != -1) foundOpen++;
    }
    
    end = clock();
    double searchTimeOpen = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "Найдено элементов: " << foundOpen << " из " << M << endl;
    cout << "Время поиска: " << searchTimeOpen << " мс" << endl;
    
    // Тест поиска в методе цепочек
    cout << "\nПоиск в методе цепочек (" << M << " операций)..." << endl;
    start = clock();
    
    int foundChain = 0;
    for (int i = 0; i < M; i++) {
        int randomIndex = rand() % N;
        int result = searchHashTableChain(tableChain, keys[randomIndex]);
        if (result != -1) foundChain++;
    }
    
    end = clock();
    double searchTimeChain = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "Найдено элементов: " << foundChain << " из " << M << endl;
    cout << "Время поиска: " << searchTimeChain << " мс" << endl;
    
    // Итоги
    cout << "\n=== Итоговое сравнение ===" << endl;
    cout << "Операция: Вставка " << N << " элементов" << endl;
    cout << "  Открытая адресация: " << timeOpen << " мс" << endl;
    cout << "  Метод цепочек: " << timeChain << " мс" << endl;
    cout << "Операция: Поиск " << M << " раз" << endl;
    cout << "  Открытая адресация: " << searchTimeOpen << " мс" << endl;
    cout << "  Метод цепочек: " << searchTimeChain << " мс" << endl;
    
    delete[] keys;
    destroyHashTableOpen(tableOpen);
    destroyHashTableChain(tableChain);
}

// ДЕМОНСТРАЦИЯ МЕТОДА СВЕРТКИ

void demonstrateFoldingMethod() {
    cout << "\n=== Демонстрация метода свертки ===" << endl;
    cout << "Метод свертки разбивает ключ-число на части и суммирует их" << endl;
    cout << "Пример: 523456795 -> 523+456+795 = 1774" << endl;
    
    string key;
    cout << "\nВведите число (ключ): ";
    cin >> key;
    cin.ignore();
    
    bool isNumber = true;
    for (size_t i = 0; i < key.length(); i++) {
        if (!isdigit(key[i])) {
            isNumber = false;
            break;
        }
    }
    
    if (!isNumber) {
        cout << "Ошибка: ключ должен быть числом" << endl;
        return;
    }
    
    cout << "\nИсходный ключ: " << key << endl;
    cout << "Разбиение на части по 3 цифры:" << endl;
    
    long long sum = 0;
    int partNum = 1;
    for (size_t i = 0; i < key.length(); i += 3) {
        size_t end = (i + 3 < key.length()) ? i + 3 : key.length();
        string part = key.substr(i, end - i);
        long long partValue = stoll(part);
        sum += partValue;
        
        cout << "  Часть " << partNum << ": " << part << " = " << partValue << endl;
        partNum++;
    }
    
    cout << "\nСумма частей: " << sum << endl;
    
    int capacity;
    cout << "\nВведите размер таблицы для взятия модуля: ";
    cin >> capacity;
    
    if (cin.fail() || capacity <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: неверный размер таблицы" << endl;
        return;
    }
    
    int hash = sum % capacity;
    cout << "\nИтоговый хеш: " << sum << " mod " << capacity << " = " << hash << endl;
    cout << "Элемент будет размещен в ячейке [" << hash << "]" << endl;
}

// ГЛАВНАЯ ФУНКЦИЯ

void processHashTables() {
    cout << "=== Задание 6: Хеш-таблицы ===" << endl;
    cout << "\nРеализовано:" << endl;
    cout << "- Хеш-таблица с открытой адресацией" << endl;
    cout << "- Хеш-таблица с методом цепочек" << endl;
    cout << "- Хеш-функция методом свертки" << endl;
    cout << "- Реструктуризация при заполнении 90%" << endl;
    
    cout << "\nВыберите режим:" << endl;
    cout << "1. Демонстрация метода свертки" << endl;
    cout << "2. Эмпирический анализ" << endl;
    cout << "Ваш выбор: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch (choice) {
        case 1:
            demonstrateFoldingMethod();
            break;
        case 2:
            empiricalAnalysis();
            break;
        default:
            cout << "Неверный выбор" << endl;
    }
}