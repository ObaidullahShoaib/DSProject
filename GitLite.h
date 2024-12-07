#pragma once
#include"RepoManager.h"

class GitLite {
	RepoManager manager;
	FolderManager foldermanager;
	TxtFileManager GitLiteMetadata;

public:
	GitLite() :foldermanager(fs::current_path()), manager(fs::current_path() / "GitLite") {
		fs::path GitLitepath = fs::current_path() / "GitLite";
		foldermanager.create_folder("GitLite");
		this->run();	
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
			else if (input == "commit") {
				manager.commit();
			}
			else if (input == "log") {
				cout << "Checkout Log:" << endl;
				manager.log();
			}
			else if (input == "delete")
			{
				char* repoName = new char[100];
				cout << "Enter Repository Name: ";
				cin >> repoName;
				manager.deleteRepo(repoName);
			}
			else if (input == "cls")
			{
				system("cls");
			}
			else if (input == "exit") {
				cout << "Exiting the program!" << endl;
				cout << "Do you want to save the changes? (Y/N): ";
				char choice;
				do {
					cin >> choice;
					if (choice == 'Y' || choice == 'y') {
						manager.saveMetaData();
						cout << "Changes saved." << endl;
						break; // Exit the loop
					}
					else if (choice == 'N' || choice == 'n') {
						foldermanager.delete_folder(foldermanager.get_current_path() / "GitLite");
						cout << "Changes not saved." << endl;
						break; // Exit the loop
					}
					else {
						cout << "Invalid choice. Please enter 'Y' or 'N': ";
					}
				} while (true);
				break; // Exit the program
			}
			else {
				cout << "Invalid command. Please try again.\n";
			}

		}
	}
};