#pragma once
#include "Repository.h"
#include <filesystem>
class RepoManager {
	Repository* activeRepo;
	Repository** allRepos;
	
	TxtFileManager repoMetadata;
	FolderManager folderManager;

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
		----- SAVING AND LOADING REPOSITORY-----
	*/

	// to save current/active repository state to txt file:
	void save() {
		Repository* temp = activeRepo;
		repoMetadata.SaveRepoToTxt(*temp, temp->getFolderManager().get_current_path());
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
		repoMetadata.LoadRepoFromTxt(*newRepo, newRepo->getFolderManager().get_current_path());
	}


	Repository* findRepo(String name) {
		for (int i = 0; i < repoCount; i++) {
			if (this->allRepos[i]->getName() == name)
				return this->allRepos[i];			
		}
		return nullptr;
	}

	~RepoManager() {
		for (int i = 0; i < repoCount; i++) {
			delete allRepos[i];
		}
		delete[] allRepos;
	}
};