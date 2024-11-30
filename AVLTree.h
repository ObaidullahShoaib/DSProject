#pragma once
#include <cmath>
#include "Nodes.h"
#include "Tree.h"

template<typename T>
class AVLTree: public Tree<T> {
private:
    AVLNode<T>* root;

	AVLNode<T>* findMin(AVLNode<T>* node) const {
		if (node == nullptr)
			return nullptr;
		while (node->descendants[0] != nullptr)
			node = node->descendants[0];
		return node;
	}

    int getHeight(AVLNode<T>* node) const {
        if (node == nullptr)
            return -1;
        return node->height;
    }

    int getBalanceFactor(AVLNode<T>* node) const {
        if (node == nullptr)
            return 0;
        return getHeight(node->descendants[0]) - getHeight(node->descendants[1]);
    }

    void updateHeight(AVLNode<T>* node) {
        if (node == nullptr) return;
        node->height = 1 + max(getHeight(node->descendants[0]), getHeight(node->descendants[1]));
    }

    AVLNode<T>* rightRotate(AVLNode<T>* imbalancedNode) {
        if (imbalancedNode == nullptr || imbalancedNode->descendants[0] == nullptr) {
            return imbalancedNode;
        }

        AVLNode<T>* temp1 = imbalancedNode->descendants[0];
        AVLNode<T>* temp2 = temp1->descendants[1];

        temp1->descendants[1] = imbalancedNode;
        imbalancedNode->descendants[0] = temp2;

        updateHeight(imbalancedNode);
        updateHeight(temp1);

        return temp1;
    }

    AVLNode<T>* leftRotate(AVLNode<T>* imbalancedNode) {
        if (imbalancedNode == nullptr || imbalancedNode->descendants[1] == nullptr) {
            return imbalancedNode;
        }

        AVLNode<T>* temp1 = imbalancedNode->descendants[1];
        AVLNode<T>* temp2 = temp1->descendants[0];

        temp1->descendants[0] = imbalancedNode;
        imbalancedNode->descendants[1] = temp2;

        updateHeight(imbalancedNode);
        updateHeight(temp1);

        return temp1;
    }

    AVLNode<T>* rebalance(AVLNode<T>* node) {
        if (node == nullptr) return nullptr;

        updateHeight(node);
        int balance = getBalanceFactor(node);

        // Left Left Case
        if (balance > 1 && getBalanceFactor(node->descendants[0]) >= 0)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && getBalanceFactor(node->descendants[1]) <= 0)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && getBalanceFactor(node->descendants[0]) < 0) {
            node->descendants[0] = leftRotate(node->descendants[0]);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && getBalanceFactor(node->descendants[1]) > 0) {
            node->descendants[1] = rightRotate(node->descendants[1]);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode<T>* insertHelper(AVLNode<T>* node, const T& key, const String& data) {
        if (node == nullptr)
            return new AVLNode<T>(key, data);

        if (key < node->key)
            node->descendants[0] = insertHelper(node->descendants[0], key, data);
		else // key >= node->key
            node->descendants[1] = insertHelper(node->descendants[1], key, data);

        return rebalance(node);
    }

    AVLNode<T>* deleteHelper(AVLNode<T>* node, const T& key) {
        if (node == nullptr)
            return nullptr;

        // Perform standard BST deletion
        if (key < node->key)
            node->descendants[0] = deleteHelper(node->descendants[0], key); // left child
        else if (key >= node->key)
            node->descendants[1] = deleteHelper(node->descendants[1], key); // right child
        else { // node to be deleted is found
            if (node->descendants[0] == nullptr || node->descendants[1] == nullptr) {
                AVLNode<T>* temp = node->descendants[0] ? node->descendants[0] : node->descendants[1];
                if (temp == nullptr) { // No children
                    temp = node;
                    node = nullptr;
                }
                else { // One child
                    *node = *temp; // Copy the contents of the non-null child
                }
                delete temp; // Delete the node
            }
            else { // Node has two children
                AVLNode<T>* temp = findMin(node->descendants[1]); // Get the in-order successor
                node->key = temp->key; // Copy the in-order successor's content to the current node
                node->data = temp->data;
                node->descendants[1] = deleteHelper(node->descendants[1], temp->key); // Delete the in-order successor
            }
        }

        if (node == nullptr)
            return nullptr;

        return rebalance(node); // Rebalance the tree if necessary
    }

    bool isAVLHelper(AVLNode<T>* node) const {
        if (node == nullptr)
            return true;

        int balance = getBalanceFactor(node);
        if (balance > 1 || balance < -1)
            return false;

        return isAVLHelper(node->descendants[0]) && isAVLHelper(node->descendants[1]);
    }

    template <typename T>
    void inorderHelper(AVLNode<T>* node) const {
        if (node == nullptr) return;
        inorderHelper(node->descendants[0]);  // Left child
        cout << node->key << " ";             // Print the current node's key
        inorderHelper(node->descendants[1]);  // Right child
    }


    void clearAll(AVLNode<T>* node) {
        if (node == nullptr) return;
        clearAll(node->descendants[0]);
        clearAll(node->descendants[1]);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        clearAll(root);
    }

    void deleteTree() override {
		clearAll(root);
		root = nullptr;
    }

    void insert(const T key, const String data) override {
        root = insertHelper(root, key, data);
    }

    void remove(const T key) override{
        root = deleteHelper(root, key);
    }

    TreeNode<T>* search(const T& key) override {
        AVLNode<T>* current = root;
        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            else if (current->key < key) {
                current = current->descendants[1];
            }
            else {
                current = current->descendants[0];
            }
        }
        return new AVLNode<T>();
    }

    bool isAVL() const {
        return isAVLHelper(root);
    }

    void inorder() const override {
		if (root == nullptr) {
			cout << "Tree is empty" << endl;
			return;
		}
        inorderHelper(root);
        cout << endl;
    }

    AVLNode<T>* getRoot() const {
        return root;
    }
};