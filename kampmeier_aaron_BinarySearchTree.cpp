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
	//TODO:
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
	return &(*parentConnection->payload);
}

/**
 * Removes the specified payload from the tree.
 * @tparam T
 * @param payloadToRemove
 * @return Whether the removal was successful or not. Should only fail if the payload does not exist in the tree.
 */
template<class T>
bool BinarySearchTree<T>::remove(const T &payloadToRemove) {
	// checkingAgainst stores the node in the list we're currently checking if it's equal to the payload to remove
	// parent stores the parent to that one
	Node *currentNode = root;
	Node **parentConnection = nullptr;
	
//	Node *parent = root;
//	Node *checkingAgainst = root;
	
	while (currentNode != nullptr) {
		if (payloadToRemove < currentNode->payload) {
			// Go to the left side
			parentConnection = &(currentNode->left);
			currentNode = currentNode->left;
		} else if (payloadToRemove > currentNode->right) {
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
				// To delete a node with two children, we replace it with its in order successor. The successor is
				// necessarily a leaf so we don't have to worry about it's children
				Node *inOrderSuccessor = currentNode;
				
				while (inOrderSuccessor->left != nullptr) {
					inOrderSuccessor= inOrderSuccessor->left;
				}
				
				// Move the parent's connection over to the successor if there is a parent
				if (parentConnection != nullptr) {
					*parentConnection = inOrderSuccessor;
				}
				
				// Move the left child from the current node over
				inOrderSuccessor
			}
			
		} else {
			exit(NODE_COMPARISON_FAILURE_EXIT);
		}
	}
	
	// Once we get here, then we know we didn't find the payload. Failure.
	return false;
	
	while (checkingAgainst != nullptr) {
		
		
		if (payloadToRemove < checkingAgainst->payload) {
			parent = checkingAgainst;
			checkingAgainst = checkingAgainst->left;
		} else if (payloadToRemove > checkingAgainst->payload) {
			parent = checkingAgainst;
			checkingAgainst = checkingAgainst->right;
		} else if (payloadToRemove == checkingAgainst->payload) {
			//This is the node we want to delete
			
		} else {
			exit(NODE_COMPARISON_FAILURE_EXIT);
		}
	}
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

template<class T>
T *BinarySearchTree<T>::toArray(int &returnLength) {
	//TODO:
	return nullptr;
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
 * Destructs the node. Shouldn't have to do anything special because no pointers are managed by this Node.
 * @tparam T
 */
template<class T>
BinarySearchTree<T>::Node::~Node() {
	delete left;
	delete right;
	delete payload;
}

/**
 * Finds the in order predecessor of the node. Goes to the left to start and then walks down the right chain until it
 * finds the node closest to but still less than this node.
 * @tparam T
 * @return In order predecessor
 */
template<class T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::Node::inOrderPredecessor() {
	Node *currentNode = left;
	
	while (currentNode->right != nullptr) {
		currentNode = currentNode->right;
	}
	
	// We've hit the end of the right chain, return this node
	return currentNode;
}

/**
 * Finds the in order successor of the node. Goes to the right, and then walks down the left chain to find the
 * smallest value closest to this node.
 * @tparam T
 * @return In order successor
 */
template<class T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::Node::inOrderSuccessor() {
	Node *currentNode = right;
	
	while (currentNode->left != nullptr) {
		currentNode = currentNode->left;
	}
	
	//Hit the end of the right chain
	return currentNode;
}
