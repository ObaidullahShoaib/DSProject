#pragma once
#include"Repository.h"
#include<iostream>
#include<filesystem>
#include"String.h"
using namespace std;

namespace fs = std::filesystem;

class FileManager
{
	fs::path current_path;

public:
	FileManager()
	{
		fs::path currentPath = fs::current_path();
		current_path = currentPath / "GitLite"; 

		// Check if the directory exists
		if (!fs::exists(current_path))
		{
			// Create the directory
			if (fs::create_directory(current_path))
			{
				fs::current_path(current_path);
				current_path = fs::current_path();
				cout << "\nDirectory created: " << current_path << endl;
			}
			else
			{
				cout << "\nFailed to create directory: " << current_path << endl;
			}
		}
		else
		{
			fs::current_path(current_path);
			current_path = fs::current_path();
			cout << "\nDirectory already exists: " << current_path << std::endl;
		}

	}

	void create_folder(fs::path Path, fs::path Folder_name)
	{
		fs::path newFolderPath = Path / Folder_name;
		if (!fs::exists(newFolderPath)) 
		{
			// Create the directory
			if (fs::create_directory(newFolderPath))
			{
				cout << "\nDirectory created: " << newFolderPath << endl;
			}
			else 
			{
				cout << "\nFailed to create directory: " << newFolderPath << endl;
				return;
			}
		}
		else 
		{
			cout << "\nDirectory already exists: " << newFolderPath << std::endl;
		}
		fs::current_path(newFolderPath);
		current_path = fs::current_path();
		return;
	}

	void delete_folder(fs::path Path, fs::path Folder_name)
	{
		fs::path newFolderPath = Path / Folder_name;
		if (!fs::exists(newFolderPath)) 
		{
			cout << "\nDirectory does not exist: " << newFolderPath << std::endl;
		}
		else 
		{
			if (fs::remove_all(newFolderPath)) 
			{
				cout << "\nDirectory deleted: " << newFolderPath << endl;
			}
			else 
			{
				cout << "\nFailed to delete directory: " << newFolderPath << endl;
			}
		}
		return;
	}

	void nagivateToFolder(fs::path Path, fs::path Folder_name)
	{
		fs::path newFolderPath = Path / Folder_name;
		if (!fs::exists(newFolderPath)) 
		{
			cout << "\nDirectory does not exist: " << newFolderPath << std::endl;
		}
		else 
		{
			fs::current_path(newFolderPath);
			current_path = fs::current_path();
			cout << "\nDirectory changed to: " << newFolderPath << endl;
		}
		return;
	}

	// list files
	void listFiles()
	{
		for (const auto& entry : fs::directory_iterator(fs::current_path()))
		{
			if (fs::is_regular_file(entry.path()))
			{
				cout << entry.path().filename() << endl;
			}
		}
		return;
	}

	// list folders
	void listFolders()
	{
		for (const auto& entry : fs::directory_iterator(current_path))
		{
			if (fs::is_directory(entry.path()))
			{
				cout << entry.path().filename() << endl;
			}
		}
		listFiles();

		return;
	}
	// copy folder
	void copyFolder(fs::path Path, fs::path newFolder_name, fs::path Folder_name)
	{

		fs::path newFolderPath = Path / newFolder_name;
		if (!fs::exists(newFolderPath)) 
		{
			// Create the directory
			if (fs::create_directory(newFolderPath))
			{
				cout << "\nDirectory created: " << newFolderPath << endl;
			}
			else 
			{
				cout << "\nFailed to create directory: " << newFolderPath << endl;
			}
		}
		else 
		{
			cout << "\nDirectory already exists: " << newFolderPath << std::endl;
		}
		fs::copy(Folder_name, newFolder_name, fs::copy_options::recursive);

		return;

	}

	// current directory path
	void currentFolder()
	{
		cout << "\nCurrent path: " << current_path << endl;
		return;
	}

	void openFolder(fs::path Path, fs::path Folder_name)
	{
		fs::path newFolderPath = Path / Folder_name;
		if (!fs::exists(newFolderPath)) 
		{
			cout << "\nDirectory does not exist: " << newFolderPath << endl;
		}
		else 
		{
			fs::current_path(newFolderPath);
			current_path = fs::current_path();
			
		}
		return;
	}

	fs::path get_current_path()
	{
		return current_path;
	}
};