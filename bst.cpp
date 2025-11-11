#include <iostream>
#include "bst.h"
using namespace std;

BSTNode* createBST() {
    return nullptr;
}

BSTNode* insertBST(BSTNode* root, int key) {
    if (!root) {
        BSTNode* newNode = new BSTNode;
        newNode->key = key;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    
    if (key < root->key) {
        root->left = insertBST(root->left, key);
    } 
    else if (key > root->key) {
        root->right = insertBST(root->right, key);
    }
    
    return root;
}

BSTNode* searchBST(BSTNode* root, int key) {
    if (!root) return nullptr;
    
    if (key == root->key) {
        return root;
    }
    else if (key < root->key) {
        return searchBST(root->left, key);
    }
    else {
        return searchBST(root->right, key);
    }
}

BSTNode* findMinBST(BSTNode* root) {
    if (!root) return nullptr;
    
    while (root->left) {
        root = root->left;
    }
    
    return root;
}

BSTNode* deleteBST(BSTNode* root, int key) {
    if (!root) return nullptr;
    
    if (key < root->key) {
        root->left = deleteBST(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteBST(root->right, key);
    }
    else {
        if (!root->left) {
            BSTNode* temp = root->right;
            delete root;
            return temp;
        }
        else if (!root->right) {
            BSTNode* temp = root->left;
            delete root;
            return temp;
        }
        else {
            BSTNode* temp = findMinBST(root->right);
            root->key = temp->key;
            root->right = deleteBST(root->right, temp->key);
        }
    }
    
    return root;
}

void inOrderTraversal(BSTNode* root) {
    if (!root) return;
    
    inOrderTraversal(root->left);
    cout << root->key << " ";
    inOrderTraversal(root->right);
}

void preOrderTraversal(BSTNode* root) {
    if (!root) return;
    
    cout << root->key << " ";
    preOrderTraversal(root->left);
    preOrderTraversal(root->right);
}

void postOrderTraversal(BSTNode* root) {
    if (!root) return;
    
    postOrderTraversal(root->left);
    postOrderTraversal(root->right);
    cout << root->key << " ";
}

void printTree(BSTNode* root, int space) {
    if (!root) return;
    
    int COUNT = 5;
    space += COUNT;
    
    printTree(root->right, space);
    
    cout << endl;
    for (int i = COUNT; i < space; i++) {
        cout << " ";
    }
    cout << root->key << endl;
    
    printTree(root->left, space);
}

void destroyBST(BSTNode* root) {
    if (!root) return;
    
    destroyBST(root->left);
    destroyBST(root->right);
    delete root;
}
