#pragma once
#include "FileManager.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

class TxtFileManager : public FileManager
{
public:
    TxtFileManager() : FileManager() {}
    void readFileData(int&) override
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

    // assignment operator
	TxtFileManager& operator=(const TxtFileManager& txt)
	{
		if (this != &txt)
		{
			FileManager::operator=(txt);
		}
		return *this;
	}

    // append data to the file
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

    void writeFileData(fs::path file_name, String data_to_append) {
        std::ofstream outputFile(file_name, std::ios::app);

        if (outputFile.is_open()) {
            outputFile << data_to_append << std::endl; // Append the data to the file
            outputFile.close(); // Close the file
            std::cout << "Data appended to the file successfully: " << file_name << std::endl;
        }
        else {
            std::cout << "Error: Could not open the file for writing." << std::endl;
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
            cout << "Error creating file: " << file_name << endl;
        }
        else
        {
            cout << "File created successfully: " << file_name << endl;
            outputFile.close();
        }

    }


    void createFile(fs::path file_name)
    {
        outputFile.open(file_name);
        if (!outputFile)
        {
            outputFile.close();
            cout << "Error creating file: " << file_name << endl;
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
        int x = 0;
        readFileData(x);
    }

    void copyFile(const std::filesystem::path& destination)
    {
        // Use the copy_file() function from the parent class to copy the file
        copy_file(PfileName, destination);
    }
};