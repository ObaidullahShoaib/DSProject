#pragma once
#include "Tree.h"
// AVl class that also manage the parent pointer
template <typename T>
class AVLTree :public Tree<T> {
	AVLNode<T>* root;
public:
	AVLTree() : root(nullptr) {}
	AVLTree(const AVLTree& other) {
		root = copyTree(other.root);
	}
	AVLTree<T>& operator=(const AVLTree<T>& other) {
		if (this != &other) {
			deleteTree();
			root = copyTree(other.root);
		}
		return *this;
	}
	~AVLTree() {
		deleteTree();
	}
	void insert(T key, String data, int hashType) override {
		root = insertHelper(root, key, data, hashType);
	}
	void remove(T key) override {
		root = removeHelper(root, key);
	}

	void inorder() const override {
		inorderHelper(root);
	}
	void deleteTree() override {
		deleteTreeHelper(root);
		root = nullptr;
	}
	TreeNode<T>* getRoot() override { return root; }
	Tree<T>* clone() override {
		return new AVLTree<T>(*this);
	}
private:
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

	void setParent(AVLNode<T>* child, AVLNode<T>* parent) {
		if (child != nullptr) {
			child->parent = parent;
		}
	}

	// rotate right and also update parent pointer
	AVLNode<T>* rotateRight(AVLNode<T>* node) {
		AVLNode<T>* leftChild = node->descendants[0];
		AVLNode<T>* leftRightChild = leftChild->descendants[1];
		leftChild->descendants[1] = node;
		node->descendants[0] = leftRightChild;
		updateHeight(node);
		updateHeight(leftChild);
		setParent(leftRightChild, node);
		setParent(node, leftChild);
		if (node == root) {
			root = leftChild;
			setParent(leftChild, nullptr);
		}
		return leftChild;
	}

	// rotate left and also update parent pointer
	AVLNode<T>* rotateLeft(AVLNode<T>* node) {
		AVLNode<T>* rightChild = node->descendants[1];
		AVLNode<T>* rightLeftChild = rightChild->descendants[0];
		rightChild->descendants[0] = node;
		node->descendants[1] = rightLeftChild;
		updateHeight(node);
		updateHeight(rightChild);
		setParent(rightLeftChild, node);
		setParent(node, rightChild);
		if (node == root) {
			root = rightChild;
			setParent(rightChild, nullptr);
		}
		return rightChild;
	}

	// balance the tree
	AVLNode<T>* rebalance(AVLNode<T>* node) {
		updateHeight(node);
		int balanceFactor = getBalanceFactor(node);
		if (balanceFactor > 1) {
			if (getBalanceFactor(node->descendants[0]) < 0) {
				node->descendants[0] = rotateLeft(node->descendants[0]);
			}
			node = rotateRight(node);
		}
		else if (balanceFactor < -1) {
			if (getBalanceFactor(node->descendants[1]) > 0) {
				node->descendants[1] = rotateRight(node->descendants[1]);
			}
			node = rotateLeft(node);
		}
		return node;
	}


	AVLNode<T>* insertHelper(AVLNode<T>* node, T key, String data, int hashType) {
		if (node == nullptr) {
			return new AVLNode<T>(key, data, hashType);
		}
		if (key < node->key) {
			node->descendants[0] = insertHelper(node->descendants[0], key, data, hashType);
			// Explicitly set parent
			setParent(node->descendants[0], node);
		}
		else if (key >= node->key) {
			node->descendants[1] = insertHelper(node->descendants[1], key, data, hashType);
			// Explicitly set parent
			setParent(node->descendants[1], node);
		}
		return rebalance(node);
	}

	// remove a node
	AVLNode<T>* removeHelper(AVLNode<T>* node, T key) {
		if (node == nullptr) return nullptr;
		if (key < node->key) {
			node->descendants[0] = removeHelper(node->descendants[0], key);
			setParent(node->descendants[0], node);
		}
		else if (key > node->key) {
			node->descendants[1] = removeHelper(node->descendants[1], key);
			setParent(node->descendants[1], node);
		}
		else {
			if (node->descendants[0] == nullptr || node->descendants[1] == nullptr) {
				AVLNode<T>* temp = node->descendants[0] ? node->descendants[0] : node->descendants[1];
				if (temp == nullptr) {
					temp = node;
					node = nullptr;
				}
				else {
					*node = *temp;
				}
				delete temp;
			}
			else {
				AVLNode<T>* temp = findMin(node->descendants[1]);
				node->key = temp->key;
				node->data = temp->data;
				node->descendants[1] = removeHelper(node->descendants[1], temp->key);
				setParent(node->descendants[1], node);
			}
		}

		if (node == nullptr) return nullptr;
		return rebalance(node);
	}

	// is AVLTree correct
	bool isAVLTree(AVLNode<T>* node) {
		if (node == nullptr) return true;
		int balanceFactor = getBalanceFactor(node);
		if (balanceFactor > 1 || balanceFactor < -1) return false;
		return isAVLTree(node->descendants[0]) && isAVLTree(node->descendants[1]);
	}

	// inorder traversal
	void inorderHelper(AVLNode<T>* node) const {
		if (node == nullptr) return;
		inorderHelper(node->descendants[0]);
		cout << node->key << " ";
		inorderHelper(node->descendants[1]);
	}

	// delete the tree
	void deleteTreeHelper(AVLNode<T>* node) {
		if (node == nullptr) return;
		deleteTreeHelper(node->descendants[0]);
		deleteTreeHelper(node->descendants[1]);
		delete node;
	}

	// copy the tree
	AVLNode<T>* copyTree(const AVLNode<T>* other) {
		if (other == nullptr) return nullptr;

		AVLNode<T>* newNode = new AVLNode<T>(other->key, other->data, other->hashType);

		newNode->height = other->height;

		newNode->descendants[0] = copyTree(other->descendants[0]);
		newNode->descendants[1] = copyTree(other->descendants[1]);

		if (newNode->descendants[0])
			newNode->descendants[0]->parent = newNode;
		if (newNode->descendants[1])
			newNode->descendants[1]->parent = newNode;

		return newNode;
	}
};