#pragma once
// class to manage .csv and .txt files
#include <iostream>
#include <fstream>
#include<filesystem>
#include "String.h"

using namespace std;
namespace fs = std::filesystem;

class FileManager
{
public:

    char* file_name;    
	fs::path PfileName;
    fs::path CSVPath;
	ifstream inputFile;
    ofstream outputFile;
	int rowCount;

    // virtual functions to be implemented in child classes
    virtual void displayFileData() = 0;
    virtual void readFileData() = 0;
    virtual void writeFileData() = 0;

    //constructor
	FileManager() 
    {
        file_name = new char[1000];
		rowCount = 0;
    }

    void openExistingFile(fs::path fileName)
    {
        inputFile.open(fileName);

        if (!inputFile.is_open()) {
            cout << "Error: Could not open the file. Please try again." << endl;
        }

        cout << "File opened successfully!" << endl;
    }

    void openFile()
    {
        do {
            cout << "Input the name of the file with extension: "; // example.csv

            // Read filename into char* then converting to fs::path
            cin.getline(file_name, 1000);

            // Attempt to open the file
            inputFile.open(file_name);
            CSVPath = fs::current_path();

            if (!inputFile.is_open()) {
                cout << "Error: Could not open the file. Please try again." << endl;
            }
        } while (!inputFile.is_open());


        cout << "File opened successfully!" << endl;
    }

    fs::path getCSVPath() {
        return this->CSVPath;
    }

    void deleteFile()
    {
        if (fs::remove(file_name)) 
        {
            cout << "File deleted successfully: " << file_name << endl;
        }
        else 
        {
            cerr << "Error: Could not delete file: " << file_name << endl;
        }
    }
    
    void closeFile()
    {
		if (inputFile.is_open())
			inputFile.close();
    }

    // Function to copy a file
    void copy_file(const std::filesystem::path& source, const std::filesystem::path& destination) 
    {
        try {
            // Check if the source file exists
            if (!std::filesystem::exists(source)) 
            {
                cout << "Source file does not exist: " << source << endl;
                return;
            }

            // Copy the file to the destination
            std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
            cout << "File copied from " << source << " to " << destination << endl;
        }
        catch (const std::filesystem::filesystem_error& e) 
        {
            cout << "Error copying file: " << e.what() << endl;
        }
    }

    void readFileData(fs::path file_name, char*& data)
    {
        // Open the file for reading
        ifstream inputFile(file_name);
        if (inputFile.is_open())
        {
            char* line = new char[5000];           

            inputFile >> line;
            while (line) 
            {
				my_strcpy(data, line);
                //line = my_strcat(line, "\n");
				delete[] line;
				line = new char[5000];
				inputFile >> line;
            }

            inputFile.close(); // Close the file
            cout << "Data read from the file successfully." << endl;
        }
        else {
            cout << "Error: Could not open the file for reading." << endl;
        }
    }

    virtual ~FileManager() 
    {
        delete[] file_name;
		closeFile();
    }
};