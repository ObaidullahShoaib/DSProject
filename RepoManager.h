#pragma once
class Repository;
#include "Repository.h"
#include"TXTFileManager.h"
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

class RepoManager {
	Repository* activeRepo;
	Repository** allRepos;
	
	TxtFileManager repoMetadata;
	FolderManager folderManager;

	int hashType;
	int repoCount;

public:
	RepoManager(fs::path GitLitePath):folderManager(GitLitePath) {
		activeRepo = nullptr;
		allRepos = nullptr;
		repoCount = 0;
	}
	void createRepo(fs::path name) {
		Repository** temp = new Repository * [repoCount + 1];
		for (int i = 0; i < repoCount; i++) {
			temp[i] = allRepos[i];
		}
		folderManager.create_folder(name);
		temp[repoCount] = new Repository(name, folderManager.get_current_path() / name);
		delete[] allRepos;
		allRepos = temp;
		this->activeRepo = allRepos[repoCount];
		
	}
	void setActiveRepo(fs::path pathName) {
		String name = pathName.string().c_str();
		for (int i = 0; i < repoCount; i++) {
			if (allRepos[i]->getName() == name) {
				activeRepo = allRepos[i];
				folderManager.open_folder(folderManager.get_current_path() / pathName);
				return;
			}
		}
		cout << "Repository not found" << endl;
	}

	void checkout(fs::path commitID) {
		if (activeRepo == nullptr) {
			cout << "No active repository" << endl;
			return;
		}
		activeRepo->checkout(commitID);
	}

	void createBranch(fs::path branchName) {
		if (activeRepo != nullptr)
			activeRepo->createBranch(branchName);
		else {
			cout << "No active repository" << endl;
		}
	}

	/*
	    .
		.
		----- SAVING AND LOADING REPOSITORY-----
		.
		.
	*/


	// to save current/active repository state to txt file:
	void save() {
		Repository* temp = activeRepo;
		//repoMetadata.SaveRepoToTxt(*temp, temp->getFolderManager().get_current_path());


		// save above info in a txt file in pathtosaveto location
		string fileName = activeRepo->getName().c_str();
		fileName += "_data.txt";

		fs::path pathToSaveToFile = activeRepo->getFolderManager().get_current_path() / fileName;
		// create file
		this->repoMetadata.outputFile = ofstream(pathToSaveToFile);
		// open file
		if (!this->repoMetadata.outputFile)
		{
			cout << "Error: Could not open the file for writing." << endl;
			return;
		}
		// write data to file
		this->repoMetadata.outputFile << activeRepo->getName().c_str() << endl;
		this->repoMetadata.outputFile << activeRepo->getFolderManager().get_current_path() << endl;
		this->repoMetadata.outputFile << activeRepo->getBranchCount() << endl;
		for (int i = 0; i < activeRepo->getBranchCount(); i++)
		{
			this->repoMetadata.outputFile << activeRepo->getAllBranches()[i]->getBranchName().string() << endl;
		}
		this->repoMetadata.outputFile << activeRepo->getActiveBranch()->getBranchName().string() << endl;

		// copying repository to a new folder
		fs::path newPath = temp->getName().c_str();
		newPath += "_copy";
		folderManager.copy_folder(folderManager.get_current_path() / newPath, folderManager.get_current_path() / temp->getName().c_str(), true);
	}

	// to load user's chosen repository from txt file:
	void load(char* txtPathName)
	{
		char* temp = new char[my_strlen(txtPathName) + 1];
		
		for (int i = 0; i < my_strlen(txtPathName) - 1 && txtPathName[i + 1] != '_'; i++)
		{
			temp[i] = txtPathName[i];
		}

		Repository* newRepo = new Repository(temp, folderManager.get_current_path());
		
		// repoMetadata.LoadRepoFromTxt(*newRepo, newRepo->getFolderManager().get_current_path());




		// load data from file
		this->repoMetadata.inputFile = ifstream(newRepo->getFolderManager().get_current_path());
		if (!this->repoMetadata.inputFile)
		{
			cout << "Error: Could not open the file for reading." << endl;
			return;
		}
		char* line = new char[256];
		// read data from file
		this->repoMetadata.inputFile.getline(line, 256);
		newRepo->setName(line);

		this->repoMetadata.inputFile.getline(line, 256);
		newRepo->setFolderManager(new FolderManager(line));

		this->repoMetadata.inputFile.getline(line, 256);
		newRepo->setBranchCount(atoi(line));

		for (int i = 0; i < newRepo->getBranchCount(); i++)
		{
			this->repoMetadata.inputFile.getline(line, 256);
			newRepo->addBranch(line, i, true);
		}

		this->repoMetadata.inputFile.getline(line, 256);

		// finding the active branch
		for (int i = 0; i < newRepo->getBranchCount(); i++)
		{
			fs::path linePathed = line;
			if (newRepo->getAllBranches()[i]->getBranchName() == linePathed)
			{
				newRepo->setActiveBranch(newRepo->getAllBranches()[i]);
			}
		}
	}


	Repository* findRepo(String name) {
		for (int i = 0; i < repoCount; i++) {
			if (this->allRepos[i]->getName() == name)
				return this->allRepos[i];			
		}
		return nullptr;
	}

	void inputHashType() {
		int choice;
		int expiryCount = 0;

		do {
			if (expiryCount == 3)
			{
				cout << "Defaulting to SHA256 Hash." << endl;
				choice = 2;
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
	}

	~RepoManager() {
		for (int i = 0; i < repoCount; i++) {
			delete allRepos[i];
		}
		delete[] allRepos;
	}
};