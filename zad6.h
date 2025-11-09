#ifndef ZAD6_H
#define ZAD6_H
#include <string>
#include "structures.h"

int hashFoldingMethod(const std::string& key, int capacity);

int hashSimple(const std::string& key, int capacity);

HashTableOpen* createHashTableOpen(int capacity);

void destroyHashTableOpen(HashTableOpen* table);

bool insertHashTableOpen(HashTableOpen* table, const std::string& key, int value);

int searchHashTableOpen(HashTableOpen* table, const std::string& key);

bool deleteHashTableOpen(HashTableOpen* table, const std::string& key);

bool rehashHashTableOpen(HashTableOpen* table);

void printHashTableOpen(HashTableOpen* table);

double getLoadFactorOpen(HashTableOpen* table);

HashTableChain* createHashTableChain(int capacity);

void destroyHashTableChain(HashTableChain* table);

bool insertHashTableChain(HashTableChain* table, const std::string& key, int value);

int searchHashTableChain(HashTableChain* table, const std::string& key);

bool deleteHashTableChain(HashTableChain* table, const std::string& key);

bool rehashHashTableChain(HashTableChain* table);

void printHashTableChain(HashTableChain* table);

double getLoadFactorChain(HashTableChain* table);


void empiricalAnalysis();

void processHashTables();

#endif
