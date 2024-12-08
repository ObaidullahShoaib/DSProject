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
	int repoCount;

public:
	RepoManager(fs::path GitLitePath):folderManager(GitLitePath) {
		this->baseGitLitePath = GitLitePath;
		activeRepo = nullptr;
		allRepos = nullptr;
		repoCount = 0;
	}
	Repository* createRepo(fs::path name, fs::path CSVFileName, fs::path activeCSVPath = "", String treeType = "", int columnNo = 0, int hashType = 0) 
	{

		Repository** temp = new Repository * [repoCount + 1];
		for (int i = 0; i < repoCount; i++) {
			temp[i] = allRepos[i];
		}
		folderManager.create_folder(name);
		temp[repoCount] = new Repository(name, CSVFileName, folderManager.get_current_path() / name, activeCSVPath, treeType, columnNo, hashType);
		delete[] allRepos;
		allRepos = temp;
		this->activeRepo = allRepos[repoCount];
		this->repoCount++;
		return allRepos[repoCount - 1];
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

	void createBranch(fs::path branchName, fs::path CSVFileName) {
		if (activeRepo != nullptr)
			activeRepo->createBranch(branchName, CSVFileName);
		else {
			cout << "No active repository" << endl;
		}
	}

	void showBranches()
	{
		if (activeRepo != nullptr)
			activeRepo->showBranches();
		else {
			cout << "No active repository" << endl;
		}
	}

	void deleteBranch(fs::path branchName) 
	{
		if (activeRepo != nullptr)
			activeRepo->deleteBranch(branchName);
		else {
			cout << "No active repository" << endl;
		}
	}

	void getCurrentBranch()
	{
		if (activeRepo != nullptr)
			activeRepo->getCurrentBranch();
		else {
			cout << "No active repository" << endl;
		}
	}

	/*
	    .
		.
		----- SAVING AND LOADING REPOSITORY	-----
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
		this->repoMetadata.outputFile << activeRepo->getHashType() << endl;
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

		fs::path repoCopyName = repoName;
		repoCopyName += "_copy";

		// check if any folder exist with repoName or not
		if (!fs::exists(folderManager.get_current_path() / repoCopyName))
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

		// repo name
		this->repoMetadata.inputFile.getline(line, 256);
		my_strcpy(tempRepoName, line);

		// repo path
		this->repoMetadata.inputFile.getline(line, 256);
		my_strcpy(tempRepoPath, line);

		// active csv path
		this->repoMetadata.inputFile.getline(line, 256);
		fs::path activeCSVPath = line;

		// tree type
		this->repoMetadata.inputFile.getline(line, 256);
		String treeType = line;

		// column no
		this->repoMetadata.inputFile.getline(line, 256);
		int columnNo = atoi(line);

		// hash type
		this->repoMetadata.inputFile.getline(line, 256);
		int hashType = atoi(line);

		// creating new repository (from loaded data):
		String CSVFileName = extractCSVFileName(activeCSVPath);
		fs::path CSVFileNamePathed = CSVFileName.c_str();
		Repository* newRepo = this->createRepo(repoName, CSVFileNamePathed, activeCSVPath, treeType, columnNo, hashType);
		this->activeRepo = newRepo;

		newRepo->setName(tempRepoName);

		// Branch Count:
		this->repoMetadata.inputFile.getline(line, 256);
		newRepo->setBranchCount(atoi(line));
		
		// wasting 1 line that stores the main branch name (since main branch is alredy created in Repository Constructor):
		this->repoMetadata.inputFile.getline(line, 256);

		// copying all branches other than main (main auto-constructed in Repo Constructor)
		for (int i = 1; i < newRepo->getBranchCount(); i++)
		{
			this->repoMetadata.inputFile.getline(line, 256);
			newRepo->addBranch(line, CSVFileNamePathed, i, true);
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


	/*
	*
	*
	--------    Other Functions 	--------
	*
	*
	*/
	Repository* findRepo(String name) {
		for (int i = 0; i < repoCount; i++) {
			if (this->allRepos[i]->getName() == name)
				return this->allRepos[i];			
		}
		return nullptr;
	}	


	void commit() {
		activeRepo->commit();
	}
	void saveMetaData() {
		for (int i = 0; i < repoCount; i++) {
			Repository* temp = allRepos[i];
			//repoMetadata.SaveRepoToTxt(*temp, temp->getFolderManager().get_current_path());


			// save above info in a txt file in pathtosaveto location
			string fileName = temp->getName().c_str();
			fileName += "_data.txt";

			fs::path pathToSaveToFile = temp->getFolderManager().get_current_path() / fileName;
			// create file
			this->repoMetadata.outputFile = ofstream(pathToSaveToFile);
			// open file
			if (!this->repoMetadata.outputFile)
			{
				cout << "Error: Could not open the file for writing." << endl;
				return;
			}
			// write data to file
			this->repoMetadata.outputFile << temp->getName().c_str() << endl;
			this->repoMetadata.outputFile << temp->getFolderManager().get_current_path().string() << endl;
			this->repoMetadata.outputFile << temp->getCSVPath().string() << endl;	// storing csv path
			this->repoMetadata.outputFile << temp->getTreeType() << endl;
			this->repoMetadata.outputFile << temp->getColumnNo() << endl;
			//this->repoMetadata.outputFile << activeRepo->getHashType() << endl;
			this->repoMetadata.outputFile << temp->getBranchCount() << endl;
			for (int i = 0; i < temp->getBranchCount(); i++)
			{
				this->repoMetadata.outputFile << temp->getAllBranches()[i]->getBranchName().string() << endl;
			}
			this->repoMetadata.outputFile << temp->getActiveBranch()->getBranchName().string() << endl;
			this->repoMetadata.outputFile.close();


			// copying repository to a new folder
			fs::path newPath = temp->getName().c_str();
			newPath += "_copy";
			folderManager.copy_folder(folderManager.get_current_path() / newPath, folderManager.get_current_path() / temp->getName().c_str(), true);
		}
	}
	void log() 
	{
		fs::path commitLogPath = folderManager.get_current_path() / this->activeRepo->getName().c_str() / this->activeRepo->getActiveBranch()->getBranchName().c_str() /"commitLog.txt";
		this->activeRepo->log(commitLogPath);
	}

	~RepoManager() {
		for (int i = 0; i < repoCount; i++) {
			delete allRepos[i];
		}
		delete[] allRepos;
	}
};