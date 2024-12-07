#pragma once
#include "Branch.h"
#include "TxtFileManager.h"
#include <iostream>

class Repository {
private:
	String name;

	Branch* activeBranch;     // Currently active branch
	int branchCount;          // Number of branches
    Branch** allBranches;      // Dynamic array of branches
	
	fs::path csvPath;
	FolderManager folderManager;
	
	String treeType;
	int columnNo;
	int hashType;

public:
	// get ColumnNo:
	int getColumnNo() const {
		return this->columnNo;
	}

	// getTreeType:
	String getTreeType() const {
		return this->treeType;
	}

	// getrepo name
	String getName() const {
		return this->name;
	}

	// set repo name
	void setName(String name) {
		this->name = name;
	}

	// get active branch
	Branch* getActiveBranch() const {
		return this->activeBranch;
	}

	// set active branch
	void setActiveBranch(Branch* branch) {
		this->activeBranch = branch;
	}

	// get count
	int getBranchCount() const {
		return this->branchCount;
	}

	// set count
	void setBranchCount(int count) {
		this->branchCount = count;
	}

	// get all branches
	Branch** getAllBranches() const {
		return this->allBranches;
	}

	// get Folder Manager
	FolderManager getFolderManager() const {
		return this->folderManager;
	}

	// set Folder Manager
	void setFolderManager(FolderManager* folderManager) {
		this->folderManager = *folderManager;
	}	

	void setCSVPath(fs::path csvPath)
	{
		this->csvPath = csvPath;
	}

	fs::path getCSVPath()
	{
		return this->csvPath;
	}


    Repository(fs::path repoName, fs::path CSVFileName, fs::path repoPath,fs::path csvPath = "", String treeType = "", int columnNo = 0)
		: branchCount(1), folderManager(repoPath), treeType(treeType), columnNo(columnNo)
	{
		this->name = repoName.string().c_str();

		this->inputHashType();

		this->allBranches = new Branch * [branchCount];
		this->allBranches[0] = new Branch("main", CSVFileName, this->hashType, this->treeType, this->columnNo, repoPath, csvPath); 
        this->activeBranch = this->allBranches[0];
		this->name = name;
		this->setCSVPath(this->allBranches[0]->getCSVPath());
    }

	Branch* findBranch(fs::path branchName) {
		for (int i = 0; i < branchCount; i++) {
			if (allBranches[i]->getBranchName() == branchName) {
				return allBranches[i];
			}
		}
		return nullptr;
	}

	void createBranch(fs::path branchName, fs::path CSVFileName) {
		// Declarations:
		bool alreadyExists = this->findBranch(branchName) != nullptr;
		int expiryCount = 0;

		// Checking if branch already exists:
		if (alreadyExists)
		{
			fs::path newFolderPath = folderManager.get_current_path() / branchName;
			cout << "\n Do you want to overwrite the file?\n1. Yes.\n2. No\nChoice: ";
			int choice;
			
			do {
				cin >> choice;
				if (choice == 1)
				{
					break;
				}
				else if (choice == 2)
					return;
				else
					expiryCount++;
			} while (choice != 1 && choice != 2 && expiryCount < 3);
		}

		if (expiryCount >= 3)
		{
			cout << "\nDefaulting to not overwriting the file." << endl;
			return;
		}

		char* sourceBranchName = new char[100];
		cout << "\nEnter source branch name to copy from: ";
		cin.ignore();
		cin.getline(sourceBranchName, 100);
		cout << endl;

		if (!alreadyExists)
		{
			Branch** temp = new Branch * [branchCount + 1];
			for (int i = 0; i < branchCount; i++) {
				temp[i] = allBranches[i];
			}
			temp[branchCount] = new Branch(branchName,  CSVFileName, this->hashType, this->treeType, this->columnNo, folderManager.get_current_path());
			temp[branchCount]->CopyBranchDetails(branchName, *findBranch(sourceBranchName));
			delete[] allBranches;
			allBranches = temp;
						
			branchCount++;
		}		
		
		else
		{
			cout << "\nBranch already exists." << endl;
			folderManager.delete_folder(folderManager.get_current_path() / branchName);			
		}
		//folderManager.copy_folder(folderManager.get_current_path() / branchName, folderManager.get_current_path() / sourceBranchName);
	}

	void showBranches()
	{
		// shows branches of active repo
		for (int i = 0; i < branchCount; i++)
		{
			cout << allBranches[i]->getBranchName() << endl;
		}
		
	}

	void deleteBranch(fs::path branchName)
	{
		// delete branch
		Branch** temp = new Branch * [branchCount - 1];
		int index = 0;
		for (int i = 0; i < branchCount; i++) {
			if (allBranches[i]->getBranchName() == branchName)
			{
				delete allBranches[i];
			}
			else
			{
				temp[index] = allBranches[i];
				index++;
			}
		}
		delete[] allBranches;
		allBranches = temp;
		branchCount--;
	}

	void getCurrentBranch()
	{
		cout << "\nCurrent Branch: " << activeBranch->getBranchName() << endl;
	}

	void addBranch(fs::path branchName, fs::path CSVFileName, int index, bool branchCountSet = false)
	{
		Branch** temp = !branchCountSet ? new Branch * [branchCount + 1]: new Branch *[index + 1];
		for (int i = 0; i < index; i++) {
			temp[i] = allBranches[i];
		}

		temp[index] = new Branch(branchName, CSVFileName, this->hashType, this->treeType, this->columnNo, folderManager.get_current_path());
		delete[] allBranches;
		allBranches = temp;
		fs::path source = folderManager.get_current_path();
		source += "_copy";
		source = source / branchName;
		folderManager.copy_folder(folderManager.get_current_path() / branchName, source);
		
		if (!branchCountSet)
			branchCount++;
	}

	void checkout(fs::path branchName) {
		// check if branch with bracnhName exists
		// set activeBranch to branchName and navigate into the folder of enetrd branchName
		for (int i = 0; i < branchCount; i++) {
			if (allBranches[i]->getBranchName() == branchName) {
				activeBranch = allBranches[i];
				//fs::current_path(folderManager.get_current_path() / activeBranch->getBranchName());
				cout << "Switched to branch: " << fs::current_path().string() << endl;
				return;
			}
		}

		cout << "Branch not found" << endl;
	}


	void commit() {
		activeBranch->commit();
	}
	void save() {
		ofstream metaData = ofstream(folderManager.get_current_path() / "MetaData.txt");
		if (metaData.is_open())
		{
			metaData << this->name << endl;
			metaData << this->branchCount << endl;
			metaData << this->activeBranch->getBranchName().string() << endl;
			for (int i = 0; i < branchCount; i++)
			{
				metaData << allBranches[i]->getBranchName().string() << endl;
			}
			metaData.close();
			for (int i = 0; i < branchCount; i++)
			{
				allBranches[i]->save();
			}
		}
		else
		{
			cout << "Error: Could not open the file for writing." << endl;
		}
	}


	/*
*
*
 --------    INPUT HASH TYPE	--------
*
*
*/
	bool inputHashType() {
		int choice;
		int expiryCount = 0;
		bool success = true;

		do {
			if (expiryCount == 3)
			{
				cout << "Defaulting to SHA256 Hash." << endl;
				choice = 2;
				success = false;
			}
			else
			{
				cout << "Which Hashing method do you want to use?" << endl << "1. Instructor Hash" << endl << "2. SHA256 Hash" << endl << "Choice: ";
				cin >> choice;
			}

			if (choice == 1 || choice == 2)
				this->hashType = choice;
			else
				cout << "Invalid Choice." << endl;

			expiryCount++;
		} while (choice != 1 && choice != 2);

		return success;
	}


	void log() {
		activeBranch->showCommits();
	}



    // Destructor
    ~Repository() {
        delete[] allBranches;
    }

};
