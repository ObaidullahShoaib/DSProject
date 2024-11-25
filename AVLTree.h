#pragma once
#include <cmath>
#include "Node.h"

template<typename T>
class AVLTree {
private:
    Node<T>* root;

    int getHeight(Node<T>* node) const {
        if (node == nullptr)
            return -1;
        return node->height;
    }

    int getBalanceFactor(Node<T>* node) const {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(Node<T>* node) {
        if (node == nullptr) return;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    Node<T>* rightRotate(Node<T>* imbalancedNode) {
        if (imbalancedNode == nullptr || imbalancedNode->left == nullptr) {
            return imbalancedNode;
        }

        Node<T>* temp1 = imbalancedNode->left;
        Node<T>* temp2 = temp1->right;

        temp1->right = imbalancedNode;
        imbalancedNode->left = temp2;

        updateHeight(imbalancedNode);
        updateHeight(temp1);

        return temp1;
    }

    Node<T>* leftRotate(Node<T>* imbalancedNode) {
        if (imbalancedNode == nullptr || imbalancedNode->right == nullptr) {
            return imbalancedNode;
        }

        Node<T>* temp1 = imbalancedNode->right;
        Node<T>* temp2 = temp1->left;

        temp1->left = imbalancedNode;
        imbalancedNode->right = temp2;

        updateHeight(imbalancedNode);
        updateHeight(temp1);

        return temp1;
    }

    Node<T>* rebalance(Node<T>* node) {
        if (node == nullptr) return nullptr;

        updateHeight(node);
        int balance = getBalanceFactor(node);

        // Left Left Case
        if (balance > 1 && getBalanceFactor(node->left) >= 0)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && getBalanceFactor(node->right) <= 0)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && getBalanceFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node<T>* insertHelper(Node<T>* node, const T& key, const String& data) {
        if (node == nullptr)
            return new Node<T>(key, data);

        if (key < node->key)
            node->left = insertHelper(node->left, key, data);
        else if (key > node->key)
            node->right = insertHelper(node->right, key, data);
        else {
            // Update data if key already exists
            node->data = data;
            return node;
        }

        return rebalance(node);
    }

    Node<T>* deleteHelper(Node<T>* node, const T& key) {
        if (node == nullptr)
            return nullptr;

        if (key < node->key)
            node->left = deleteHelper(node->left, key);
        else if (key > node->key)
            node->right = deleteHelper(node->right, key);
        else {
            if (node->left == nullptr || node->right == nullptr) {
                Node<T>* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                delete temp;
            }
            else {
                Node<T>* temp = findMin(node->right);
                node->key = temp->key;
                node->data = temp->data;
                node->right = deleteHelper(node->right, temp->key);
            }
        }

        if (node == nullptr)
            return nullptr;

        return rebalance(node);
    }

    bool isAVLHelper(Node<T>* node) const {
        if (node == nullptr)
            return true;

        int balance = getBalanceFactor(node);
        if (balance > 1 || balance < -1)
            return false;

        return isAVLHelper(node->left) && isAVLHelper(node->right);
    }

    void inorderHelper(Node<T>* node) const {
        if (node == nullptr) return;
        inorderHelper(node->left);
        // cout << "Key: " << node->key << ", Data: " << node->data << " | ";
        cout << node->key << " ";
        inorderHelper(node->right);
    }

    void clearAll(Node<T>* node) {
        if (node == nullptr) return;
        clearAll(node->left);
        clearAll(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clearAll(root);
    }

    void insert(const T& key, const String& data) {
        root = insertHelper(root, key, data);
    }

    void remove(const T& key) {
        root = deleteHelper(root, key);
    }

    String search(const T& key) const {
        Node<T>* current = root;
        while (current != nullptr) {
            if (key == current->key)
                return current->data;
            else if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        return ""; // Return empty string if key not found
    }

    bool isAVL() const {
        return isAVLHelper(root);
    }

    void inorder() const {
        inorderHelper(root);
        cout << endl;
    }

    Node<T>* getRoot() const {
        return root;
    }
};