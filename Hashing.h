#pragma once
#include <iostream>
#include <iomanip>
#include<openssl/sha.h>
#include <openssl/evp.h> // Include the EVP header for the new API
#include "String.h"

using namespace std;

template <typename T>
int instructorHashFunction(T key) 
{
    // generate hash for whether string or integer
    return instructor_Hash(key);
}

// Hashing for string value
int instructor_Hash(String str) {
    int hashkey = 1;
    int i = 0;
    while (str[i] != '\0') {
        int temp = str[i]; // converting to ASCII
        hashkey *= temp;
        ++i;
    }
    return hashkey % 29;
}

// Hashing for integer value
int instructor_Hash(int num) {
    int hashkey = 1;
    while (num > 0) {
        int temp = num % 10; // extracting digits
        hashkey *= temp;
        num /= 10;
    }
    return hashkey % 29;
}

void generate_sha256_hash(const char* input, unsigned char output[SHA256_DIGEST_LENGTH]) 
{
    EVP_MD_CTX* ctx = EVP_MD_CTX_new(); // Create a new context
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr); // Initialize SHA-256
    EVP_DigestUpdate(ctx, input, my_strlen(input)); // Update with input data
    EVP_DigestFinal_ex(ctx, output, nullptr); // Finalize the hash
    EVP_MD_CTX_free(ctx); // Free the context
}

void print_hash(unsigned char hash[SHA256_DIGEST_LENGTH]) 
{
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        // Print in hexadecimal format
        cout << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }
    cout << endl;
}

void generateHash(int hashType, String data, int& iHash, unsigned char shaHash[SHA256_DIGEST_LENGTH])
{
    if (hashType == 1) { iHash = instructorHashFunction(data); }
	else if (hashType == 2) { generate_sha256_hash(data.c_str(), shaHash); iHash = -1; }
}