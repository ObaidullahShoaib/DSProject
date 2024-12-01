#include "FolderManager.h"
#include <iostream>
using namespace std;

int main()
{
    FolderManager FM;
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
            FM.create_folder(FM.get_current_path(), folderName);
            break;
        }
        case 2:
        {
            string folderName;
            cout << "\nEnter folder name to delete: ";
            cin.ignore();
            getline(cin, folderName);
            FM.delete_folder(FM.get_current_path(), folderName);
            break;
        }
        case 3:
        {
            string folderName;
            cout << "\nEnter folder name to navigate to: ";
            cin.ignore();
            getline(cin, folderName);
            FM.nagivateToFolder(FM.get_current_path(), folderName);
            break;
        }
        case 4:
        {
            cout << "\nListing folders and files in the current directory:\n";
            FM.listFolders();
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
            FM.copyFolder(FM.get_current_path(), destinationFolder, sourceFolder);
            break;
        }
        case 6:
        {
            FM.currentFolder();
            break;
        }
        case 7:
        {
            string folderName;
            cout << "\nEnter folder name to open: ";
            cin.ignore();
            getline(cin, folderName);
            FM.openFolder(FM.get_current_path(), folderName);
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
