#pragma once
#include <iostream>
#include <fstream>
#include "String.h"
#include <sstream>
#include "AVLTree.h"
#include "RedBlackTree.h"
#include "FileReader.h"
#include "FolderManager.h"
using namespace std;

class Branch {
	FileReader fileReader;
	Tree<String>* tree1;
	FolderManager folderManager;
	fs::path branchName;

	void initializeTree(int rowCount) {
		int choice;
		do {
			cout << "Enter 1 for Red Black Tree\nEnter 2 for AVL Tree\nEnter 3 for B-Tree\nChoice: ";
			cin >> choice;

			if (choice == 1) {
				tree1 = new RedBlackTree<String>();
				break; // Exit the loop
			}
			else if (choice == 2) {
				tree1 = new AVLTree<String>();
				break; // Exit the loop
			}
			// Uncomment this block if BTree option is needed
			// else if (choice == 3) {
			//     tree1 = new B-Tree<String>();
			//     break; // Exit the loop
			// }
			else {
				cout << "Invalid choice. Please enter a valid option.\n";
			}
		} while (true);

		for (int i = 0; i < rowCount; i++) {
			String key, data;
			fileReader.getInformation(key, data, i);
			tree1->insert(key, data);
		}
		cout << "Inorder traversal of AVL Tree: " << endl;
		tree1->inorder();
		tree1->deleteTree();
		cout << endl;
		tree1->inorder();
	}

public:
	Branch(fs::path repoPath):folderManager(repoPath){}

	Branch(fs::path branchName, fs::path repoPath) : branchName(branchName), tree1(nullptr), folderManager(repoPath) {
		if (branchName == "main")
		{
			folderManager.create_folder(this->branchName);
			folderManager.create_folder(folderManager.get_current_path() / this->branchName / "Nodes");

		}
		else
		{
			//string source;
			//cout << "Enter the branch name to copy from: ";
			//cin >> source;

			//folderManager.copyFolder(branchName, source);
		}
	}

	//// copy constructor
	//Branch(const Branch& branch) : branchName(branch.branchName), tree1(nullptr) {
	//	folderManager.create_folder(folderManager.get_current_path(), this->branchName);
	//	folderManager.create_folder(folderManager.get_current_path(), "Nodes");
	//	this->init();
	//}

	////assignment operator
	//Branch& operator=(const Branch& branch) {
	//	if (this != &branch) {
	//		this->branchName = branch.branchName;
	//		folderManager.create_folder(folderManager.get_current_path(), this->branchName);
	//		folderManager.create_folder(folderManager.get_current_path(), "Nodes");
	//		this->init();
	//	}
	//	return *this;
	//}

	~Branch() {
		if (tree1) {
			tree1->deleteTree();
			delete tree1;       
		}
	}

	void init() {
		fileReader.openFile();
		fileReader.readFile();
		int rowCount = fileReader.getRowCount();
		initializeTree(rowCount);
	}

	fs::path getBranchName() {
		return branchName;
	}



};