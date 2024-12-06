#pragma once
#include "Branch.h"
//#include "TXTFileManager.h"
#include <iostream>

class Repository {
private:
	String name;
	Branch* activeBranch;     // Currently active branch
	int branchCount;          // Number of branches
    Branch** allBranches;      // Dynamic array of branches
    
	FolderManager folderManager;

public:
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

    Repository(fs::path repoName, fs::path repoPath) : branchCount(1), folderManager(repoPath) {
		this->name = repoName.string().c_str();
		this->allBranches = new Branch * [branchCount];
		this->allBranches[0] = new Branch("main", repoPath);
        this->activeBranch = this->allBranches[0];
		this->name = name;
    }

	Branch* findBranch(fs::path branchName) {
		for (int i = 0; i < branchCount; i++) {
			if (allBranches[i]->getBranchName() == branchName) {
				return allBranches[i];
			}
		}
		return nullptr;
	}

	void createBranch(fs::path branchName) {
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
			temp[branchCount] = new Branch(branchName, folderManager.get_current_path());			
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
		// folderManager.copy_folder(folderManager.get_current_path() / branchName, folderManager.get_current_path() / sourceBranchName);
	}

	void addBranch(fs::path branchName, int index, bool branchCountSet = false)
	{
		Branch** temp = !branchCountSet ? new Branch * [branchCount + 1]: new Branch *[index + 1];
		for (int i = 0; i < index; i++) {
			temp[i] = allBranches[i];
		}

		temp[index] = new Branch(branchName, folderManager.get_current_path());
		delete[] allBranches;
		allBranches = temp;
		folderManager.create_folder(folderManager.get_current_path() / branchName);
		
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

    // Destructor
    ~Repository() {
        delete[] allBranches;
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

	void log() {
		activeBranch->showCommits();
	}



};
