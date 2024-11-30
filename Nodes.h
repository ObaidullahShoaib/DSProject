#pragma once

#include "String.h"

#define FOR_FAST(start, end, step) for (int i = (start); i < (end); i += (step))
#define FOR(start, end) for (int i = (start); i < (end); i++)

template <typename T>
struct TreeNode {

	T key;
	String data;
	int numOfChildren;
	int hash;

	// Number of children by default is 2 because that will be the minimum number of children in our implementation
	TreeNode() : numOfChildren(2) {};
	TreeNode(T key, String data, int numOfChildren = 2) : key(key), data(data), numOfChildren(2), hash(this->generateHash()) {}


	// Hash-generator Method:
	int generateHash()
	{
		return 0;
	};
};

template<typename T>
struct AVLNode : public TreeNode<T> {

	// States:
	AVLNode* parent;
	AVLNode* descendants[2];
	int height;


	// Constructors:
	AVLNode(AVLNode* parent = nullptr) : height(0), parent(parent) { this->nullAllDescendants(); }
	AVLNode(T key, String data, AVLNode* parent = nullptr) : height(0), TreeNode<T>(key, data), parent(parent) { this->nullAllDescendants(); }


	// Sets both descendants to nullptr:
	void nullAllDescendants() { FOR(0, this->numOfChildren) this->descendants[i] = nullptr; }

};

template <typename T>
class RBNode : public TreeNode<T> {
public:
	int color; // 0:Red, 1:Black
	RBNode* parent;
	RBNode* descendants[2];

	RBNode() : color(0), parent(nullptr) { this->nullAllDescendants(); }
	RBNode(T key, String data, RBNode* parent = nullptr) : color(0), TreeNode<T>(key, data), parent(parent) { this->nullAllDescendants(); }

	void nullAllDescendants() { FOR(0, this->numOfChildren) this->descendants[i] = nullptr; }
};