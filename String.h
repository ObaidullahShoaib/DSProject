#pragma once
#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

class String {
private:
    char* str;

    // Helper functions
    static int my_strlen(const char* s) {
        int len = 0;
        while (s[len] != '\0') len++;
        return len;
    }

    static void my_strcpy(char* dest, const char* src) {
        int i = 0;
        while ((dest[i] = src[i]) != '\0') i++;
    }

    static int my_strcmp(const char* s1, const char* s2) {
        while (*s1 && (*s1 == *s2)) {
            s1++;
            s2++;
        }
        return (unsigned char)*s1 - (unsigned char)*s2;
    }

    static char* my_strcat(char* dest, const char* src) {
        char* temp = dest + my_strlen(dest);
        while ((*temp++ = *src++) != '\0');
        return dest;
    }

public:
    // Constructors and Destructor
    String() : str(nullptr) {}

    char* getStr() const {
		return str;
    }

    // set str
	void setStr(const char* s) {
		delete[] str;
		str = new char[my_strlen(s) + 1];
		my_strcpy(str, s);
	}
    
    String(const char* s) {
        str = new char[my_strlen(s) + 1];
        my_strcpy(str, s);
    }

    String(const String& s) {
        str = new char[my_strlen(s.str) + 1];
        my_strcpy(str, s.str);
    }

    ~String() {
        delete[] str;
    }

    // Assignment Operators
    String& operator=(const String& s) {
        if (this != &s) {
            delete[] str;
            str = new char[my_strlen(s.str) + 1];
            my_strcpy(str, s.str);
        }
        return *this;
    }

    String& operator=(const char* s) {
        delete[] str;
        str = new char[my_strlen(s) + 1];
        my_strcpy(str, s);
        return *this;
    }

    // Comparison Operators
    bool operator==(const String& s) const {
        return my_strcmp(str, s.str) == 0;
    }

    bool operator!=(const String& s) const {
        return !(*this == s);
    }

    bool operator<(const String& s) const {
        return my_strcmp(str, s.str) < 0;
    }

    bool operator>(const String& s) const {
        return my_strcmp(str, s.str) > 0;
    }

    bool operator<=(const String& s) const {
        return !(*this > s);
    }

    bool operator>=(const String& s) const {
        return !(*this < s);
    }

    // Indexing Operators
    char& operator[](int index) {
        return str[index];
    }

    const char& operator[](int index) const {
        return str[index];
    }

    // Concatenation
    String operator+(const String& s) const {
        char* temp = new char[length() + s.length() + 1];
        my_strcpy(temp, str);
        my_strcat(temp, s.str);
        String result(temp);
        delete[] temp;
        return result;
    }

    String operator+(const char* s) const {
        char* temp = new char[length() + my_strlen(s) + 1];
        my_strcpy(temp, str);
        my_strcat(temp, s);
        String result(temp);
        delete[] temp;
        return result;
    }

    String& operator+=(const String& s) {
        *this = *this + s;
        return *this;
    }

    String& operator+=(const char* s) {
        *this = *this + s;
        return *this;
    }

    // Utility Functions
    int length() const {
        return my_strlen(str);
    }

    bool empty() const {
        return length() == 0;
    }

    void clear() {
        delete[] str;
        str = nullptr;
    }

    const char* c_str() const {
        return str;
    }

    // String Manipulation
    String substr(int start, int len) const {
        if (start < 0 || start >= length() || len <= 0) {
            return String();
        }
        char* temp = new char[len + 1];
        for (int i = 0; i < len; ++i) {
            temp[i] = str[start + i];
        }
        temp[len] = '\0';
        String result(temp);
        delete[] temp;
        return result;
    }

    void to_upper() {
        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] -= 32;
            }
        }
    }

    void to_lower() {
        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] >= 'A' && str[i] <= 'Z') {
                str[i] += 32;
            }
        }
    }

    int find(const char* substring) const {
        int len = my_strlen(substring);
        for (int i = 0; i <= length() - len; i++) {
            if (my_strcmp(&str[i], substring) == 0) {
                return i;
            }
        }
        return -1;
    }

    int rfind(const char* substring) const {
        int len = my_strlen(substring);
        for (int i = length() - len; i >= 0; i--) {
            if (my_strcmp(&str[i], substring) == 0) {
                return i;
            }
        }
        return -1;
    }

    String trim() const {
        int start = 0, end = length() - 1;
        while (str[start] == ' ') start++;
        while (str[end] == ' ') end--;
        return substr(start, end - start + 1);
    }
    

    // Append a character at the start of the string
    String appendchAtStart(char ch) const
    {
        String temp = "";
        temp.str = new char[this->length() + 2];
        temp.str[0] = ch;
        for (int i = 0; i < this->length(); i++) {
            temp.str[i + 1] = str[i];
        }

        // Null-terminate the new string
        temp.str[this->length() + 1] = '\0';

        return temp;
    }

    // Append character at end of the string
    String appendchAtEnd(char ch) const {

        String temp = "";
        temp.str = new char[this->length() + 2];

        for (int i = 0; i < this->length(); i++) 
        {
            temp.str[i] = str[i];
        }

        temp.str[this->length()] = ch;
        temp.str[this->length() + 1] = '\0';

        return temp;
    }


    // Function to edit a specific column in a comma-separated row
    char* editColumnValue(const char* rowData, int columnNo, const char* newValue)
    {
        // Copy the input row data into a mutable array
        char* mutableRow = new char[strlen(rowData) + 1];
        strcpy(mutableRow, rowData);

        // Temporary array to store the new row data
        char* updatedRow = new char[strlen(rowData) + strlen(newValue) + 1];
        updatedRow[0] = '\0'; // Initialize as an empty string

        // Tokenize the row data to extract column values
        char* token = strtok(mutableRow, ",");
        int currentColumn = 1;

        while (token != nullptr)
        {
            // Check if it's the column to be edited
            if (currentColumn == columnNo)
            {
                strcat(updatedRow, newValue); // Append the new value
            }
            else
            {
                strcat(updatedRow, token); // Append the original value
            }

            // Add a comma if it's not the last column
            token = strtok(nullptr, ",");
            if (token != nullptr)
            {
                strcat(updatedRow, ",");
            }

            currentColumn++;
        }

        // Clean up
        delete[] mutableRow;

        // Return the updated row
        return updatedRow;
    }

    


    // Additional Features
    bool starts_with(const char* prefix) const {
        int len = my_strlen(prefix);
        return my_strcmp(str, prefix) == 0;
    }

    bool ends_with(const char* suffix) const {
        int len = my_strlen(suffix);
        return my_strcmp(&str[length() - len], suffix) == 0;
    }

    int compare(const char* s) const {
        return my_strcmp(str, s);
    }

    int compare(const String& s) const {
        return my_strcmp(str, s.str);
    }

    friend ostream& operator<<(ostream& os, const String& s);
    friend istream& operator>>(istream& is, String& s);
};


ostream& operator<<(ostream& os, const String& s) {
	os << s.str;
	return os;
}

istream& operator>>(istream& is, String& s) {
	char temp[100];
	is >> temp;
	s = String(temp);
	return is;
}

int my_strlen(const char* sentence) {
	int i = 0;
	while (sentence[i] != '\0') {
		i++;
	}
	return i;
}
// copy a string
void my_strcpy(char* to, const char* from) {
	int i = 0;
	while (from[i] != '\0') {
		to[i] = from[i];
		i++;
	}
	to[i] = '\0';
}
// compare two strings
int my_strcmp(const char* p1, const char* p2) {
	int i = 0;
	while (p1[i] == p2[i] && p1[i] != '\0' && p2[i] != '\0') {
		i++;
	}
	if (p1[i] == '\0' && p2[i] == '\0') {
		return 0; // Strings are equal
	}
	else {
		// Strings are not equal
		if (p1[i] < p2[i]) {
			return -1;
		}
		else {
			return 1;

		}
	}
}

char* my_strtok(char* str, const char* delim) {
	static char* p;
	if (str != nullptr) {
		p = str;
	}
	else if (*p == '\0') {
		return nullptr;
	}
	char* ret = p;
	while (*p != '\0') {
		if (*p == *delim) {
			*p = '\0';
			p++;
			break;
		}
		p++;
	}
	return ret;
}

// String to char*
char* stringToChar(const String& str) {
	char* cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}
 

void my_strcat(char* destination, const char* source) {
    while (*destination) {
        destination++;
    }

    while (*source) {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
}

int stringToInt(const std::string& str) {
    int result = 0;
    int multiplier = 1;

    for (int i = str.length() - 1; i >= 0; --i) {
        int digit = str[i] - '0';
        result += digit * multiplier;
        multiplier *= 10;
    }

    return result;
}


String intToString(int num) {
	if (num == 0) {
		return "0";
	}

	String result = "";
	while (num > 0) {
		char digit = num % 10 + '0';
		result = result.appendchAtStart(digit);
		num /= 10;
	}
	return result;
}

// Function to extract the .csv filename
String extractCSVFileName(fs::path fp)
{
	String filePath = fp.string().c_str();
    // Remove trailing slashes
    String normalizedPath = filePath;
    while (!normalizedPath.empty() && (normalizedPath[normalizedPath.length() - 1] == '/' || normalizedPath[normalizedPath.length() - 1] == '\\')) {
        normalizedPath = normalizedPath.substr(0, normalizedPath.length() - 1);
    }

    // Find the last slash to locate the file name
    int lastSlashIndex = -1;
    for (int i = 0; i < normalizedPath.length(); i++) {
        if (normalizedPath[i] == '/' || normalizedPath[i] == '\\') {
            lastSlashIndex = i;
        }
    }

    // Extract the filename
    String fileName = normalizedPath.substr(lastSlashIndex + 1, normalizedPath.length() - lastSlashIndex - 1);

    // Check if the file name ends with ".csv"
    if (fileName.length() > 4 && fileName.substr(fileName.length() - 4, 4) == ".csv") {
        return fileName; // Valid .csv filename
    }

    // Return an empty string if it's not a .csv file
    String emptyStr = "";
    return emptyStr;
}

void replaceLineInFile(const fs::path& filePath, int lineNumber, const string& newLine)
{
    // Open the file for reading
    ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        cout << "Error: Could not open file for reading: " << filePath << endl;
        return;
    }

    // Dynamically allocate memory for storing lines
    int maxLines = 1000; // Arbitrary maximum for demonstration
    char** lines = new char* [maxLines];
    int lineCount = 0;

    // Read lines into the dynamic array
    char buffer[1000]; // Temporary buffer for line reading
    while (inputFile.getline(buffer, 1000) && lineCount < maxLines)
    {
        lines[lineCount] = new char[strlen(buffer) + 1];
        strcpy(lines[lineCount], buffer);
        lineCount++;
    }
    inputFile.close();

    // Validate the line number
    if (lineNumber < 1 || lineNumber > lineCount)
    {
        cout << "Error: Line number out of range.\n";
        for (int i = 0; i < lineCount; i++)
        {
            delete[] lines[i];
        }
        delete[] lines;
        return;
    }

    // Replace the specific line
    delete[] lines[lineNumber - 1]; // Free the old line memory
    lines[lineNumber - 1] = new char[newLine.length() + 1];
    strcpy(lines[lineNumber - 1], newLine.c_str());

    // Open the file for writing
    ofstream outputFile(filePath);
    if (!outputFile.is_open())
    {
        cout << "Error: Could not open file for writing: " << filePath << endl;
        for (int i = 0; i < lineCount; i++)
        {
            delete[] lines[i];
        }
        delete[] lines;
        return;
    }

    // Write all lines back to the file
    for (int i = 0; i < lineCount; i++)
    {
        outputFile << lines[i] << '\n';
    }
    outputFile.close();

    // Free allocated memory
    for (int i = 0; i < lineCount; i++)
    {
        delete[] lines[i];
    }
    delete[] lines;

    cout << "Line " << lineNumber << " replaced successfully.\n";
}