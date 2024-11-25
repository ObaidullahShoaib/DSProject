#include "RedBlackTree.h"
#include "AVLTree.h"

int main() {
	RedBlackTree<int> tree1;
	AVLTree<int> tree2;
	tree1.insert(10, "10");
	tree1.insert(20, "20");
	tree1.insert(22, "22");
	tree1.insert(25, "25");
	tree1.insert(0, "0");
	tree1.insert(5, "5");
	tree1.insert(1, "1");
	tree1.inorder();
	cout << endl;

	tree2.insert(10, "10");
	tree2.insert(20, "20");
	tree2.insert(22, "22");
	tree2.insert(25, "25");
	tree2.insert(0, "0");
	tree2.insert(5, "5");
	tree2.insert(1, "1");
	tree2.inorder();
	cout << endl;
	

	return 0;
}