#include <iostream>
#include <algorithm>
#include "zad4.h"

using namespace std;

struct Block {
    int width;
    int height;
};

bool compareBlocks(const Block& a, const Block& b) {
    if (a.width != b.width) {
        return a.width > b.width;
    }
    return a.height > b.height;
}

void buildMaxPyramid() {
    cout << "=== Задание 4: Пирамида из блоков ===" << endl;
    
    int N;
    cout << "Введите количество блоков: ";
    cin >> N;
    
    if (N <= 0 || N > 100000) {
        cout << "Некорректное количество блоков" << endl;
        return;
    }
    
    Block* blocks = new Block[N];
    
    cout << "Введите блоки (ширина высота):" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Блок " << (i + 1) << ": ";
        cin >> blocks[i].width >> blocks[i].height;
    }
    
    cout << "\nСортировка блоков по ширине..." << endl;
    sort(blocks, blocks + N, compareBlocks);
    
    cout << "Блоки после сортировки:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "  Блок " << (i + 1) << ": ширина=" << blocks[i].width 
             << ", высота=" << blocks[i].height << endl;
    }
    
    long long* dp = new long long[N];
    
    for (int i = 0; i < N; i++) {
        dp[i] = blocks[i].height;
    }
    
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (blocks[j].width > blocks[i].width) {
                long long newHeight = dp[j] + blocks[i].height;
                if (newHeight > dp[i]) {
                    dp[i] = newHeight;
                }
            }
        }
    }
    
    long long maxHeight = 0;
    for (int i = 0; i < N; i++) {
        if (dp[i] > maxHeight) {
            maxHeight = dp[i];
        }
    }
    
    cout << "\nНаибольшая высота пирамиды: " << maxHeight << endl;
    
    delete[] dp;
    delete[] blocks;
}
