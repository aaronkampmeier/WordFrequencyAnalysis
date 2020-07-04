//
// Created by Aaron Kampmeier on 7/3/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>
#include "kampmeier_aaron_BinarySearchTree.h"

using namespace std;

int main(int argc, char **argv) {
	std::cout << "Running tests..." << std::endl;
	
	//Create a binary search tree and test it
	BinarySearchTree<int> *binarySearchTree = new BinarySearchTree<int>();
	
	binarySearchTree->insert(3);
	binarySearchTree->insert(5);
	binarySearchTree->insert(1);
	binarySearchTree->insert(6);
	binarySearchTree->insert(1);
	binarySearchTree->insert(2);
	binarySearchTree->insert(100);
	binarySearchTree->insert(101);
	binarySearchTree->insert(95);
	binarySearchTree->insert(-2);
	binarySearchTree->insert(-10001);
	binarySearchTree->insert(4);
	
	
	cout << "1 Exists: " << binarySearchTree->elementExists(1) << endl;
	
	cout << "Removing 6 and 1" << endl;
	binarySearchTree->remove(3);
	binarySearchTree->remove(1);
	
	cout << "6 Exists: " << binarySearchTree->elementExists(6) << endl;
	cout << "1 Exists: " << binarySearchTree->elementExists(1) << endl;
	
	int length;
	const int **repArray = binarySearchTree->asRepresentationalArray(length);
	int *outputRepArrary = binarySearchTree->copyPayloads(repArray, length);
	
	for (int i=0; i < length; i++) {
		cout << outputRepArrary[i] << " ";
	}
	cout << endl;
	
	const int **inOrderArray = binarySearchTree->asInOrderArray(length);
	int *outputInOrderArray = binarySearchTree->copyPayloads(inOrderArray, length);
	
	for (int i=0; i < length; i++) {
		cout << outputInOrderArray[i] << " ";
	}
	cout << endl;
	
	
	BinarySearchTree<int*> *secondBst = new BinarySearchTree<int*>();
//	secondBst->insert(&length);
	int a1 = 10;
	secondBst->insert(&a1);
	
	int a2 = 15;
	int a3 = 1;
	int a4 = -23;
	int a5 = 22;
	int a6 = 3;
	secondBst->insert(&a2);
	secondBst->insert(&a3);
	secondBst->insert(&a4);
	secondBst->insert(&a5);
	secondBst->insert(&a6);
	secondBst->insert(&a2);
	
	int* const** secondBstInOrderPointers = secondBst->asInOrderArray(length);
	
	int **secondBstInOrder = secondBst->copyPayloads(secondBstInOrderPointers, length);
	for (int i=0; i < length; i++) {
		cout << secondBstInOrder[i] << ": " << *secondBstInOrder[i] << ", ";
	}
	cout << endl;
	
	int* const** secondBstRepPointers = secondBst->asRepresentationalArray(length);
	int **secondBstRep = secondBst->copyPayloads(secondBstRepPointers, length);
	
	for (int i=0; i < length; i++) {
		if (secondBstRep[i] != nullptr) {
			cout << secondBstRep[i] << ": " << *secondBstRep[i] << ", ";
		} else {
			cout << "empty, ";
		}
	}
	cout << endl;
	
	
	delete [] repArray;
	delete [] inOrderArray;
	delete [] outputInOrderArray;
	delete [] outputRepArrary;
	delete binarySearchTree;
	delete [] secondBstInOrder;
	delete [] secondBstInOrderPointers;
	delete secondBst;
	delete [] secondBstRep;
	delete [] secondBstRepPointers;
}
