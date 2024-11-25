#pragma once
#include "String.h"
template <typename T>
struct Node {
    T key;
    String data;
    Node* parent;
    Node* left;
    Node* right;
    int height;
    String color;

    Node() {
        left = right = parent = nullptr;
        color = "BLACK";
        height = 0;
    }

    Node(T key, String data) {
        this->key = key;
        this->data = data;
        left = right = parent = nullptr;
        color = "RED";
        height = 0;
    }
};