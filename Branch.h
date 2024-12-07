#pragma once
#define _CRT_SECURE_NO_WARNINGS
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
#include <ctime>
using namespace std;

class Branch {
	CSVFileManager fileReader;
	FolderManager folderManager;
	TxtFileManager commitLog;
	int commitCount = 0;
	Tree<String>* tree1;

	fs::path branchName;
	fs::path csvPath;

	String treeType;

	int hashType;


	/*
	*
	*
	 --------    Function Name: initializeTree    --------
	*
	*
	*/

	void initializeTree(int hashType, String& treeType, int rowCount)
	{
		if (treeType == "")
		{
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
		}

		else
		{
			if (treeType == "RedBlackTree")
			{
				tree1 = new RedBlackTree<String>();
			}
			else if (treeType == "AVLTree")
			{
				tree1 = new AVLTree<String>();
			}
			// Uncomment this block if BTree option is needed
			// else if (treeType == "BTree")
			// {
			// 	tree1 = new B-Tree<String>();
			// }
		}


		// CONSTRUCTING THE TREE:
		for (int i = 0; i < rowCount; i++) {
			String key, data;
			fileReader.getInformation(key, data, i);
			tree1->insert(key, data, hashType);
		}


		//cout << "Inorder traversal of AVL Tree: " << endl;
		//tree1->inorder();
	}





public:
	/*
	*
	*
	 --------    BRANCH Constructor    --------
	*
	*
	*/

	Branch(fs::path branchName, fs::path CSVFileName, int hashType, String& treeType, int& columnNo, fs::path repoPath, fs::path csvPath = "")
		: branchName(branchName), tree1(nullptr), folderManager(repoPath), treeType(treeType), hashType(hashType)
	{
		folderManager.create_folder(this->branchName);
		folderManager.create_folder(folderManager.get_current_path() / this->branchName / "Nodes");
		commitLog.createFile(folderManager.get_current_path() / this->branchName / "commitLog.txt");

		this->fileReader.setFileName(CSVFileName);
		this->fileReader.setCSVPath(csvPath);

		if (branchName == "main")
		{
			init(hashType, treeType, columnNo, csvPath);
			fileReader.copy_file(fileReader.getCSVPath(), repoPath / this->branchName);

		}
	}

	/*
	*
	*
	 --------    Function Name: init    --------
	*
	*
	*/
	void init(int hashType, String& treeType, int& columnNo, fs::path csvPath = "") {
		fileReader.openFile(csvPath);
		this->setCSVPath();
		fileReader.readFileData(columnNo);

		int rowCount = fileReader.getRowCount();

		initializeTree(hashType, treeType, rowCount);
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
		outputFile << root->id << endl;
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
		commitMsg();
		int choice;
		commitMenu(choice);
		while (choice != 4) {
			switch (choice) {
			case 1:
				addRecord();
				break;
			case 2:
				deleteRecord();
				break;
			case 3:
				editRecord();
				break;
			default:
				cout << "Invalid choice. Please enter a valid option.\n";
				break;
			}
			commitMenu(choice);
		}
	}

	// Function to add a record to the tree and update csv file
	// extract key from data based on column number added previously
	void addRecord() {
		String key, data, concatenatedData = "";
		for (int i = 0; i < fileReader.columnCount; i++) {
			cout << "Enter " << fileReader.columns_data[i] << ": ";
			cin >> data;

			// Add to concatenatedData with a comma (except for the first column)
			if (i > 0) {
				concatenatedData += ",";
			}
			concatenatedData += data;
		}

		key = fileReader.extractKey(concatenatedData); // Use concatenated data to extract the key
		tree1->insert(key, concatenatedData, hashType); // Insert record into the tree
		createNodeFile(tree1->getRoot()); // Create a node file for the new record

		fs::path branchCSVPath = folderManager.get_current_path();
		branchCSVPath += "\\";
		branchCSVPath += branchName;
		branchCSVPath += "\\";
		branchCSVPath += fileReader.getFileName();

		fileReader.appendToFile(branchCSVPath,concatenatedData); // Append concatenated data to the file
	}

	void deleteRecord() {

	}
	void editRecord() {

	}

	void commitMsg() {
		commitCount++;
		string index = intToString(commitCount);
		string commitNo = "Commit# " + index + ":";
		const char* i = commitNo.c_str();
		char* i1 = new char[commitNo.length() + 1];
		my_strcpy(i1, i);
		char* msg = new char[1000];
		cout << "Enter message to commit" << endl;
		cin.ignore();
		cin.getline(msg, 1000);

		char* timeStamp = getTimeStamp();
		// append timeStamp with msg and store it in char* data
		char* data = new char[1000];
		my_strcpy(data, timeStamp);
		my_strcat(data, msg);
		commitLog.writeFileData(folderManager.get_current_path() / this->branchName / "commitLog.txt", i1);
		commitLog.writeFileData(folderManager.get_current_path() / this->branchName / "commitLog.txt", data);
	}

	char* getTimeStamp() {
		time_t now = time(0);
		char* dt = ctime(&now);
		return dt;
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


	void commitMenu(int& choice) {
		do {
			cout << "\nMENU: ";
			cout << "\n1. Add a Record";
			cout << "\n2. Delete a Record";
			cout << "\n3. Edit a Record";
			cout << "\n4. Exit Commit Menu";
			cout << "\nEnter choice: ";
			cin >> choice;

			if (choice < 1 || choice > 4) {
				cout << "Invalid choice. Please enter a number between 1 and 4.\n";
			}
		} while (choice < 1 || choice > 4); // Validate the choice is within range
	}



	/*
	*
	*
	 --------    Function Name: Copy Branch Details    --------
	*
	*
	*/

	void CopyBranchDetails(fs::path newName, Branch& source) {
		this->branchName = newName;

		//this->folderManager.create_folder(folderManager.get_current_path() / branchName / "Nodes");
		this->folderManager = source.folderManager;
		this->folderManager.copy_folder(folderManager.get_current_path() / branchName / "Nodes", folderManager.get_current_path() / source.branchName / "Nodes");

		//this->commitLog.createFile(folderManager.get_current_path() / this->branchName / "commitLog.txt");
		this->fileReader = source.fileReader;
		this->fileReader.setCSVPath(folderManager.get_current_path() / newName / source.getFileReader().getFileName());
		fileReader.copy_file(source.getCSVPath(), folderManager.get_current_path() / this->branchName);
		this->treeType = source.treeType;
		this->hashType = source.hashType;
		this->tree1 = source.tree1->clone();
	}


	/*
	*
	*
	 --------    Getters, Setters:    --------
	*
	*
	*/
	fs::path getBranchName() {
		return branchName;
	}

	fs::path getCSVPath()
	{
		return this->csvPath;
	}

	void setCSVPath()
	{
		this->csvPath = this->fileReader.getCSVPath();
	}

	CSVFileManager& getFileReader()
	{
		return this->fileReader;
	}


	// Destructor:
	~Branch() {
		if (tree1) {
			tree1->deleteTree();
			delete tree1;
		}
	}

};