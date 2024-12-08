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
    virtual void readFileData(int&) = 0;
    virtual void writeFileData() = 0;

    //constructor
	FileManager() 
    {
        file_name = new char[1000];
		rowCount = 0;
    }

    // assignment operator
    FileManager& operator=(const FileManager& file) {
		if (this != &file)
		{
			delete[] file_name;
            file_name = NULL;
			file_name = new char[my_strlen(file.file_name) + 1];
			my_strcpy(file_name, file.file_name);
			rowCount = file.rowCount;
		}
		return *this;
	}

	// copy constructor


	// get file name
	char* getFileName () const
	{
		return this->file_name;
	}

    // set file name
	void setFileName(fs::path fileName)
	{
		this->file_name = new char[fileName.string().length() + 1];
		my_strcpy(this->file_name, fileName.string().c_str());
	}

    void openFile(fs::path CSVPath = "")
    {
        if (CSVPath == "")
        {
            int c = 1;
            do
            {
                cout << "Input the name of the file with extension: "; // example.csv                
                // reading filename into char* then converting to fs::path

                if (c == 1)
                    cin.ignore();
                cin.getline(this->file_name, 1000);
                c++;

                // convert char* to fs::path
                fs::path PfileName(this->file_name);               
                CSVPath = fs::current_path() / file_name;
                this->setCSVPath(CSVPath);
                inputFile.open(CSVPath != "" ? CSVPath : this->file_name);
                
            } while (!inputFile.is_open());
            if (!inputFile.is_open())
                cout << "Error: Could not open the file. Please try again." << endl;
            else
                cout << "File opened successfully!" << endl;
            return;
        }
        if (inputFile.is_open())
			inputFile.close();
        inputFile.open(CSVPath != "" ? CSVPath: this->file_name);
        if (!inputFile.is_open())
            cout << "Error: Could not open the file. Please try again." << endl;
        else
            cout << "File opened successfully!" << endl;
    }

    fs::path getCSVPath() {
        return this->CSVPath;
    }

    void setCSVPath(fs::path CSVPath)
    {
        this->CSVPath = CSVPath;
    }

    static void deleteFile(fs::path fileToDelete)
    {
        if (!fs::exists(fileToDelete))
            return;

		if (fs::remove(fileToDelete))
		{
			cout << "File deleted successfully: " << fileToDelete << endl;
		}
		else
		{
			cerr << "Error: Could not delete file: " << fileToDelete << endl;
		}
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

    virtual ~FileManager() 
    {
        delete[] file_name;
		closeFile();
    }
};