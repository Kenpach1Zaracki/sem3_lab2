#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
using namespace std;

struct Array {
    std::string name;
    std::string* data;
    int capacity;
    int size;
};

struct Node {
    string data;
    Node* next;
};

struct Stack {
    Node* top;
};

struct SimpleHashNode {
    string key;
    int value;
    SimpleHashNode* next;
};

struct SimpleHashTable {
    SimpleHashNode** table;
    int capacity;
    int size;
};

struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
};

struct Block {
    long long width;
    long long height;
};

struct HashNode {
    string key;
    int value;
    bool isEmpty;
    bool isDeleted;
};

struct HashTableOpen {
    HashNode* table;
    int capacity;
    int size;
};

struct ChainNode {
    string key;
    int value;
    ChainNode* next;
};

struct HashTableChain {
    ChainNode** table;
    int capacity;
    int size;
};

struct LFUNode {
    int key;
    int value;
    int frequency;
    int timestamp;
};

struct LFUCache {
    HashTableOpen* cache;
    int capacity;
    int currentTime;
};

#endif
