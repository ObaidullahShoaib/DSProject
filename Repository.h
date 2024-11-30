#pragma once
#include <iostream>
#include <fstream>
#include "String.h"
#include <sstream>
#include "AVLTree.h"
#include "RedBlackTree.h"
#include "FileReader.h"
using namespace std;

class Repository {
	FileReader fileReader;
	Tree<String>* tree1;

	void initlizeTree(int rowCount) {
		int choice;
		do {
			cout << "Enter 1 for Red Black Tree\nEnter 2 for AVL Tree\nEnter 3 for BST\nChoice: ";
			cin >> choice;

			if (choice == 1) {
				tree1 = new RedBlackTree<String>();
				break; // Exit the loop
			}
			else if (choice == 2) {
				tree1 = new AVLTree<String>();
				break; // Exit the loop
			}
			// Uncomment this block if BST option is needed
			// else if (choice == 3) {
			//     tree1 = new BST<String>();
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
	Repository() : tree1(nullptr) {}
	~Repository() { delete tree1; }

	void init() {
		fileReader.openFile();
		fileReader.readFile();
		int rowCount = fileReader.getRowCount();
		initlizeTree(rowCount);
	}
};