#pragma once

#include "Nodes.h"

class MerkleTree
{
public:
	MerkleTree(AVLNode<String>* root)
	{
		this->constructMerkleTree(root);
	}

	// Recursively construct the merkle tree using indoreder traversal of the root, 
	// such that all the nodes of the tree whose root passed are now leaf nodes in the merkle tree, 
	// and the parents of nodes in merkle tree are hashed based on children hashes (using my rehash function)
	void constructMerkleTree(AVLNode<String>* root)
	{
		if (root != nullptr)
		{
			constructMerkleTree(root->descendants[0]);
			constructMerkleTree(root->descendants[1]);
			this->insertMerkleNode(root);
		}
	}

	void insertMerkleNode(AVLNode<String>* root)
	{
		
	}
};