//
// Created by Aaron Kampmeier on 6/30/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

//Checks to make sure that whoever compiles my program does not compile this file into an object file because it will
// already be compiled as part of the kampmeier_aaron_BinarySearchTree.h. See bottom of
// kampmeier_aaron_BinarySearchTree.h for more info.
#ifndef ASSIGNMENT_4_KAMPMEIER_AARON_BINARYSEARCHTREE_H
#error "This file must not be compiled directly! It is compiled implicitly with the rest of the program because it is included inside of kampmeier_aaron_BinarySearchTree.h"
#endif

#include "kampmeier_aaron_BinarySearchTree.h"

/**
 * Constructor for BinarySearchTree. Creates an empty binary search tree with no root node.
 * @tparam T
 */
template<class T>
BinarySearchTree<T>::BinarySearchTree() {
	root = nullptr;
}

/**
 * Destructs the binary search tree by removing all the nodes and then the root
 * @tparam T
 */
template<class T>
BinarySearchTree<T>::~BinarySearchTree() {
	empty();
}

/**
 * Inserts the specified payload into the binary search tree. If the payload already exists, then nothing happens.
 * This function will maintain the integrity of the BST by not adding in duplicates of equal payloads.
 * @tparam T
 * @param payload
 * @return A pointer to the payload stored in the BST, could be from a payload newly added or from a payload already
 * in the tree.
 */
template<class T>
const T *BinarySearchTree<T>::insert(const T &payload) {
	// Find the place where it should be added starting at root
	
	// Go through each node in the loop and check them one by one to see where the new node should be stored. Instead
	// of storing a pointer to the node I'm currently checking, I'm storing a pointer to the pointer on the parent
	// node that points to the child I'm checking. This way I can modify the parent's child pointer value if I want
	// to add my node in there. Thus, the current node I'm checking against is given by the double de-reference of
	// parentConnection (**parentConnection) and the left child is **parentConnection.left or (*parentConnection)->left
 	Node **parentConnection = &root;
	while (*parentConnection != nullptr) {
		 if (payload < (*parentConnection)->payload) {
			// The payload is less than the current node's payload, move down to the left
			parentConnection = &((*parentConnection)->left);
		} else if (payload > (*parentConnection)->payload) {
		 	// The payload is greater than the current node's payload, now check the right child
		 	parentConnection = &((*parentConnection)->right);
		 } else if (payload == (*parentConnection)->payload) {
		 	// The new payload is equal to this node's payload, so cancel the insertion and just return a pointer to
		 	// this payload
			 return &((*parentConnection)->payload);
		 } else {
		 	exit(NODE_COMPARISON_FAILURE_EXIT);
		 }
	}
	
	// Reached the end of the list and didn't find any previously added equal payloads, so just add it here
	*parentConnection = new Node(payload);
	return &((*parentConnection)->payload);
}

/**
 * Removes the specified payload from the tree.
 * @tparam T
 * @param payloadToRemove
 * @return Whether the removal was successful or not. Should only fail if the payload does not exist in the tree.
 */
template<class T>
bool BinarySearchTree<T>::remove(const T &payloadToRemove) {
	// We're gonna traverse our tree checking each node until we find the one to delete, which will end up in
	// currentNode
	Node *currentNode = root;
	// Because we need to also update the parent's connection to the node we're deleting, we'll store a pointer to
	// the parent's connection to the child to be deleted. This is a pointer to either the left or right properties
	// on Node.
	Node **parentConnection = &root;
	
	while (currentNode != nullptr) {
		if (payloadToRemove < currentNode->payload) {
			// Go to the left side
			parentConnection = &(currentNode->left);
			currentNode = currentNode->left;
		} else if (payloadToRemove > currentNode->payload) {
			// Go to the right side
			parentConnection = &(currentNode->right);
			currentNode = currentNode->right;
		} else if (payloadToRemove == currentNode->payload) {
			// This is the one to delete
			// Three scenarios for deletion:
			// 1. Node is a leaf
			if (currentNode->left == nullptr && currentNode->right == nullptr) {
				// Just delete it
				*parentConnection = nullptr;
				delete currentNode;
			}
			// 2. Node has one child
			else if (currentNode->left == nullptr || currentNode->right == nullptr) {
				// Connect the parent's connection to its one child
				*parentConnection = currentNode->left == nullptr ? currentNode->right : currentNode->left;
				
				// Delete the current node
				delete currentNode;
			}
			// 3. Node has two children
			else {
				//To delete a node with two children, we replace it with its in order successor. The successor
				// necessarily does not have a left child
				//To find the in order successor, we will jump to the right child and then traverse down the left side
				Node *inOrderSuccessor = currentNode->right;
				Node **inOrderSuccessorParentConnection = &(currentNode->right);
				while (inOrderSuccessor->left != nullptr) {
					inOrderSuccessorParentConnection = &(inOrderSuccessor->left);
					inOrderSuccessor = inOrderSuccessor->left;
				}
				
				// Move the parent's connection over to the successor
				*parentConnection = inOrderSuccessor;
				
				// Replace the old parent connection to the successor with the successor's right child
				*inOrderSuccessorParentConnection = inOrderSuccessor->right;
				
				// Move the left child from the current node over
				inOrderSuccessor->left = currentNode->left;
				
				// Move the right child over
				inOrderSuccessor->right = currentNode->right;
				
				// Now delete the old node because everything has been moved over
				delete currentNode;
			}
			
			return true;
		} else {
			exit(NODE_COMPARISON_FAILURE_EXIT);
		}
	}
	
	// Once we get here, then we know we didn't find the payload. Failure.
	return false;
}

/**
 * Checks if a specified payload exists in the BST.
 * @tparam T
 * @param payload
 * @return True if it exists, false if it does not.
 */
template<class T>
bool BinarySearchTree<T>::elementExists(const T &payload) {
	return find(payload) != nullptr; //If we can find a pointer to it, it exists
}

/**
 * Finds the specified payload in the tree and returns a pointer to it's location within the tree.
 * @tparam T
 * @param payload
 * @return A pointer to the payload found within the tree. Will be NULL if not found.
 */
template<class T>
const T * BinarySearchTree<T>::find(const T &payload) {
	// Check the payload against nodes in the tree, starting with the root node
	Node *checkingAgainst = root;
	
	while (checkingAgainst != nullptr) {
		if (checkingAgainst->payload == payload) {
			// The specified payload and this node's payload are the same, return this payload pointer
			return &(checkingAgainst->payload);
		} else if (payload < checkingAgainst->payload) {
			// Payload is less, go check the left tree
			checkingAgainst = checkingAgainst->left;
		} else if (payload > checkingAgainst->payload) {
			// Payload is bigger, go check right tree
			checkingAgainst = checkingAgainst->right;
		} else {
			exit(NODE_COMPARISON_FAILURE_EXIT);
		}
	}
	
	// Reached the end of the tree, didn't find it
	return nullptr;
}

/**
 * Returns a representation of the current BST stored in an array structure. Stores it in an array the traditional
 * way, and not in-order.
 * @tparam T
 * @param returnLength A reference variable where the length of the returned array will be set into.
 * @return The array representation of all the Nodes in the BST
 */
template<class T>
T *BinarySearchTree<T>::toRepresentationalArray(int &returnLength) {
	//TODO: Arr length is wrong, it should be 2^(max depth)
	const int arrLength = 
	auto *outputArray = new T[arrLength];
	
	addNodeToRepresentationalArray(root, outputArray, 0);
	
	returnLength = arrLength;
	return outputArray;
}

/**
 * Recursively defined method that adds a specified node to the output array at the specified index. Then calls on
 * its child nodes to do the same at their indices. The indices for the child storage are standard. If a parent node
 * is stored at index k in a representational array, then its left child is stored at 2k+1.
 * @tparam T
 * @param node The parent node to store
 * @param array The output representational array to write to
 * @param nodeIndex The index to store the parent node at
 */
template<class T>
void BinarySearchTree<T>::addNodeToRepresentationalArray(const BinarySearchTree::Node *node, T *array,
														 const int nodeIndex) {
	if (node != nullptr) {
		array[nodeIndex] = node->payload;
		addNodeToRepresentationalArray(node->left, array, 2 * nodeIndex + 1);
		addNodeToRepresentationalArray(node->right, array, 2 * nodeIndex + 2);
	}
}

/**
 * Returns an ordered array of the payloads stored within this BST.
 * @tparam T
 * @param returnLength A reference variable where the length of the returned array will be set into.
 * @return The return, in-order collection of payloads
 */
template<class T>
T *BinarySearchTree<T>::toInOrderArray(int &returnLength) {
	const int arrLength = length();
	auto *outputArray = new T[arrLength];
	// Variable to track the current write index in the array
	int arrWriteIndex = 0;
	
	addNodeToInOrderArray(root, outputArray, arrWriteIndex);
	
	returnLength = arrLength;
	return outputArray;
}

/**
 * Recursively defined method to add the payloads of a node and its children to an array in-order. Because each
 * recursive call has no idea how many array slots its children will take up, we keep a reference to a singular
 * currentWriteIndex variable that increments every time we write to the array.
 * @tparam T
 * @param node
 * @param array The array to write into. Must be at least of length lengthOfTree(node).
 * @param currentWriteIndex A reference to the write index to start writing from, will be updated as more values are
 * written into the array.
 */
template<class T>
void BinarySearchTree<T>::addNodeToInOrderArray(Node *node, T *array, int &currentWriteIndex) {
	if (node != nullptr) {
		addNodeToInOrderArray(node->left, array, currentWriteIndex);
		
		array[currentWriteIndex] = node->payload;
		currentWriteIndex++;
		
		addNodeToInOrderArray(node->right, array, currentWriteIndex);
	}
}

/**
 * Empties out the entire list - removes all nodes and their payloads. All information is destroyed (or invalidated) so
 * be careful.
 * @tparam T
 * @return Successful or not
 */
template<class T>
bool BinarySearchTree<T>::empty() {
	deleteBinaryTree(root);
	root = nullptr;
	return true;
}

/**
 * Recursively defined method that deletes a binary tree starting from a node and cascading all the way down. Make
 * sure to take into account that all pointers to any nodes deleted in this method will be invalid after deletion.
 * @tparam T
 * @param startingNode The root node to start deletion from
 */
template<class T>
void BinarySearchTree<T>::deleteBinaryTree(Node *startingNode) {
	if (startingNode != nullptr) {
		deleteBinaryTree(startingNode->left);
		deleteBinaryTree(startingNode->right);
		delete startingNode;
	} else {
		// Do nothing, the node is null
	}
}

/**
 * Counts and returns the number of elements (Nodes) in this Binary Search Tree.
 * @tparam T
 * @return
 */
template<class T>
int BinarySearchTree<T>::length() {
	return lengthOfTree(root);
}

/**
 * Recursively defined method that returns the num of elements of a binary tree including the supplied root element.
 * @tparam T
 * @param rootNode The root of the binary tree to count. Note: Could be a child node in some larger tree.
 * @return The number of elements in this tree.
 */
template<class T>
int BinarySearchTree<T>::lengthOfTree(const BinarySearchTree::Node *rootNode) {
	if (rootNode == nullptr) {
		return 0;
	} else {
		return 1 + lengthOfTree(rootNode->left) + lengthOfTree(rootNode->right); // +1 for the current node
	}
}




/**
 * Generates a new Node with payload and both children connections set to null.
 * @tparam T
 * @param payload
 */
template<class T>
BinarySearchTree<T>::Node::Node(const T &newPayload): payload(newPayload) {
	left = right = nullptr;
}

template<class T>
bool BinarySearchTree<T>::Node::operator<(const BinarySearchTree::Node &other) {
	return this->payload < other.payload;
}

template<class T>
bool BinarySearchTree<T>::Node::operator>(const BinarySearchTree::Node &other) {
	return this->payload > other.payload;
}

template<class T>
bool BinarySearchTree<T>::Node::operator==(const BinarySearchTree::Node &other) {
	return this->payload == other.payload;
}

/**
 * Destructs the node. Shouldn't have to do anything special because no pointers are managed by this Node. Does not
 * cascade delete nodes below it.
 * @tparam T
 */
template<class T>
BinarySearchTree<T>::Node::~Node() = default;
