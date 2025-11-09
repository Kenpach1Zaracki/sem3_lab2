#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <climits>
#include "stack.h"
#include "zad1.h"
using namespace std;

// Получить приоритет оператора
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*') return 2;
    return 0;
}

// Применить операцию
long long applyOperation(long long a, long long b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        default: return 0;
    }
}

// Проверка на переполнение
void checkOverflow(long long result) {
    if (result > 2000000000LL || result < -2000000000LL) {
        throw runtime_error("Результат превышает допустимый диапазон (2×10^9)");
    }
}

int evaluateExpression(const string& expr) {
    Stack* values = createStack();    
    Stack* operators = createStack(); 
    
    bool expectOperand = true;  // Ожидаем операнд (для обработки унарного минуса)
    
    for (size_t i = 0; i < expr.length(); i++) {
        // Пропускаем пробелы
        if (expr[i] == ' ') continue;
        
        // Если цифра - читаем число
        if (isdigit(expr[i])) {
            if (!expectOperand) {
                destroyStack(values);
                destroyStack(operators);
                throw runtime_error("Неожиданное число");
            }
            
            long long num = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                if (num > 2000000000LL) {
                    destroyStack(values);
                    destroyStack(operators);
                    throw runtime_error("Число превышает 2×10^9");
                }
                i++;
            }
            i--;  // Откатываемся на один символ назад
            
            push(values, to_string(num));
            expectOperand = false;
        }
        // Открывающая скобка
        else if (expr[i] == '(') {
            if (!expectOperand) {
                destroyStack(values);
                destroyStack(operators);
                throw runtime_error("Неожиданная открывающая скобка");
            }
            push(operators, string(1, expr[i]));
            expectOperand = true;
        }
        // Закрывающая скобка
        else if (expr[i] == ')') {
            if (expectOperand) {
                destroyStack(values);
                destroyStack(operators);
                throw runtime_error("Пустое выражение в скобках");
            }
            
            // Применяем все операции до открывающей скобки
            while (!isEmpty(operators) && top(operators) != "(") {
                if (isEmpty(values)) {
                    destroyStack(values);
                    destroyStack(operators);
                    throw runtime_error("Недостаточно операндов");
                }
                long long val2 = stoll(top(values)); pop(values);
                
                if (isEmpty(values)) {
                    destroyStack(values);
                    destroyStack(operators);
                    throw runtime_error("Недостаточно операндов");
                }
                long long val1 = stoll(top(values)); pop(values);
                
                char op = top(operators)[0]; pop(operators);
                
                long long result = applyOperation(val1, val2, op);
                checkOverflow(result);
                
                push(values, to_string(result));
            }
            
            // Проверка наличия открывающей скобки
            if (isEmpty(operators)) {
                destroyStack(values);
                destroyStack(operators);
                throw runtime_error("Несбалансированные скобки");
            }
            pop(operators);  // Удаляем '('
            expectOperand = false;
        }
        // Операторы +, -, *
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*') {
            // Обработка унарного минуса
            if (expr[i] == '-' && expectOperand) {
                push(values, "0");
                expectOperand = false;
            }
            
            if (expectOperand && expr[i] != '-') {
                destroyStack(values);
                destroyStack(operators);
                throw runtime_error("Неожиданный оператор");
            }
            
            // Применяем операции с более высоким или равным приоритетом
            while (!isEmpty(operators) && 
                   top(operators) != "(" && 
                   getPriority(top(operators)[0]) >= getPriority(expr[i])) {
                
                if (isEmpty(values)) {
                    destroyStack(values);
                    destroyStack(operators);
                    throw runtime_error("Недостаточно операндов");
                }
                long long val2 = stoll(top(values)); pop(values);
                
                if (isEmpty(values)) {
                    destroyStack(values);
                    destroyStack(operators);
                    throw runtime_error("Недостаточно операндов");
                }
                long long val1 = stoll(top(values)); pop(values);
                
                char op = top(operators)[0]; pop(operators);
                
                long long result = applyOperation(val1, val2, op);
                checkOverflow(result);
                
                push(values, to_string(result));
            }
            
            push(operators, string(1, expr[i]));
            expectOperand = true;
        }
        // Недопустимый символ
        else {
            destroyStack(values);
            destroyStack(operators);
            throw runtime_error("Недопустимый символ: " + string(1, expr[i]));
        }
    }
    
    if (expectOperand) {
        destroyStack(values);
        destroyStack(operators);
        throw runtime_error("Выражение заканчивается оператором");
    }
    
    // Применяем оставшиеся операции
    while (!isEmpty(operators)) {
        if (top(operators) == "(") {
            destroyStack(values);
            destroyStack(operators);
            throw runtime_error("Несбалансированные скобки");
        }
        
        if (isEmpty(values)) {
            destroyStack(values);
            destroyStack(operators);
            throw runtime_error("Недостаточно операндов");
        }
        long long val2 = stoll(top(values)); pop(values);
        
        if (isEmpty(values)) {
            destroyStack(values);
            destroyStack(operators);
            throw runtime_error("Недостаточно операндов");
        }
        long long val1 = stoll(top(values)); pop(values);
        
        char op = top(operators)[0]; pop(operators);
        
        long long result = applyOperation(val1, val2, op);
        checkOverflow(result);
        
        push(values, to_string(result));
    }
    
    // Должно остаться ровно одно значение
    if (isEmpty(values)) {
        destroyStack(values);
        destroyStack(operators);
        throw runtime_error("Пустое выражение");
    }
    
    string resultStr = top(values);
    pop(values);
    
    if (!isEmpty(values)) {
        destroyStack(values);
        destroyStack(operators);
        throw runtime_error("Некорректное выражение");
    }
    
    int result = stoi(resultStr);
    
    destroyStack(values);
    destroyStack(operators);
    
    return result;
}

void calculateExpression() {
    cout << "=== Задание 1: Вычисление арифметического выражения ===" << endl;
    cout << "Поддерживаемые операции: +, -, *, скобки ()" << endl;
    cout << "Введите выражение: ";
    
    string expression;
    getline(cin, expression);
    
    if (expression.empty()) {
        cout << "Ошибка: Пустое выражение" << endl;
        return;
    }
    
    try {
        int result = evaluateExpression(expression);
        cout << "Результат: " << result << endl;
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}
