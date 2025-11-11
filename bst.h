#ifndef BST_H
#define BST_H

#include <string>
using namespace std;

struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
};

BSTNode* createBST();
BSTNode* insertBST(BSTNode* root, int key);
BSTNode* searchBST(BSTNode* root, int key);
BSTNode* deleteBST(BSTNode* root, int key);
BSTNode* findMinBST(BSTNode* root);
void inOrderTraversal(BSTNode* root);
void preOrderTraversal(BSTNode* root);
void postOrderTraversal(BSTNode* root);
void printTree(BSTNode* root, int space);
void destroyBST(BSTNode* root);

#endif
