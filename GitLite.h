#pragma once
#include"RepoManager.h"

class GitLite {
	RepoManager manager;
	FolderManager foldermanager;
public:
	GitLite():foldermanager(fs::current_path()), manager(fs::current_path() / "GitLite") {
		fs::path GitLitepath = fs::current_path();
		foldermanager.create_folder("GitLite");
	}
	void run() {
		String input;
		while (true) {
			cout << "-----> "; // Prompt
			cin >> input;

			if (input == "init") {
				cout << "Enter Repository Name: ";
				string name;
				cin >> name;
				fs::path repoName = name;
				manager.createRepo(name); // Initialize repository
			}
			else if (input == "branch") {
				fs::path branchName;
				cin >> branchName;
				manager.createBranch(branchName);
			}
			else if (input == "checkout") {
				fs::path branchName;
				cin >> branchName;
				manager.checkout(branchName);
			}
			else if (input == "save")
			{
				manager.save();
			}
			else if (input == "load")
			{
				char* repoName = new char[100];
				cin >> repoName;
				manager.load(repoName);
			}
			else if (input == "exit") {
				break; // Exit the program
			}
			else {
				cout << "Invalid command. Please try again.\n";
			}

		}
	 }
};