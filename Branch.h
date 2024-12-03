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
	TxtFileManager commitLog;
	String treeType;

	int hashType;

	void initializeTree(int rowCount) {
		int choice;
		do {
			cout << "Enter 1 for Red Black Tree\nEnter 2 for AVL Tree\nEnter 3 for B-Tree\nChoice: ";
			cin >> choice;

			if (choice == 1) {
				tree1 = new RedBlackTree<String>();
				treeType = "RedBlackTree";
				break; // Exit the loop
			}
			else if (choice == 2) {
				tree1 = new AVLTree<String>();
				treeType = "AVLTree";
				break; // Exit the loop
			}
			// Uncomment this block if BTree option is needed
			// else if (choice == 3) {
			//     tree1 = new B-Tree<String>();
			//	   treeType = "BTree";
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
	}

public:
	// Branch(fs::path repoPath):folderManager(repoPath){}
	fs::path getBranchName() {
		return this->branchName;
	}

	Branch(fs::path branchName, fs::path repoPath) : branchName(branchName), tree1(nullptr), folderManager(repoPath) {
		folderManager.create_folder(this->branchName);
		folderManager.create_folder(folderManager.get_current_path() / this->branchName / "Nodes");
		commitLog.createFile(folderManager.get_current_path() / this->branchName / "commitLog.txt");
		if (branchName == "main")
		{
			fs::path tempPath;
			init(tempPath);
			fileReader.copy_file(fileReader.getCSVPath() / fileReader.file_name, repoPath / this->branchName);
		}
	}

	fs::path getCSVPath()
	{
		return fileReader.getCSVPath() / fileReader.file_name;
	}

	void CopyBranchDetails(fs::path newName, Branch& source) {
		this->branchName = newName;
		//this->folderManager.create_folder(folderManager.get_current_path() / branchName / "Nodes");
		this->folderManager.copy_folder(folderManager.get_current_path() / branchName / "Nodes", folderManager.get_current_path() / source.branchName / "Nodes");
		//this->commitLog.createFile(folderManager.get_current_path() / this->branchName / "commitLog.txt");
		fileReader.copy_file(source.getCSVPath(), folderManager.get_current_path() / this->branchName);
		this->treeType = source.treeType;
		this->hashType = source.hashType;
		this->tree1 = source.tree1->clone();

	}

	~Branch() {
		if (tree1) {
			tree1->deleteTree();
			delete tree1;
		}
	}

	void init(fs::path sourceFile, bool openExisting = false) {
		if (!openExisting)
			fileReader.openFile();
		else
			fileReader.openExistingFile(sourceFile);

		fileReader.readFileData();
		int rowCount = fileReader.getRowCount();
		initializeTree(rowCount);
		createNodeFile(tree1->getRoot());
	}


	void createNodeFile(TreeNode<String>* root) {
		if (root == nullptr) return;
		createNodeFile(root->getChild(0));
		//////////////////////////////////////////////////////////////////////////////
		fs::path fileName = folderManager.get_current_path() / this->branchName / "Nodes" / root->nodeName;

		ofstream outputFile;
		outputFile.open(fileName);
		if (!outputFile)
		{
			outputFile.close();
			cout << "Error creating file: " << fileName << endl;
		}
		else
		{
			cout << "File created successfully: " << fileName << endl;
		}
		// write data to file
		outputFile << root->key << endl;
		outputFile << root->data << endl;
		outputFile << root->numOfChildren << endl;
		for (int i = 0; i < root->numOfChildren; i++) {
			if (root->getChild(i) != nullptr)
				outputFile << root->getChild(i)->nodeName.string() << endl;
			else {
				outputFile << "Nullptr" << endl;
			}
		}
		if (root->getParent() != nullptr)
			outputFile << root->getParent()->nodeName.string() << endl;
		else {
			outputFile << "Nullptr" << endl;
		}
		outputFile.close();
		/////////////////////////////////////////////////////////////////////////////////
		createNodeFile(root->getChild(1));
	}

	void commit() {
		char* msg = new char[1000];
		cout << "Enter message to commit" << endl;
		cin.ignore();
		cin.getline(msg, 1000);
		commitLog.writeFileData(folderManager.get_current_path() / this->branchName / "commitLog.txt", msg);
	}

	void save() {
		ofstream branchMetaData = ofstream(folderManager.get_current_path() / branchName / "BranchMetaData.txt");
		if (branchMetaData.is_open())
		{
			branchMetaData << this->branchName.string() << endl;
			branchMetaData << this->treeType << endl;
			branchMetaData << this->hashType << endl;
			//branchMetaData << this->commitLog.file_name << endl;
			branchMetaData << this->tree1->getRoot()->nodeName.string() << endl;
			branchMetaData.close();

		}
		else
		{
			cout << "Error: Could not open the file for writing." << endl;
		}
	}

	void showCommits() {
		commitLog.displayFileData();
	}

};