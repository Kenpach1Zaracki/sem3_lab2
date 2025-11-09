#include <iostream>
#include "huffman.h"
using namespace std;

// Создать таблицу кодов
HuffmanTable* createHuffmanTable() {
    HuffmanTable* table = new HuffmanTable;
    table->capacity = 256;  
    table->size = 0;
    table->codes = new HuffmanCode[table->capacity];
    return table;
}

// Уничтожить таблицу кодов
void destroyHuffmanTable(HuffmanTable* table) {
    if (!table) return;
    delete[] table->codes;
    delete table;
}

// Добавить код в таблицу
void addHuffmanCode(HuffmanTable* table, char symbol, const string& code) {
    if (!table) return;
    
    // Проверяем, есть ли уже такой символ
    for (int i = 0; i < table->size; i++) {
        if (table->codes[i].symbol == symbol) {
            table->codes[i].code = code;
            return;
        }
    }
    
    // Добавляем новый код
    if (table->size < table->capacity) {
        table->codes[table->size].symbol = symbol;
        table->codes[table->size].code = code;
        table->size++;
    }
}

// Найти код символа
string findHuffmanCode(HuffmanTable* table, char symbol) {
    if (!table) return "";
    
    for (int i = 0; i < table->size; i++) {
        if (table->codes[i].symbol == symbol) {
            return table->codes[i].code;
        }
    }
    return "";
}

// Подсчитать частоты символов
void countFrequencies(const string& text, int* frequencies) {
    for (int i = 0; i < 256; i++) {
        frequencies[i] = 0;
    }
    
    for (size_t i = 0; i < text.length(); i++) {
        frequencies[(unsigned char)text[i]]++;
    }
}

// Найти узел с минимальной частотой (и удалить его из массива)
HuffmanNode* extractMin(HuffmanNode** nodes, int& count) {
    if (count == 0) return nullptr;
    
    int minIndex = 0;
    for (int i = 1; i < count; i++) {
        if (nodes[i]->frequency < nodes[minIndex]->frequency) {
            minIndex = i;
        }
    }
    
    HuffmanNode* minNode = nodes[minIndex];
    
    // Удаляем узел из массива (сдвигаем элементы)
    for (int i = minIndex; i < count - 1; i++) {
        nodes[i] = nodes[i + 1];
    }
    count--;
    
    return minNode;
}

// Построить дерево Хаффмана
HuffmanNode* buildHuffmanTree(const string& text) {
    if (text.empty()) return nullptr;
    
    // Подсчитываем частоты
    int frequencies[256];
    countFrequencies(text, frequencies);
    
    // Создаем массив узлов для уникальных символов
    HuffmanNode** nodes = new HuffmanNode*[256];
    int nodeCount = 0;
    
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            nodes[nodeCount++] = new HuffmanNode((char)i, frequencies[i]);
        }
    }
    
    // Особый случай: только один уникальный символ
    if (nodeCount == 1) {
        HuffmanNode* root = new HuffmanNode('\0', nodes[0]->frequency);
        root->left = nodes[0];
        root->right = nullptr;
        delete[] nodes;
        return root;
    }
    
    // Построение дерева
    while (nodeCount > 1) {
        // Извлекаем два узла с минимальной частотой
        HuffmanNode* left = extractMin(nodes, nodeCount);
        HuffmanNode* right = extractMin(nodes, nodeCount);
        
        // Создаем родителя
        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        // Добавляем родителя обратно в массив
        nodes[nodeCount++] = parent;
    }
    
    HuffmanNode* root = nodes[0];
    delete[] nodes;
    
    return root;
}

// Построить таблицу кодов из дерева (рекурсивно)
void buildCodesFromTree(HuffmanNode* root, HuffmanTable* table, const string& currentCode) {
    if (!root) return;
    
    // Если это лист (символ)
    if (!root->left && !root->right && root->symbol != '\0') {
        addHuffmanCode(table, root->symbol, currentCode);
        return;
    }
    
    // Рекурсивно обходим левое и правое поддеревья
    if (root->left) {
        buildCodesFromTree(root->left, table, currentCode + "0");
    }
    if (root->right) {
        buildCodesFromTree(root->right, table, currentCode + "1");
    }
}

// Закодировать строку
string encodeHuffman(const string& text, HuffmanTable* table) {
    if (!table || text.empty()) return "";
    
    string encoded = "";
    for (size_t i = 0; i < text.length(); i++) {
        string code = findHuffmanCode(table, text[i]);
        encoded += code;
    }
    
    return encoded;
}

// Декодировать строку
string decodeHuffman(const string& encoded, HuffmanNode* root) {
    if (!root || encoded.empty()) return "";
    
    string decoded = "";
    HuffmanNode* current = root;
    
    // Особый случай: только один символ в дереве
    if (!root->left && !root->right) {
        for (size_t i = 0; i < encoded.length(); i++) {
            decoded += root->symbol;
        }
        return decoded;
    }
    
    for (size_t i = 0; i < encoded.length(); i++) {
        if (encoded[i] == '0') {
            current = current->left;
        } else if (encoded[i] == '1') {
            current = current->right;
        }
        
        // Если достигли листа
        if (current && !current->left && !current->right) {
            decoded += current->symbol;
            current = root;  // Возвращаемся к корню
        }
    }
    
    return decoded;
}

// Вывести таблицу кодов
void printHuffmanTable(HuffmanTable* table) {
    if (!table) return;
    
    cout << "\n--- Таблица кодов Хаффмана ---" << endl;
    for (int i = 0; i < table->size; i++) {
        cout << "'" << table->codes[i].symbol << "' → " 
             << table->codes[i].code << endl;
    }
}

// Уничтожить дерево Хаффмана (рекурсивно)
void destroyHuffmanTree(HuffmanNode* root) {
    if (!root) return;
    
    destroyHuffmanTree(root->left);
    destroyHuffmanTree(root->right);
    delete root;
}
