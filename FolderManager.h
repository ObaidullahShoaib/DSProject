#pragma once
class Repository;
#include <iostream>
#include <filesystem>
#include "String.h"

using namespace std;

namespace fs = std::filesystem;

class FolderManager
{
private:
    fs::path current_path;

public:
    FolderManager(fs::path current_path)
    {
        this->current_path = current_path;
    }

    void create_folder(const fs::path& folder_name)
    {
        fs::path newFolderPath = current_path / folder_name;
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
    }

    void open_folder(const fs::path& folder_name)
    {
        fs::path folderPath = current_path / folder_name;
        if (fs::exists(folderPath) && fs::is_directory(folderPath))
        {
            fs::current_path(folderPath);
            current_path = fs::current_path();
            cout << "\nOpened folder: " << folderPath << endl;
        }
        else
        {
            cout << "\nFolder does not exist: " << folderPath << endl;
        }
    }

    void delete_folder(const fs::path& folder_name)
    {
        fs::path folderPath = current_path / folder_name;
        if (!fs::exists(folderPath))
        {
            cout << "\nDirectory does not exist: " << folderPath << std::endl;
        }
        else
        {
            if (fs::remove_all(folderPath))
            {
                cout << "\nDirectory deleted: " << folderPath << endl;
            }
            else
            {
                cout << "\nFailed to delete directory: " << folderPath << endl;
            }
        }
    }

    void navigate_to_folder(const fs::path& folder_name)
    {
        fs::path newFolderPath = current_path / folder_name;
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
    }

    // List files in the current directory
    void list_files() const
    {
        for (const auto& entry : fs::directory_iterator(current_path))
        {
            if (fs::is_regular_file(entry.path()))
            {
                cout << entry.path().filename() << endl;
            }
        }
    }

    // List folders in the current directory
    void list_folders() const
    {
        for (const auto& entry : fs::directory_iterator(current_path))
        {
            if (fs::is_directory(entry.path()))
            {
                cout << entry.path().filename() << endl;
            }
        }
        list_files();
    }

    // Copy folder
    void copy_folder(const fs::path& new_folder_name, const fs::path& folder_name, bool forceCopy = false)
    {
        fs::path newFolderPath = current_path / new_folder_name;
        if (forceCopy)
        {
            delete_folder(new_folder_name);
            create_folder(new_folder_name);
            fs::path source = current_path / folder_name;
            fs::copy(source, newFolderPath, fs::copy_options::overwrite_existing);
            return;
        }

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
            delete_folder(new_folder_name);
        }
        fs::copy(current_path / folder_name, newFolderPath, fs::copy_options::recursive);
    }

    // Current directory path
    void current_folder() const
    {
        cout << "\nCurrent path: " << current_path << endl;
    }

    fs::path get_current_path() const
    {
        return current_path;
    }

    void set_current_path(const fs::path& path)
    {
        fs::current_path(path);
        current_path = fs::current_path();
    }
};