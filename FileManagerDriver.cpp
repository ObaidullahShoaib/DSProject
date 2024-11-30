#include "FileManager.h"
#include <iostream>
using namespace std;

int main()
{
    FileManager fileManager;
    int choice;

    do
    {
        cout << "\n--- File Manager Menu ---";
        cout << "\n1. Create Folder";
        cout << "\n2. Delete Folder";
        cout << "\n3. Navigate to Folder";
        cout << "\n4. List Folders";
        cout << "\n5. Copy Folder";
        cout << "\n6. Show Current Folder Path";
        cout << "\n7. Open Folder";
        cout << "\n8. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string folderName;
            cout << "\nEnter folder name to create: ";
            cin.ignore();
            getline(cin, folderName);
            fileManager.create_folder(fileManager.get_current_path(), folderName);
            break;
        }
        case 2:
        {
            string folderName;
            cout << "\nEnter folder name to delete: ";
            cin.ignore();
            getline(cin, folderName);
            fileManager.delete_folder(fileManager.get_current_path(), folderName);
            break;
        }
        case 3:
        {
            string folderName;
            cout << "\nEnter folder name to navigate to: ";
            cin.ignore();
            getline(cin, folderName);
            fileManager.nagivateToFolder(fileManager.get_current_path(), folderName);
            break;
        }
        case 4:
        {
            cout << "\nListing folders and files in the current directory:\n";
            fileManager.listFolders();
            break;
        }
        case 5:
        {
            string sourceFolder, destinationFolder;
            cout << "\nEnter the folder name to copy: ";
            cin.ignore();
            getline(cin, sourceFolder);
            cout << "\nEnter the new folder name: ";
            getline(cin, destinationFolder);
            fileManager.copyFolder(fileManager.get_current_path(), destinationFolder, sourceFolder);
            break;
        }
        case 6:
        {
            fileManager.currentFolder();
            break;
        }
        case 7:
        {
            string folderName;
            cout << "\nEnter folder name to open: ";
            cin.ignore();
            getline(cin, folderName);
            fileManager.openFolder(fileManager.get_current_path(), folderName);
            cout << "\nFolder opened successfully.";
            break;
        }
        case 8:
        {
            cout << "\nExiting program.\n";
            break;
        }
        default:
            cout << "\nInvalid choice. Please try again.";
            break;
        }
    } while (choice != 8);

    return 0;
}
