#include <iostream>
#include <algorithm>
#include "zad4.h"
#include "structures.h"
using namespace std;

// Функция сравнения для сортировки блоков
// Сортируем по ширине (убывание), при равенстве - по высоте (убывание)
bool compareBlocks(const Block& a, const Block& b) {
    if (a.width != b.width) {
        return a.width > b.width;  // По убыванию ширины
    }
    return a.height > b.height;  // По убыванию высоты
}

void buildMaxPyramid() {
    cout << "=== Задание 4: Пирамида из блоков ===" << endl;
    
    int N;
    cout << "Введите количество блоков (1 <= N <= 100000): ";
    cin >> N;
    
    if (cin.fail() || N <= 0 || N > 100000) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Ошибка: Неверное количество блоков" << endl;
        return;
    }
    
    // Создаем массив блоков
    Block* blocks = new Block[N];
    
    cout << "Введите " << N << " пар (ширина высота):" << endl;
    
    for (int i = 0; i < N; i++) {
        cout << "Блок " << (i + 1) << ": ";
        cin >> blocks[i].width >> blocks[i].height;
        
        if (cin.fail() || blocks[i].width <= 0 || blocks[i].height <= 0 ||
            blocks[i].width > 1000000000LL || blocks[i].height > 1000000000LL) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ошибка: Неверные значения (должны быть от 1 до 10^9)" << endl;
            delete[] blocks;
            return;
        }
    }
    
    // Выводим исходные блоки
    cout << "\n--- Исходные блоки ---" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Блок " << (i + 1) << ": ширина=" << blocks[i].width 
             << ", высота=" << blocks[i].height << endl;
    }
    
    // Сортируем блоки
    std::sort(blocks, blocks + N, compareBlocks);
    
    cout << "\n--- Отсортированные блоки (по убыванию ширины) ---" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Блок " << (i + 1) << ": ширина=" << blocks[i].width 
             << ", высота=" << blocks[i].height << endl;
    }
    
    // Применяем динамическое программирование
    long long* dp = new long long[N];
    
    // Инициализация: каждый блок сам по себе
    for (int i = 0; i < N; i++) {
        dp[i] = blocks[i].height;
    }
    
    // DP: для каждого блока i проверяем все предыдущие блоки j
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            // Можем поставить блок i на блок j, если ширина j СТРОГО больше
            if (blocks[j].width > blocks[i].width) {
                long long newHeight = dp[j] + blocks[i].height;
                if (newHeight > dp[i]) {
                    dp[i] = newHeight;
                }
            }
        }
    }
    
    // Находим максимальную высоту
    long long maxHeight = 0;
    for (int i = 0; i < N; i++) {
        if (dp[i] > maxHeight) {
            maxHeight = dp[i];
        }
    }
    
    cout << "\n--- Результат ---" << endl;
    cout << "Наибольшая высота пирамиды: " << maxHeight << endl;
    
    // Опционально: вывод самой пирамиды (обратный проход)
    // Найдем блок с максимальной высотой
    int maxIndex = 0;
    for (int i = 0; i < N; i++) {
        if (dp[i] == maxHeight) {
            maxIndex = i;
            break;
        }
    }
    
    // Восстанавливаем последовательность (простой вариант)
    cout << "\nОдна из возможных пирамид (снизу вверх):" << endl;
    
    // Используем массив для хранения индексов пирамиды
    int* pyramid = new int[N];
    int pyramidSize = 0;
    
    // Восстанавливаем путь
    long long currentHeight = maxHeight;
    int currentIndex = maxIndex;
    
    while (currentIndex >= 0) {
        pyramid[pyramidSize++] = currentIndex;
        
        long long prevHeight = currentHeight - blocks[currentIndex].height;
        if (prevHeight == 0) break;
        
        // Ищем предыдущий блок
        bool found = false;
        for (int j = 0; j < currentIndex; j++) {
            if (blocks[j].width > blocks[currentIndex].width && dp[j] == prevHeight) {
                currentHeight = prevHeight;
                currentIndex = j;
                found = true;
                break;
            }
        }
        
        if (!found) break;
    }
    
    // Выводим пирамиду снизу вверх
    for (int i = pyramidSize - 1; i >= 0; i--) {
        int idx = pyramid[i];
        cout << "  Блок: ширина=" << blocks[idx].width 
             << ", высота=" << blocks[idx].height << endl;
    }
    
    // Освобождаем память
    delete[] pyramid;
    delete[] dp;
    delete[] blocks;
}
