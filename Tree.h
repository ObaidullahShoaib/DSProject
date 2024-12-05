#pragma once
#include "String.h"
#include "Nodes.h"
template <typename T>
class Tree {
public:
	virtual void insert(T, String, int) = 0;
	virtual void remove(T) = 0;
	//virtual TreeNode<T>* search(const T&) = 0;
	virtual void inorder() const = 0;
	virtual void deleteTree() = 0;
	virtual TreeNode<T>* getRoot() = 0;
	virtual ~Tree() {}
	virtual Tree<T>* clone() = 0;
};
