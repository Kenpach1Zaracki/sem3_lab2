#include <iostream>
#include "zad5.h"
#include "huffman.h"
using namespace std;

void processHuffman() {
    cout << "=== Задание 5: Алгоритм Хаффмана ===" << endl;
    cout << "Выберите режим:" << endl;
    cout << "1. Кодирование строки" << endl;
    cout << "2. Декодирование строки" << endl;
    cout << "3. Кодирование и декодирование (демонстрация)" << endl;
    cout << "Ваш выбор: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
        // Кодирование
        cout << "\nВведите строку для кодирования: ";
        string text;
        getline(cin, text);
        
        if (text.empty()) {
            cout << "Ошибка: Пустая строка" << endl;
            return;
        }
        
        cout << "\n--- Исходная строка ---" << endl;
        cout << text << endl;
        cout << "Длина: " << text.length() << " символов" << endl;
        
        // Строим дерево Хаффмана
        HuffmanNode* root = buildHuffmanTree(text);
        
        // Строим таблицу кодов
        HuffmanTable* table = createHuffmanTable();
        buildCodesFromTree(root, table, "");
        
        // Выводим таблицу
        printHuffmanTable(table);
        
        // Кодируем
        string encoded = encodeHuffman(text, table);
        
        cout << "\n--- Закодированная строка ---" << endl;
        cout << encoded << endl;
        cout << "Длина: " << encoded.length() << " бит" << endl;
        
        // Коэффициент сжатия
        double ratio = (double)encoded.length() / (text.length() * 8) * 100;
        cout << "Коэффициент сжатия: " << ratio << "%" << endl;
        
        // Освобождаем память
        destroyHuffmanTree(root);
        destroyHuffmanTable(table);
    }
    else if (choice == 2) {
        // Декодирование
        cout << "\nВведите исходную строку (для построения дерева): ";
        string originalText;
        getline(cin, originalText);
        
        if (originalText.empty()) {
            cout << "Ошибка: Пустая строка" << endl;
            return;
        }
        
        cout << "Введите закодированную строку (только 0 и 1): ";
        string encoded;
        getline(cin, encoded);
        
        if (encoded.empty()) {
            cout << "Ошибка: Пустая строка" << endl;
            return;
        }
        
        // Проверка на корректность (только 0 и 1)
        for (size_t i = 0; i < encoded.length(); i++) {
            if (encoded[i] != '0' && encoded[i] != '1') {
                cout << "Ошибка: Закодированная строка должна содержать только 0 и 1" << endl;
                return;
            }
        }
        
        // Строим дерево Хаффмана из исходной строки
        HuffmanNode* root = buildHuffmanTree(originalText);
        
        // Декодируем
        string decoded = decodeHuffman(encoded, root);
        
        cout << "\n--- Декодированная строка ---" << endl;
        cout << decoded << endl;
        
        // Освобождаем память
        destroyHuffmanTree(root);
    }
    else if (choice == 3) {
        // Демонстрация
        cout << "\nВведите строку для демонстрации: ";
        string text;
        getline(cin, text);
        
        if (text.empty()) {
            cout << "Ошибка: Пустая строка" << endl;
            return;
        }
        
        cout << "\n=== ДЕМОНСТРАЦИЯ АЛГОРИТМА ХАФФМАНА ===" << endl;
        cout << "\n--- Исходная строка ---" << endl;
        cout << text << endl;
        cout << "Длина: " << text.length() << " символов" << endl;
        cout << "Размер без сжатия: " << (text.length() * 8) << " бит" << endl;
        
        // Строим дерево Хаффмана
        cout << "\n[Шаг 1] Построение дерева Хаффмана..." << endl;
        HuffmanNode* root = buildHuffmanTree(text);
        cout << "✓ Дерево построено" << endl;
        
        // Строим таблицу кодов
        cout << "\n[Шаг 2] Построение таблицы кодов..." << endl;
        HuffmanTable* table = createHuffmanTable();
        buildCodesFromTree(root, table, "");
        printHuffmanTable(table);
        
        // Кодируем
        cout << "\n[Шаг 3] Кодирование..." << endl;
        string encoded = encodeHuffman(text, table);
        cout << "✓ Строка закодирована" << endl;
        cout << "\nЗакодированная строка:" << endl;
        cout << encoded << endl;
        cout << "Длина: " << encoded.length() << " бит" << endl;
        
        // Коэффициент сжатия
        double ratio = (double)encoded.length() / (text.length() * 8) * 100;
        cout << "Коэффициент сжатия: " << ratio << "%" << endl;
        cout << "Экономия: " << (100 - ratio) << "%" << endl;
        
        // Декодируем
        cout << "\n[Шаг 4] Декодирование..." << endl;
        string decoded = decodeHuffman(encoded, root);
        cout << "✓ Строка декодирована" << endl;
        cout << "\nДекодированная строка:" << endl;
        cout << decoded << endl;
        
        // Проверка
        cout << "\n[Шаг 5] Проверка корректности..." << endl;
        if (text == decoded) {
            cout << "✓ Декодирование прошло успешно!" << endl;
            cout << "✓ Исходная и декодированная строки совпадают!" << endl;
        } else {
            cout << "✗ Ошибка: Строки не совпадают!" << endl;
        }
        
        // Освобождаем память
        destroyHuffmanTree(root);
        destroyHuffmanTable(table);
    }
    else {
        cout << "Неверный выбор" << endl;
    }
}
