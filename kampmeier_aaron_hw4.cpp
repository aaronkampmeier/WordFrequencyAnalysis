//
// Created by Aaron Kampmeier on 6/30/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>
#include "kampmeier_aaron_BinarySearchTree.h"

using namespace std;

int main() {
	std::cout << "Hello, World!" << std::endl;
	
	//Create a binary search tree and test it
	BinarySearchTree<int> *binarySearchTree = new BinarySearchTree<int>();
	
	binarySearchTree->insert(3);
	binarySearchTree->insert(5);
	binarySearchTree->insert(1);
	binarySearchTree->insert(6);
	binarySearchTree->insert(1);
	binarySearchTree->insert(2);
	
	binarySearchTree->remove(3);
	
	cout << "6 Exists: " << binarySearchTree->elementExists(6) << endl;
	cout << "1 Exists: " << binarySearchTree->elementExists(1) << endl;
	
	int length;
	int *repArray = binarySearchTree->toRepresentationalArray(length);
	int *inOrderArray = binarySearchTree->toInOrderArray(length);
	
	delete [] repArray;
	delete [] inOrderArray;
	
	return 0;
}
