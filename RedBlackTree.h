#pragma once
#include "Node.h"
#include <string>
using namespace std;
template <typename T>
class RedBlackTree {
private:
    Node<T>* root;
    Node<T>* nil;

    void leftRotate(Node<T>* x) {
        Node<T>* y = x->right;
        x->right = y->left;
        if (y->left != nil) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node<T>* x) {
        Node<T>* y = x->left;
        x->left = y->right;
        if (y->right != nil) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nil) {
            root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void transplant(Node<T>* u, Node<T>* v) {
        if (u->parent == nil) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node<T>* minimum(Node<T>* node) {
        while (node->left != nil) {
            node = node->left;
        }
        return node;
    }

    /*void deleteFixup(Node<T>* x) {
        while (x != root && x->color == "BLACK") {
            if (x == x->parent->left) {
                Node<T>* w = x->parent->right;
                if (w->color == "RED") {
                    w->color = "BLACK";
                    x->parent->color = "RED";
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == "BLACK" && w->right->color == "BLACK") {
                    w->color = "RED";
                    x = x->parent;
                }
                else {
                    if (w->right->color == "BLACK") {
                        w->left->color = "BLACK";
                        w->color = "RED";
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = "BLACK";
                    w->right->color = "BLACK";
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else {
                Node<T>* w = x->parent->left;
                if (w->color == "RED") {
                    w->color = "BLACK";
                    x->parent->color = "RED";
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == "BLACK" && w->left->color == "BLACK") {
                    w->color = "RED";
                    x = x->parent;
                }
                else {
                    if (w->left->color == "BLACK") {
                        w->right->color = "BLACK";
                        w->color = "RED";
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = "BLACK";
                    w->left->color = "BLACK";
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = "BLACK";
    }*/

    void deleteFixup(Node<T>* x) {
        while (x != root && x->color == "BLACK") {
            if (x == x->parent->left) {
                Node<T>* w = x->parent->right; // Get sibling

                // Case 1: Sibling is red
                if (w->color == "RED") {
                    w->color = "BLACK"; // Change sibling to black
                    x->parent->color = "RED"; // Change parent to red
                    leftRotate(x->parent); // Rotate left
                    w = x->parent->right; // Update sibling
                }

                // Case 2: Both children of sibling are black
                if (w->left->color == "BLACK" && w->right->color == "BLACK") {
                    w->color = "RED"; // Change sibling to red
                    x = x->parent; // Move up the tree
                }
                else {
                    // Case 3: Right child of sibling is black
                    if (w->right->color == "BLACK") {
                        w->left->color = "BLACK"; // Change left child to black
                        w->color = "RED"; // Change sibling to red
                        rightRotate(w); // Rotate right
                        w = x->parent->right; // Update sibling
                    }

                    // Case 4: Sibling is black and right child is red
                    w->color = x->parent->color; // Copy parent's color to sibling
                    x->parent->color = "BLACK"; // Change parent to black
                    w->right->color = "BLACK"; // Change right child to black
                    leftRotate(x->parent); // Rotate left
                    x = root; // Exit the loop
                }
            }
            else {
                Node<T>* w = x->parent->left; // Get sibling

                // Case 1: Sibling is red
                if (w->color == "RED") {
                    w->color = "BLACK"; // Change sibling to black
                    x->parent->color = "RED"; // Change parent to red
                    rightRotate(x->parent); // Rotate right
                    w = x->parent->left; // Update sibling
                }

                // Case 2: Both children of sibling are black
                if (w->right->color == "BLACK" && w->left->color == "BLACK") {
                    w->color = "RED"; // Change sibling to red
                    x = x->parent; // Move up the tree
                }
                else {
                    // Case 3: Left child of sibling is black
                    if (w->left->color == "BLACK") {
                        w->right->color = "BLACK"; // Change right child to black
                        w->color = "RED"; // Change sibling to red
                        leftRotate(w); // Rotate left
                        w = x->parent->left; // Update sibling
                    }

                    // Case 4: Sibling is black and left child is red
                    w->color = x->parent->color; // Copy parent's color to sibling
                    x->parent->color = "BLACK"; // Change parent to black
                    w->left->color = "BLACK"; // Change left child to black
                    rightRotate(x->parent); // Rotate right
                    x = root; // Exit the loop
                }
            }
        }
        x->color = "BLACK"; // Ensure the root is black
    }

public:
    RedBlackTree() {
        nil = new Node<T>();
        root = nil;
    }

    void insert(T key, String data) {
        Node<T>* z = new Node<T>(key, data);
        z->left = z->right = z->parent = nil;

        Node<T>* y = nil;
        Node<T>* x = root;

        while (x != nil) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        z->parent = y;
        if (y == nil) {
            root = z;
        }
        else if (z->key < y->key) {
            y->left = z;
        }
        else {
            y->right = z;
        }

        z->color = "RED";
        insertFixup(z);
    }

    void insertFixup(Node<T>* z) {
        while (z->parent->color == "RED") {
            if (z->parent == z->parent->parent->left) {
                Node<T>* y = z->parent->parent->right;
                if (y->color == "RED") {
                    z->parent->color = "BLACK";
                    y->color = "BLACK";
                    z->parent->parent->color = "RED";
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = "BLACK";
                    z->parent->parent->color = "RED";
                    rightRotate(z->parent->parent);
                }
            }
            else {
                Node<T>* y = z->parent->parent->left;
                if (y->color == "RED") {
                    z->parent->color = "BLACK";
                    y->color = "BLACK";
                    z->parent->parent->color = "RED";
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = "BLACK";
                    z->parent->parent->color = "RED";
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = "BLACK";
    }

    /*void remove(T key) {
        Node<T>* z = search(key);
        if (z == nil) return;

        Node<T>* y = z;
        Node<T>* x;
        string y_original_color = y->color;

        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            }
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if (y_original_color == "BLACK") {
            deleteFixup(x);
        }

        delete z;
    }*/

    void remove(T key) {
        Node<T>* z = search(key);
        if (z == nil) return;

        Node<T>* y = z; // Node to be deleted
        Node<T>* x; // Node to replace z
        String y_original_color = y->color; // Store the original color of y

        // Case 1: Node has no children (leaf node)
        if (z->left == nil) {
            x = z->right; // x is the right child
            transplant(z, z->right);
        }
        // Case 2: Node has no right child
        else if (z->right == nil) {
            x = z->left; // x is the left child
            transplant(z, z->left);
        }
        // Case 3: Node has two children
        else {
            y = minimum(z->right); // Find the in-order successor
            y_original_color = y->color; // Store the color of the successor
            x = y->right; // x is the right child of y

            if (y->parent == z) {
                x->parent = y; // Maintain parent link
            }
            else {
                transplant(y, y->right); // Transplant y
                y->right = z->right; // Replace y's right child with z's right child
                y->right->parent = y; // Set parent of y's right child
            }

            transplant(z, y); // Transplant z with y
            y->left = z->left; // Replace y's left child with z's left child
            y->left->parent = y; // Set parent of y's left child
            y->color = z->color; // Transfer color from z to y
        }

        delete z; // Delete the original node

        if (y_original_color == "BLACK") {
            deleteFixup(x); // Call fix-up if the original color was black
        }
    }

    Node<T>* search(T key) {
        Node<T>* current = root;
        while (current != nil && key != current->key) {
            if (key < current->key) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return current;
    }

	//For testing purposes
	void inorder() {
		inorderHelper(root);
	}

	void inorderHelper(Node<T>* node) {
		if (node == nil) return;
		inorderHelper(node->left);
		cout << node->key <<" " << node->color << endl;
		inorderHelper(node->right);
	}

	~RedBlackTree() {
		clearAll(root);
		delete nil;
	}

	void clearAll(Node<T>* node) {
		if (node == nil) return;
		clearAll(node->left);
		clearAll(node->right);
		delete node;
	}
};