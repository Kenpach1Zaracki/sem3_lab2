#include <iostream>
#include "zad5.h"
#include "huffman.h"
using namespace std;

void processHuffman() {
    cout << "=== Задание 5: Алгоритм Хаффмана ===" << endl;
    
    string text;
    cout << "Введите строку для кодирования: ";
    getline(cin, text);
    
    if (text.empty()) {
        cout << "Ошибка: пустая строка" << endl;
        return;
    }
    
    cout << "\n--- Исходная строка ---" << endl;
    cout << text << endl;
    cout << "Длина: " << text.length() << " символов" << endl;
    
    // Строим дерево Хаффмана
    HuffmanNode* root = buildHuffmanTree(text);
    if (!root) {
        cout << "Ошибка построения дерева" << endl;
        return;
    }
    
    // Создаем таблицу кодов
    HuffmanTable* table = createHuffmanTable();
    buildCodesFromTree(root, table, "");
    
    // Выводим таблицу кодов
    cout << "\n--- Таблица кодов Хаффмана ---" << endl;
    for (int i = 0; i < table->size; i++) {
        cout << "'" << table->codes[i].symbol << "' -> " 
             << table->codes[i].code << endl;
    }
    
    // Кодируем строку
    string encoded = encodeHuffman(text, table);
    cout << "\n--- Закодированная строка ---" << endl;
    cout << encoded << endl;
    cout << "Длина: " << encoded.length() << " бит" << endl;
    
    int originalBits = text.length() * 8;
    double compression = (1.0 - (double)encoded.length() / originalBits) * 100;
    cout << "Коэффициент сжатия: " << compression << "%" << endl;
    
    // Декодируем строку
    string decoded = decodeHuffman(encoded, root);
    cout << "\n--- Декодированная строка ---" << endl;
    cout << decoded << endl;
    
    if (decoded == text) {
        cout << "Декодирование корректно!" << endl;
    } else {
        cout << "ОШИБКА: декодированная строка не совпадает!" << endl;
    }
    
    // Очистка памяти
    destroyHuffmanTree(root);
    destroyHuffmanTable(table);
}