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
			fs::path CSVFileName;

			if (input == "init")
			{
				CSVFileName = this->openFile();
				cout << "Enter Repository Name: ";
				string name;
				cin >> name;
				fs::path repoName = name;
				manager.createRepo(name, CSVFileName, foldermanager.get_current_path() / CSVFileName); // Initialize repository
			}
			else if (input == "branch") {
				fs::path branchName;
				cin >> branchName;
				manager.createBranch(branchName, CSVFileName);
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

	fs::path openFile(fs::path CSVPath = "")
	{
		// Declarations:
		ifstream inputFile;
		char* file_name = new char[1000];

		// Opening File:
		if (CSVPath == "")
		{
			int c = 1;
			do
			{
				cout << "Input the name of the file with extension: "; // example.csv                
				// reading filename into char* then converting to fs::path

				if (c == 1)
					cin.ignore();


				cin.getline(file_name, 1000);
				c++;

				inputFile.open(CSVPath != "" ? CSVPath : file_name);

			} while (!inputFile.is_open());
			if (!inputFile.is_open())
				cout << "Error: Could not open the file. Please try again." << endl;
			else
			{
				CSVPath = file_name;
				cout << "File opened successfully!" << endl;
			}
			return CSVPath;
		}

		if (inputFile.is_open())
			inputFile.close();
		inputFile.open(CSVPath != "" ? CSVPath : file_name);

		if (!inputFile.is_open())
			cout << "Error: Could not open the file. Please try again." << endl;
		else
			cout << "File opened successfully!" << endl;

		return CSVPath;
	}

};