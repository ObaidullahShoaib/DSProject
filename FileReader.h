#pragma once
#include <iostream>
#include <fstream>
#include "String.h"

using namespace std;
class FileReader {
    char file_name[1000];
    ifstream inputFile;
    int rowCount = 0;
    char** row_data;       // Array of char* for rows
    char** key_data;      // Array of char* for keys
    int columnCount = 0;
    char** columns_data;


    void closeFile()
    {
		if (inputFile.is_open())
            inputFile.close();
    }

public:
	FileReader() {}
    ~FileReader() {
		closeFile();
        for (int i = 0; i < columnCount; ++i)
        {
            delete[] columns_data[i];
        }
        delete[] columns_data;

        for (int i = 0; i < rowCount; ++i)
        {
            delete[] row_data[i];
            delete[] key_data[i];
        }
        delete[] row_data;
        delete[] key_data;
    }
	void openFile()
	{
        do
        {
            //cout << "Input the name of the CSV file with extension: "; // example.csv
            cin.ignore();
            cin.getline(file_name, 1000);

            inputFile.open(file_name);
            if (!inputFile.is_open())
            {
                cout << "Error: Could not open the file. Please try again." << endl;
            }
        } while (!inputFile.is_open());

        cout << "File opened successfully!" << endl;
	}

    void readFile()
    {
        // Reads row data
        char line[5000];

        // Read the first line to determine the number of columns
        if (inputFile.getline(line, 5000))
        {
            // Count the number of commas in the line
            for (int i = 0; line[i] != '\0'; i++)
            {
                if (line[i] == ',')
                {
                    columnCount++;
                }
            }
            columnCount++; // Add one to get the total number of columns
        }
        cout << "\nNumber of columns: " << columnCount << endl;

        // Storing column headings
        columns_data = new char* [columnCount]; // Array of char* for columns
        int iterator = 0; // Iterator to check if it enters the specific key column

        cout << "\nColumn Headers: " << endl;
        int i = 0;
        char* token = my_strtok(line, ","); // Split the line by commas
        while (token != nullptr && iterator < columnCount)
        {
            columns_data[i] = new char[my_strlen(token) + 1];
            my_strcpy(columns_data[i], token);
            cout << i + 1 << ". " << columns_data[i] << endl;
            token = my_strtok(nullptr, ",");
            i++;
            ++iterator;
        }

        int choice = 0;
        cout << "\nInput serial number for Key column: ";
        cin >> choice;
        while (choice <= 0 || choice > columnCount)
        {
            cout << "\nInvalid choice. Try again.";
            cout << "\nInput serial number for column: ";
            cin >> choice;
        }

        cout << "\nYour Key Column is: " << columns_data[choice - 1] << endl;

        while (inputFile.getline(line, 1000))
        {
            rowCount++; // Increment the row count for each line read
        }

        cout << "\nNumber of rows: " << rowCount << endl;

        row_data = new char* [rowCount];       // Array of char* for rows
        key_data = new char* [rowCount];      // Array of char* for keys

        // Goto start of file
        inputFile.clear();
        inputFile.seekg(0);

        // Skip the header again
        inputFile.getline(line, 1000);
        int row_index = 0;  // Index for row data
        int key_index = 0;  // Key string indexing

        while (inputFile.getline(line, 1000))
        {
            if (line[0] == '\0')
                break;

            // Store the entire row
            row_data[row_index] = new char[my_strlen(line) + 1];
            my_strcpy(row_data[row_index], line);
            row_index++;

            // Extract the key
            char* token = my_strtok(line, ",");
            int columnIndex = 1; // Column counter
            while (token != nullptr)
            {
                if (columnIndex == choice) // Key column
                {
                    key_data[key_index] = new char[my_strlen(token) + 1];
                    my_strcpy(key_data[key_index], token);
                    key_index++;
                    break;
                }
                token = my_strtok(nullptr, ",");
                columnIndex++;
            }
        }
    }

	int getRowCount()
	{
		return rowCount;
	} 

    void getInformation(String& key, String& data, int index) {
        if (index >= 0 && index < rowCount) {
            key = key_data[index];
            data = row_data[index];
        }
    }

    void displayData() {
        // Print data along with keys
        for (int i = 0; i < rowCount; ++i)
        {
            String key, data;
            getInformation(key, data, i);
            cout << i + 1 << ". Key: " << key << "\n" << data << endl;
        }
    }


};