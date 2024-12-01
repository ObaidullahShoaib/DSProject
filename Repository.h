#pragma once
#include "Branch.h"
#include <iostream>

class Repository {
private:
    Branch* activeBranch;     // Currently active branch
    Branch** allBranches;      // Dynamic array of branches
    int branchCount;          // Number of branches
	String name;
	FolderManager folderManager;

public:
    // Default Constructor
    Repository(fs::path repoName, fs::path repoPath) : branchCount(1), folderManager(repoPath) {
		this->name = repoName.string().c_str();
		this->allBranches = new Branch * [branchCount];
		this->allBranches[0] = new Branch("main", repoPath);
        this->activeBranch = this->allBranches[0];
		this->name = name;
    }

	String getName() const {
		return this->name;
	}

	void createBranch(fs::path branchName) {
		Branch** temp = new Branch * [branchCount + 1];
        for (int i = 0; i < branchCount; i++) {
			temp[i] = allBranches[i];
		}
		temp[branchCount] = new Branch(branchName, folderManager.get_current_path());
		delete[] allBranches;
		allBranches = temp;
		folderManager.create_folder(folderManager.get_current_path() / branchName);
		branchCount++;
	}

	void checkout(fs::path branchName) {
		// check if branch with bracnhName exists
		// set activeBranch to branchName and navigate into the folder of enetrd branchName
		for (int i = 0; i < branchCount; i++) {
			if (allBranches[i]->getBranchName() == branchName) {
				activeBranch = allBranches[i];
				fs::current_path(folderManager.get_current_path() / activeBranch->getBranchName());
				cout << "Switched to branch: " << fs::current_path().string() << endl;
				return;
			}
		}
	}

    // Destructor
    ~Repository() {
        delete[] allBranches;
    }

};
