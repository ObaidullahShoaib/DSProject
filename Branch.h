#pragma once
#include <iostream>
#include <fstream>
#include "String.h"
#include <sstream>
#include "AVLTree.h"
#include "RedBlackTree.h"
#include "FileManager.h"
#include "FolderManager.h"
#include "CSVFileManager.h"
#include "TXTFileManager.h"
using namespace std;

class Branch {
	CSVFileManager fileReader;
	Tree<String>* tree1;
	FolderManager folderManager;
	fs::path branchName;

	int hashType;

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
			tree1->insert(key, data, hashType);
		}
		cout << "Inorder traversal of AVL Tree: " << endl;
		tree1->inorder();
		tree1->deleteTree();
		cout << endl;
		tree1->inorder();
	}

public:
	// Branch(fs::path repoPath):folderManager(repoPath){}

	Branch(fs::path branchName, fs::path repoPath) : branchName(branchName), tree1(nullptr), folderManager(repoPath) {
		if (branchName == "main")
		{
			folderManager.create_folder(this->branchName);
			folderManager.create_folder(folderManager.get_current_path() / this->branchName / "Nodes");
			init();
			fileReader.copy_file(fileReader.getCSVPath() / fileReader.file_name, repoPath / this->branchName);
		}		
	}

	~Branch() {
		if (tree1) {
			tree1->deleteTree();
			delete tree1;       
		}
	}

	void init() {
		fileReader.openFile();
		fileReader.readFileData();
		int rowCount = fileReader.getRowCount();
		initializeTree(rowCount);
	}

	fs::path getBranchName() {
		return branchName;
	}



};