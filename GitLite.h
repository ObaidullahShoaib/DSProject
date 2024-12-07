#pragma once
#include"RepoManager.h"
#include<iomanip>

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
		this->showGitlite();
		this->commands();
		cout << "\nPress ENTER to continue....";
		cin.ignore();

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
			else if (input == "branches")
			{
				manager.showBranches();
			}
			else if (input == "delete-branch")
			{
				cout << "Enter Branch Name: ";
				String branchName;
				cin >> branchName;
				cout << endl;
				manager.deleteBranch(branchName.c_str());
			}
			else if (input == "current-branch")
			{
				manager.getCurrentBranch();
			}
			else if (input == "delete")
			{
				char* repoName = new char[100];
				cout << "Enter Repository Name: ";
				cin >> repoName;
				manager.deleteRepo(repoName);
			}
			else if (input == "commands")
			{
				this->commands();
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

	void showGitlite()
	{
		String* gitlite = new String[11];
		cout << endl << endl;

		int i = 0;
		gitlite[i++] = " .----------------. .----------------. .----------------.   .----------------. .----------------. .----------------. .----------------. ";
		gitlite[i++] = "| .--------------. | .--------------. | .--------------. | | .--------------. | .--------------. | .--------------. | .--------------. |";
		gitlite[i++] = "| |    ______    | | |     _____    | | |  _________   | | | |   _____      | | |     _____    | | |  _________   | | |  _________   | |";
		gitlite[i++] = "| |  .' ___  |   | | |    |_   _|   | | | |  _   _  |  | | | |  |_   _|     | | |    |_   _|   | | | |  _   _  |  | | | |_   ___  |  | |";
		gitlite[i++] = "| | / .'   \\_|   | | |      | |     | | | |_/ | | \\_|  | | | |    | |       | | |      | |     | | | |_/ | | \\_|  | | |   | |_  \\_|  | |";
		gitlite[i++] = "| | | |    ____  | | |      | |     | | |     | |      | | | |    | |   _   | | |      | |     | | |     | |      | | |   |  _ | _   | |";
		gitlite[i++] = "| | \\ `.___]  _| | | |     _| |_    | | |    _| |_     | | | |   _| |__/ |  | | |     _| |_    | | |    _| |_     | | |  _| |___/ |  | |";
		gitlite[i++] = "| |  `._____.'   | | |    |_____|   | | |   |_____|    | | | |  |________|  | | |    |_____|   | | |   |_____|    | | | |_________|  | |";
		gitlite[i++] = "| |              | | |              | | |              | | | |              | | |              | | |              | | |              | |";
		gitlite[i++] = "| '--------------' | '--------------' | '--------------' | | '--------------' | '--------------' | '--------------' | '--------------' |";
		gitlite[i++] = "'----------------' '----------------' '----------------'   '----------------' '----------------' '----------------' '----------------'";

		for (int i = 0; i < 11; i++)
			cout << gitlite[i] << endl;

		cout << endl << endl << endl;
		delete[] gitlite;
	}


	void commands() {
		using std::cout;
		using std::endl;
		using std::setw;
		using std::setfill;

		const int width = 30;
		cout << setfill('_') << "," << setw(width + 3) << "_," << setw(width + 3) << "," << endl;
		cout << setfill(' ') << "| " << setw(width / 2) << "COMMANDS" << setw(width / 2 + 2) << "|" << setw(width / 2 + 7) << "INSTRUCTIONS" << setw(width/2 - 4) << " |" << endl;
		cout << setfill('_') << "|" << setw(width + 3) << "_|" << setw(width + 3) << "|" << endl;
		
		cout << setfill(' ') << "| " << setw(width) << "init" << " | " << setw(width) << "initialize a repository" << " |" << endl;
		cout << "| " << setw(width) << "branch" << " | " << setw(width) << "create a new branch" << " |" << endl;
		cout << "| " << setw(width) << "checkout" << " | " << setw(width) << "switch between branches" << " |" << endl;
		cout << "| " << setw(width) << "save" << " | " << setw(width) << "save a repository" << " |" << endl;
		cout << "| " << setw(width) << "load" << " | " << setw(width) << "loads a repository" << " |" << endl;
		cout << "| " << setw(width) << "log" << " | " << setw(width) << "displays commit log history" << " |" << endl;
		cout << "| " << setw(width) << "commit" << " | " << setw(width) << "to edit a branch (Nodes)" << " |" << endl;
		cout << "| " << setw(width) << "merge" << " | " << setw(width) << "to merge branches" << " |" << endl;
		cout << "| " << setw(width) << "visualize-tree" << " | " << setw(width) << "to visualize tree for branch" << " |" << endl;
		cout << "| " << setw(width) << "branches" << " | " << setw(width) << "displays all branches" << " |" << endl;
		cout << "| " << setw(width) << "current-branch" << " | " << setw(width) << "displays current branch" << " |" << endl;
		cout << "| " << setw(width) << "delete-branch" << " | " << setw(width) << "deletes a branch" << " |" << endl;
		cout << "| " << setw(width) << "delete" << " | " << setw(width) << "deletes a repository" << " |" << endl;
		cout << "| " << setw(width) << "cls" << " | " << setw(width) << "clears console screen" << " |" << endl;
		cout << "| " << setw(width) << "commands" << " | " << setw(width) << "displays all commands" << " |" << endl;
		cout << "| " << setw(width) << "exit" << " | " << setw(width) << "exit program" << " |" << endl;
		cout << setfill('_') << "|" << setw(width + 3) << "_|" << setw(width + 3) << "|" << endl;

		cout << setfill(' ') << endl;
	}

};
