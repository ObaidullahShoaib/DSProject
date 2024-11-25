#pragma once
#include <iostream>
using namespace std;
class String {
	char* str;
public:
	// default constructor
	String() {
		str = nullptr;
	}
	// constructor with a string
	String(const char* s) {
		str = new char[my_strlen(s) + 1];
		my_strcpy(str, s);
	}
	// copy constructor
	String(const String& s) {
		str = new char[my_strlen(s.str) + 1];
		my_strcpy(str, s.str);
	}
	// destructor
	~String() {
		if (str != nullptr) {
			delete[] str;
		}
	}
	// assignment operator
	String& operator=(const String& s) {
		if (this != &s) {
			if (str != nullptr) {
				delete[] str;
			}
			str = new char[my_strlen(s.str) + 1];
			my_strcpy(str, s.str);
		}
		return *this;
	}
	// equality operator
	bool operator==(const String& s) {
		return my_strcmp(str, s.str) == 0;
	}

	// find length of a string
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
			return -1; // Strings are not equal
		}
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
};