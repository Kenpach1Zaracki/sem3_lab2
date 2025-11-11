#include <iostream>
#include <string>
#include <cctype>
#include "stack.h"
#include "zad1.h"
using namespace std;

int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*') return 2;
    return 0;
}

int applyOperation(int a, int b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    return 0;
}

int evaluateExpression(const string& expr) {
    Stack* values = createStack();
    Stack* operators = createStack();
    
    bool expectOperand = true;
    
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ') continue;
        
        if (isdigit(expr[i])) {
            int num = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            i--;
            push(values, to_string(num));
            expectOperand = false;
        }
        else if (expr[i] == '(') {
            push(operators, string(1, expr[i]));
            expectOperand = true;
        }
        else if (expr[i] == ')') {
            while (!isEmpty(operators) && top(operators) != "(") {
                int val2 = stoi(top(values)); pop(values);
                int val1 = stoi(top(values)); pop(values);
                char op = top(operators)[0]; pop(operators);
                
                int result = applyOperation(val1, val2, op);
                push(values, to_string(result));
            }
            
            if (!isEmpty(operators)) {
                pop(operators);
            }
            expectOperand = false;
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*') {
            if (expr[i] == '-' && expectOperand) {
                push(values, "0");
                expectOperand = false;
            }
            
            while (!isEmpty(operators) && 
                   top(operators) != "(" && 
                   getPriority(top(operators)[0]) >= getPriority(expr[i])) {
                
                int val2 = stoi(top(values)); pop(values);
                int val1 = stoi(top(values)); pop(values);
                char op = top(operators)[0]; pop(operators);
                
                int result = applyOperation(val1, val2, op);
                push(values, to_string(result));
            }
            
            push(operators, string(1, expr[i]));
            expectOperand = true;
        }
    }
    
    while (!isEmpty(operators)) {
        int val2 = stoi(top(values)); pop(values);
        int val1 = stoi(top(values)); pop(values);
        char op = top(operators)[0]; pop(operators);
        
        int result = applyOperation(val1, val2, op);
        push(values, to_string(result));
    }
    
    int result = stoi(top(values));
    
    destroyStack(values);
    destroyStack(operators);
    
    return result;
}

void calculateExpression() {
    cout << "=== Задание 1: Вычисление арифметического выражения ===" << endl;
    cout << "Поддерживаемые операции: +, -, *, скобки ()" << endl;
    
    string expression;
    cout << "Введите выражение: ";
    
    cin.ignore();
    getline(cin, expression);
    
    if (expression.empty()) {
        cout << "Ошибка: Пустое выражение" << endl;
        return;
    }
    
    int result = evaluateExpression(expression);
    cout << "Результат: " << result << endl;
}