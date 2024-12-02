#pragma once
#include "FileManager.h"
#include"Repository.h"

#include <iostream>
#include <fstream>
#include <filesystem>


using namespace std;

class TxtFileManager : public FileManager
{

public:
    TxtFileManager() : FileManager() {}

    void readFileData() override
    {
        // Open the file for reading
        openFile();

        // Read the file data
        string line;
        while (getline(inputFile, line))
        {
            cout << line << endl;
            rowCount++; // Increment row count
        }

        // Close the file
        closeFile();
    }

    void writeFileData() override
    {
        // Open the file for writing (append mode)
        ofstream outputFile(file_name, ios::app);
        if (outputFile.is_open())
        {
            cout << "Enter data to write to the file (type 'END' on a new line to finish):" << endl;
            string line;
            while (true) 
            {
                getline(cin, line);
                if (line == "END" || line == "end" || line == "End")
                {
                    break; // Exit the loop if the user types "END"
                }
                outputFile << line << endl; // Write each line to the file
            }
            outputFile.close(); // Close the file
            cout << "Data written to the file successfully." << endl;
        }
        else
        {
            cout << "Error: Could not open the file for writing." << endl;
        }
    }

    void createFile() 
    {
        cout << "Enter the TXT file name to create: ";
        cin >> file_name;
        outputFile.open(file_name);
        if (!outputFile)
        {
            outputFile.close();
            cerr << "Error creating file: " << file_name << endl;
        }
        else 
        {
            cout << "File created successfully: " << file_name << endl;
            outputFile.close();
        }

    }

    void displayFileData() override 
    {
        // Use the readFileData() function to read and display the file data
        readFileData();
    }

    void copyFile(const std::filesystem::path& destination) 
    {
        // Use the copy_file() function from the parent class to copy the file
        copy_file(PfileName, destination);
    }

    void SaveRepoToTxt(Repository& repo1, fs::path pathToSaveTo)
    {
        // save above info in a txt file in pathtosaveto location
        string fileName = repo1.getName().c_str();
        fileName += "_data.txt";
        
        fs::path pathToSaveToFile = pathToSaveTo / fileName;
        // create file
		this->outputFile = ofstream(pathToSaveToFile);
        // open file
		if (!outputFile) 
		{
			cout << "Error: Could not open the file for writing." << endl;
			return;
		}
        // write data to file
        outputFile << repo1.getName().c_str() << endl;
        outputFile << repo1.getFolderManager().get_current_path();
		outputFile << repo1.getBranchCount() << endl;
        for (int i = 0; i < repo1.getBranchCount(); i++)
        {
            outputFile << repo1.getAllBranches()[i]->getBranchName().c_str() << endl;
        }
        outputFile << repo1.getActiveBranch()->getBranchName().c_str() << endl;
    }

    // load data from a txt file
    void LoadRepoFromTxt(Repository& repo1, fs::path FilePath)
	{
		// load data from file
		this->inputFile = ifstream(FilePath);
		if (!inputFile) 
		{
			cout << "Error: Could not open the file for reading." << endl;
			return;
		}
		char* line = new char[256];
        // read data from file
		inputFile.getline(line, 256);
		repo1.setName(line);

		inputFile.getline(line, 256);
		repo1.setFolderManager(new FolderManager(line));

		inputFile.getline(line, 256);
		repo1.setBranchCount(atoi(line));

		for (int i = 0; i < repo1.getBranchCount(); i++)
		{
			inputFile.getline(line, 256);
            repo1.addBranch(line, i, true);
		}

		inputFile.getline(line, 256);
        
        // finding the active branch
		for (int i = 0; i < repo1.getBranchCount(); i++)
		{
            fs::path linePathed = line;
            if (repo1.getAllBranches()[i]->getBranchName() == linePathed)
			{
				repo1.setActiveBranch(repo1.getAllBranches()[i]);
			}
		}

	}
};