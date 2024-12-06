#pragma once
#include <iostream>
using namespace std;
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
