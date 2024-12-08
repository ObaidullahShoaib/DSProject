#pragma once

#include "String.h"
#include "Hashing.h"
#include <filesystem>

namespace fs = std::filesystem;

#define FOR_FAST(start, end, step) for (int i = (start); i < (end); i += (step))
#define FOR(start, end) for (int i = (start); i < (end); i++)

template <typename T>
struct TreeNode {

	// States:
	T key;
	String data;
	int numOfChildren;
	int id;
	fs::path nodeName;
	// Hashes:
	int hashType;
	int instructorHash;
	unsigned char shaHash[SHA256_DIGEST_LENGTH];
	bool updated;

	// Number of children by default is 2 because that will be the minimum number of children in our implementation
	TreeNode(int id) : numOfChildren(2), id(id), updated(false) {
		nodeName = generateNodeName();
	}

	TreeNode(T key, String data, int hashType, int id, int numOfChildren = 2)
		: key(key), data(data), numOfChildren(2), hashType(hashType), id(id), updated(false) {
		nodeName = generateNodeName();
	}

	void rehash() {
		if (hashType == 1)
		{
			if (this->descendants[0] == nullptr)
				this->instructorHash = this->descendants[1]->instructorHash;
			else if (this->descendants[1] == nullptr)
				this->instructorHash = this->descendants[0]->instructorHash;
			else
			{
				this->instructorHash = this->descendants[0]->instructorHash * this->descendants[1]->instructorHash;
				this->instructorHash %= 29;
			}
		}
		else if (hashType == 2)
		{
			if (this->descendants[0] == nullptr)
				this->shaHash = this->descendants[1]->shaHash;
			else if (this->descendants[1] == nullptr)
				this->shaHash = this->descendants[0]->shaHash;
			else
				this->shaHash = my_strcat(this->descendants[0]->shaHash, this->descendants[1]->shaHash);
		}
	}

	~TreeNode() { }
	virtual TreeNode<T>* getChild(int index) = 0;
	virtual TreeNode<T>* getParent() = 0;

private:
	fs::path generateNodeName() {
		std::ostringstream oss;
		oss << "Node_" << id << ".txt";
		return oss.str();
	}
};


template<typename T>
struct AVLNode : public TreeNode<T> {

	// States:
	AVLNode* parent;
	AVLNode* descendants[2];
	int height;


	// Constructors:
	AVLNode() : height(0), parent(nullptr) { this->nullAllDescendants(); }
	AVLNode(T key, String data, int hashType, int id) : height(0), TreeNode<T>(key, data, hashType, id), parent(nullptr) { this->nullAllDescendants(); }


	// Sets both descendants to nullptr:
	void nullAllDescendants() { FOR(0, this->numOfChildren) this->descendants[i] = nullptr; }
	TreeNode<T>* getChild(int index) override { return this->descendants[index]; }
	TreeNode<T>* getParent() override { return this->parent; }

	bool isLeaf() { return this->descendants[0] == nullptr && this->descendants[1] == nullptr; }
};

template <typename T>
class RBNode : public TreeNode<T> {
public:
	int color; // 0:Red, 1:Black
	RBNode* parent;
	RBNode* descendants[2];

	RBNode(int id) : color(0), parent(nullptr) { this->nullAllDescendants(); }
	RBNode(T key, String data, int hashType, int id, RBNode* parent = nullptr)
		: color(0), TreeNode<T>(key, data, hashType, id), parent(parent) { this->nullAllDescendants(); }

	void nullAllDescendants() { FOR(0, this->numOfChildren) this->descendants[i] = nullptr; }
	TreeNode<T>* getChild(int index) override { return this->descendants[index]; }
	TreeNode<T>* getParent() override { return this->parent; }

	void setColor(int color)
	{
		if (this->color != color)
		{
			this->color = color;
			this->updated = true;
		}
	}

	void setParent(RBNode* newParent)
	{
		if (this->parent != newParent)
		{
			this->parent = newParent;
			this->updated = true;
		}
	}

	bool isLeaf() { return this->descendants[0] == nullptr && this->descendants[1] == nullptr; }
};

class MerkleNode {
public:
	int hashType;
	int instructorHash;
	unsigned char shaHash[SHA256_DIGEST_LENGTH];

	TreeNode<String>* referencedNode;
	MerkleNode** descendants;
	MerkleNode* parent;

	MerkleNode(int hashType, TreeNode<String>* referencedNode)
		:hashType(hashType), referencedNode(referencedNode), parent(nullptr) {
		this->nullAllDescendants();
	}

	void nullAllDescendants() { FOR(0, 2) this->descendants[i] = nullptr; }
	MerkleNode* getChild(int index) { return this->descendants[index]; }
	MerkleNode* getParent() { return this->parent; }

	void setParent(MerkleNode* newParent) { this->parent = newParent; }
};