//
// Created by Aaron Kampmeier on 6/30/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>
#include "kampmeier_aaron_BinarySearchTree.h"

int main() {
	std::cout << "Hello, World!" << std::endl;
	
	//Create a binary search tree and test it
	BinarySearchTree<int> *binarySearchTree = new BinarySearchTree<int>();
	
	binarySearchTree->insert(3);
	binarySearchTree->insert(5);
	binarySearchTree->insert(1);
	binarySearchTree->insert(6);
	binarySearchTree->insert(2);
	
	BinarySearchTree<char[]> *bst = new BinarySearchTree<char[]>();
	
	return 0;
}
