#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <string>
#include "structures.h"

struct HuffmanCode {
    char symbol;
    std::string code;
};

struct HuffmanTable {
    HuffmanCode* codes;
    int size;
    int capacity;
};

HuffmanTable* createHuffmanTable();

void destroyHuffmanTable(HuffmanTable* table);

void addHuffmanCode(HuffmanTable* table, char symbol, const std::string& code);

std::string findHuffmanCode(HuffmanTable* table, char symbol);

HuffmanNode* buildHuffmanTree(const std::string& text);

void buildCodesFromTree(HuffmanNode* root, HuffmanTable* table, const std::string& currentCode);

std::string encodeHuffman(const std::string& text, HuffmanTable* table);

std::string decodeHuffman(const std::string& encoded, HuffmanNode* root);

void printHuffmanTable(HuffmanTable* table);

void destroyHuffmanTree(HuffmanNode* root);

#endif
