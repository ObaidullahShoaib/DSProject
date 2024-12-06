#pragma once
class Repository;
#include "Repository.h"
#include"TxtFileManager.h"
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

class RepoManager {
	Repository* activeRepo;
	Repository** allRepos;

	TxtFileManager repoMetadata;
	FolderManager folderManager;
	fs::path baseGitLitePath;
	int hashType;
	int repoCount;

public:
	RepoManager(fs::path GitLitePath) :folderManager(GitLitePath) {
		this->baseGitLitePath = GitLitePath;
		activeRepo = nullptr;
		allRepos = nullptr;
		repoCount = 0;
	}
	void createRepo(fs::path name) {
		if (exists(folderManager.get_current_path() / name)) {
			cout << "Repository already exists" << endl;
			return;
		}
		Repository** temp = new Repository * [repoCount + 1];
		for (int i = 0; i < repoCount; i++) {
			temp[i] = allRepos[i];
		}
		folderManager.create_folder(name);
		temp[repoCount] = new Repository(name, folderManager.get_current_path() / name);
		delete[] allRepos;
		allRepos = temp;
		this->activeRepo = allRepos[repoCount];
		this->repoCount++;
	}
	void setActiveRepo(fs::path pathName) {
		String name = pathName.string().c_str();
		for (int i = 0; i < repoCount; i++) {
			if (allRepos[i]->getName() == name) {
				activeRepo = allRepos[i];
				this->activeRepo->getCSVPath();
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
		this->repoMetadata.outputFile << activeRepo->getFolderManager().get_current_path().string() << endl;
		this->repoMetadata.outputFile << activeRepo->getCSVPath().string() << endl;	// storing csv path
		this->repoMetadata.outputFile << activeRepo->getTreeType() << endl;
		this->repoMetadata.outputFile << activeRepo->getColumnNo() << endl;
		//this->repoMetadata.outputFile << activeRepo->getHashType() << endl;
		this->repoMetadata.outputFile << activeRepo->getBranchCount() << endl;
		for (int i = 0; i < activeRepo->getBranchCount(); i++)
		{
			this->repoMetadata.outputFile << activeRepo->getAllBranches()[i]->getBranchName().string() << endl;
		}
		this->repoMetadata.outputFile << activeRepo->getActiveBranch()->getBranchName().string() << endl;
		this->repoMetadata.outputFile.close();


		// copying repository to a new folder
		fs::path newPath = temp->getName().c_str();
		newPath += "_copy";
		folderManager.copy_folder(folderManager.get_current_path() / newPath, folderManager.get_current_path() / temp->getName().c_str(), true);
	}

	// delete repo
	void deleteRepo(fs::path pathName)
	{
		String name = pathName.string().c_str();
		for (int i = 0; i < repoCount; i++)
		{
			if (allRepos[i]->getName() == name)
			{

				folderManager.delete_folder(allRepos[i]->getFolderManager().get_current_path());
				delete allRepos[i];
				for (int j = i; j < repoCount - 1; j++)
				{
					allRepos[j] = allRepos[j + 1];
				}

				repoCount--;
				return;
			}
		}

		cout << "\nRepository not found" << endl;
	}

	// to load user's chosen repository from txt file:
	void load(char* txtPathName)
	{
		char* repoName = new char[my_strlen(txtPathName) + 1];

		int i;
		for (i = 0; i < my_strlen(txtPathName) - 1 && txtPathName[i] != '_'; i++)
		{
			repoName[i] = txtPathName[i];
		}
		repoName[i] = '\0';

		// check if any folder exist with repoName or not
		if (!fs::exists(folderManager.get_current_path() / repoName))
		{
			cout << "\nRepository not found" << endl;
			return;
		}


		// delete existing repository 
		deleteRepo(repoName);
		fs::path pathToCreate = folderManager.get_current_path();
		fs::current_path(pathToCreate);
		folderManager.create_folder(repoName);


		fs::path repoPath = repoName;
		repoPath += "_data.txt";

		fs::path repoCopyFolderName = repoName;
		repoCopyFolderName += "_copy";

		// extracting path of metadata file from the old state
		pathToCreate = this->baseGitLitePath / repoCopyFolderName / repoPath;

		char* tempRepoName = new char[256];
		char* tempRepoPath = new char[256];

		// load data from file
		this->repoMetadata.inputFile = ifstream(pathToCreate);
		this->repoMetadata.inputFile;
		if (!this->repoMetadata.inputFile.is_open())
		{
			cout << "Error: Could not open the file for reading." << endl;
			return;
		}
		char* line = new char[256];

		// read data from file
		this->repoMetadata.inputFile.getline(line, 256);
		my_strcpy(tempRepoName, line);

		this->repoMetadata.inputFile.getline(line, 256);
		my_strcpy(tempRepoPath, line);

		this->repoMetadata.inputFile.getline(line, 256);
		fs::path activeCSVPath = line;

		this->repoMetadata.inputFile.getline(line, 256);
		String treeType = line;

		this->repoMetadata.inputFile.getline(line, 256);
		int columnNo = atoi(line);

		Repository* newRepo = new Repository(repoName, folderManager.get_current_path() / repoName, activeCSVPath, treeType, columnNo);
		this->activeRepo = newRepo;

		newRepo->setName(tempRepoName);
		// newRepo->setFolderManager(new FolderManager(tempRepoPath));

		this->repoMetadata.inputFile.getline(line, 256);
		newRepo->setBranchCount(atoi(line));

		// wasting 1 line:
		this->repoMetadata.inputFile.getline(line, 256);

		// copying all branches other than main (main auto-constructed in Repo Constructor)
		for (int i = 1; i < newRepo->getBranchCount(); i++)
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

		// close input file:
		this->repoMetadata.inputFile.close();

		// copy input file to newRepo:
		fs::path source = this->baseGitLitePath / repoCopyFolderName / txtPathName;
		fs::path dest = newRepo->getFolderManager().get_current_path() / txtPathName;
		newRepo->getActiveBranch()->getFileReader().copy_file(source, dest);

		if (this->repoMetadata.inputFile.is_open())
			this->repoMetadata.inputFile.close();
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

	void commit() {
		activeRepo->commit();
	}
	void saveMetaData() {
		ofstream RepositoryMetaData = ofstream(folderManager.get_current_path() / "RepositoryMetaData.txt");
		if (RepositoryMetaData.is_open())
		{
			RepositoryMetaData << activeRepo->getName() << endl;
			RepositoryMetaData << this->repoCount << endl;
			for (int i = 0; i < this->repoCount; i++) {
				RepositoryMetaData << allRepos[i]->getName() << endl;
			}
			RepositoryMetaData.close();
			for (int i = 0; i < this->repoCount; i++) {
				allRepos[i]->save();
			}
		}
		else
		{
			cout << "Error: Could not open the file for writing." << endl;
		}
	}
	void log() {
		this->activeRepo->log();
	}

	~RepoManager() {
		for (int i = 0; i < repoCount; i++) {
			delete allRepos[i];
		}
		delete[] allRepos;
	}
};