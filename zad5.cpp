#include <iostream>
#include "zad5.h"
#include "bst.h"
using namespace std;

void processHuffman() {
    cout << "=== Задание 5: Бинарное дерево поиска ===" << endl;
    
    BSTNode* root = createBST();
    
    cout << "\nКоманды:" << endl;
    cout << "  INSERT <число> - вставить" << endl;
    cout << "  SEARCH <число> - найти" << endl;
    cout << "  DELETE <число> - удалить" << endl;
    cout << "  INORDER - центрированный обход" << endl;
    cout << "  PREORDER - префиксный обход" << endl;
    cout << "  POSTORDER - постфиксный обход" << endl;
    cout << "  PRINT - показать дерево" << endl;
    cout << "  EXIT - выход" << endl;
    
    string command;
    
    while (true) {
        cout << "\n> ";
        cin >> command;
        
        if (command == "EXIT") {
            break;
        }
        else if (command == "INSERT") {
            int key;
            cin >> key;
            root = insertBST(root, key);
            cout << "Добавлено: " << key << endl;
        }
        else if (command == "SEARCH") {
            int key;
            cin >> key;
            BSTNode* found = searchBST(root, key);
            if (found) {
                cout << "Найдено: " << key << endl;
            } else {
                cout << "Не найдено: " << key << endl;
            }
        }
        else if (command == "DELETE") {
            int key;
            cin >> key;
            root = deleteBST(root, key);
            cout << "Удалено: " << key << endl;
        }
        else if (command == "INORDER") {
            cout << "In-Order: ";
            inOrderTraversal(root);
            cout << endl;
        }
        else if (command == "PREORDER") {
            cout << "Pre-Order: ";
            preOrderTraversal(root);
            cout << endl;
        }
        else if (command == "POSTORDER") {
            cout << "Post-Order: ";
            postOrderTraversal(root);
            cout << endl;
        }
        else if (command == "PRINT") {
            cout << "\nДерево:" << endl;
            printTree(root, 0);
        }
        else {
            cout << "Неизвестная команда" << endl;
        }
    }
    
    destroyBST(root);
}
