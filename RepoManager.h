#pragma once
#include "Repository.h"
#include <filesystem>
class RepoManager {
	Repository* activeRepo;
	Repository** allRepos;
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
		activeRepo->createBranch(branchName);
	}
	~RepoManager() {
		for (int i = 0; i < repoCount; i++) {
			delete allRepos[i];
		}
		delete[] allRepos;
	}
};