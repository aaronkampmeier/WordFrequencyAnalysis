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
 * Constructor for BinarySearchTree. Creates an empty binary search tree with no root node. Tries to generate an
 * automatic storageKeyFor(T*) function but if it fails you will need to use the other constructor and provide one
 * yourself.
 * @tparam T
 */
template<class T>
BinarySearchTree<T>::BinarySearchTree() {
	// Makes a generated function that tries to take the long representation of a payload for storage keys
	auto generatedKeyFromPayload = [] (const T& val) -> StorageKey {return (StorageKey) (val);};
	_storageKeyFor = generatedKeyFromPayload;
	root = nullptr;
}

/**
 * Constructs a binary search tree that stores payloads. Payloads are organized in the tree using storage keys.
 * @tparam T
 * @param storageKeyFor A function that returns the storage key of a given payload. Should return the exact same
 * storage key every time if the same payload is passed in. A good storage key would probably be the hash of an
 * immutable member on the payload or some id. If you're storing a type that is already numeric (can be converted to
 * an int) then use the default BinarySearchTree() constructor which will generate a storageKeyFor(T*) function for
 * you by attempting to cast the payload.
 */
template<class T>
BinarySearchTree<T>::BinarySearchTree(StorageKey (*storageKeyFor)(const T&)) {
	_storageKeyFor = storageKeyFor;
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
 * Compares two payloads using their storage keys and returns a PayloadComparison
 * @tparam T
 * @param first
 * @param second
 * @return
 */
template<class T>
typename BinarySearchTree<T>::PayloadComparison BinarySearchTree<T>::_comparePayloads(const T &first, const T &second) {
	StorageKey firstKey = _storageKeyFor(first);
	StorageKey secondKey = _storageKeyFor(second);
	
	if (firstKey < secondKey) {
		return BinarySearchTree::LESS_THAN;
	} else if (firstKey > secondKey) {
		return BinarySearchTree::GREATER_THAN;
	} else if (firstKey == secondKey) {
		return BinarySearchTree::EQUAL;
	} else {
		exit(NODE_COMPARISON_FAILURE_EXIT);
	}
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
	// parentConnection (**parentConnection) and the _left child is **parentConnection._left or (*parentConnection)->_left
 	Node **parentConnection = &root;
	while (*parentConnection != nullptr) {
		PayloadComparison comparison = _comparePayloads(payload, ((*parentConnection)->_payload));
		
		 if (comparison == LESS_THAN) {
			// The payload is less than the current node's payload, move down to the left
			parentConnection = &((*parentConnection)->_left);
		} else if (comparison == GREATER_THAN) {
		 	// The payload is greater than the current node's payload, now check the right child
		 	parentConnection = &((*parentConnection)->_right);
		 } else if (comparison == EQUAL) {
		 	// The new payload is equal to this node's payload, so cancel the insertion and just return a pointer to
		 	// this payload
			 return &((*parentConnection)->_payload);
		 } else {
		 	exit(NODE_COMPARISON_FAILURE_EXIT);
		 }
	}
	
	// Reached the end of the list and didn't find any previously added equal payloads, so just add it here
	*parentConnection = new Node(payload);
	return &((*parentConnection)->_payload);
}

/**
 * Inserts an array of payloads. Can be used to import lots of payloads from another tree of the same type.
 * @tparam T
 * @param numOfPayloads
 * @param payloads
 * @return If all inserts were successful.
 */
template<class T>
bool BinarySearchTree<T>::insert(const int numOfPayloads, const T **payloads) {
	bool allSuccess = true;
	for (int i=0; i < numOfPayloads; i++) {
		allSuccess = insert(*payloads[i]) != nullptr && allSuccess;
	}
	
	return allSuccess;
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
		PayloadComparison comparison = _comparePayloads(payloadToRemove, (currentNode->_payload));
		
		if (comparison == LESS_THAN) {
			// Go to the left side
			parentConnection = &(currentNode->_left);
			currentNode = currentNode->_left;
		} else if (comparison == GREATER_THAN) {
			// Go to the right side
			parentConnection = &(currentNode->_right);
			currentNode = currentNode->_right;
		} else if (comparison == EQUAL) {
			// This is the one to delete
			// Three scenarios for deletion:
			// 1. Node is a leaf
			if (currentNode->_left == nullptr && currentNode->_right == nullptr) {
				// Just delete it
				*parentConnection = nullptr;
				delete currentNode;
			}
			// 2. Node has one child
			else if (currentNode->_left == nullptr || currentNode->_right == nullptr) {
				// Connect the parent's connection to its one child
				*parentConnection = currentNode->_left == nullptr ? currentNode->_right : currentNode->_left;
				
				// Delete the current node
				delete currentNode;
			}
			// 3. Node has two children
			else {
				//To delete a node with two children, we replace it with its in order successor. The successor
				// necessarily does not have a left child
				//To find the in order successor, we will jump to the right child and then traverse down the left side
				Node *inOrderSuccessor = currentNode->_right;
				Node **inOrderSuccessorParentConnection = &(currentNode->_right);
				while (inOrderSuccessor->_left != nullptr) {
					inOrderSuccessorParentConnection = &(inOrderSuccessor->_left);
					inOrderSuccessor = inOrderSuccessor->_left;
				}
				
				// Move the parent's connection over to the successor
				*parentConnection = inOrderSuccessor;
				
				// Replace the old parent connection to the successor with the successor's right child
				*inOrderSuccessorParentConnection = inOrderSuccessor->_right;
				
				// Move the left child from the current node over
				inOrderSuccessor->_left = currentNode->_left;
				
				// Move the right child over
				inOrderSuccessor->_right = currentNode->_right;
				
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
		PayloadComparison comparison = _comparePayloads(payload, (checkingAgainst->_payload));
		
		if (comparison == EQUAL) {
			// The specified payload and this node's payload are the same, return this payload pointer
			return &(checkingAgainst->_payload);
		} else if (comparison == LESS_THAN) {
			// Payload is less, go check the left tree
			checkingAgainst = checkingAgainst->_left;
		} else if (comparison == GREATER_THAN) {
			// Payload is bigger, go check right tree
			checkingAgainst = checkingAgainst->_right;
		} else {
			exit(NODE_COMPARISON_FAILURE_EXIT);
		}
	}
	
	// Reached the end of the tree, didn't find it
	return nullptr;
}

/**
 * Returns a representation of the current BST stored in an array structure. Stores the entire tree in an array
 * preserving all of the nodes' connections, not in-order: root stored at index 0, left and right children of a node
 * at index k are at 2k+1 ans 2k+2 respectively.
 * @tparam T
 * @param returnLength A reference variable where the length of the returned array will be set into.
 * @return The array representation of all the Nodes in the BST with pointers to each of the payloads. Can be NULL if
 * there are no elements in the BST.
 */
template<class T>
const T **BinarySearchTree<T>::asRepresentationalArray(int &returnLength) {
	// The length of the rep array is 2^(max depth) + 2^(max depth - 1) + ... + 2^0 which is a geometric series with
	// r=2 and a1 = 1. The number of terms, n, in the series is maxDepth + 1. The geometric series formula is S_n = a1 *
	// (1-r^n) / (1-r).
	const int maxDepth = _maxDepthOfTree(root);
	const int arrLength = 1 * (1 - pow(2,maxDepth + 1)) / (1 - 2);
	if (arrLength < 1) {
		// No elements in the BST, return null
		return nullptr;
	}
	const T **outputArray = new T const *[arrLength];
	// Null out the entire array
	for (int i=0; i < arrLength; i++) {
		outputArray[i] = nullptr;
	}
	
	//This array is a list of pointers to payloads, because values in here can and should be NULL sometimes
	_addNodeToRepresentationalArray(root, outputArray, 0, arrLength);
	
	returnLength = arrLength;
	return outputArray;
}

/**
 * Recursively finds the max depth of a binary tree starting at the root node. The depth of a tree defined by a root
 * node is the greater max depth of the left node tree vs of the right node tree, + 1 for the current node.
 * @tparam T
 * @param rootNode
 * @return
 */
template<class T>
int BinarySearchTree<T>::_maxDepthOfTree(const BinarySearchTree::Node *rootNode) {
	if (rootNode == nullptr) {
		// The depth of a single root node is 0, so the depth of the absence of one (null) must be -1.
		return -1;
	} else {
		
		// Left node depth
		int leftDepth = _maxDepthOfTree(rootNode->_left);
		// Right node depth
		int rightDepth = _maxDepthOfTree(rootNode->_right);
		
		// Return the greater of the two plus one
		return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
	}
}

/**
 * Recursively defined method that adds a pointer to the payload on the specified node to the output array at the
 * specified index. Then calls on its child nodes to do the same at their indices. The indices for the child storage
 * are standard. If a parent node is stored at index k in a representational array, then its left child is stored at
 * 2k+1.
 * @tparam T
 * @param node The parent node to store
 * @param array The output representational array to write to
 * @param nodeIndex The index to store the parent node at
 */
template<class T>
void
BinarySearchTree<T>::_addNodeToRepresentationalArray(const BinarySearchTree::Node *node, const T **array, const int nodeIndex,
													 const int &arraySize) {
	if (node != nullptr) {
		array[nodeIndex] = &(node->_payload);
		_addNodeToRepresentationalArray(node->_left, array, 2 * nodeIndex + 1, arraySize);
		_addNodeToRepresentationalArray(node->_right, array, 2 * nodeIndex + 2, arraySize);
	} else {
		// Don't set it null if it's passed the bounds of this representation
		if (nodeIndex < arraySize) {
			array[nodeIndex] = nullptr;
		}
	}
}

/**
 * Returns an ordered array of pointers to the payloads stored within this BST. As usual the payloads are not to be
 * modified here and the pointers to these payloads are constant.
 * @tparam T
 * @param returnLength A reference variable where the length of the returned array will be set into.
 * @return The in-order collection of payload pointers
 */
template<class T>
const T ** BinarySearchTree<T>::asInOrderArray(int &returnLength) {
	const int arrLength = length();
	const T **outputArray = new T const *[arrLength];
	// Variable to track the current write index in the array
	int arrWriteIndex = 0;
	
	_addNodeToInOrderArray(root, outputArray, arrWriteIndex);
	
	returnLength = arrLength;
	return outputArray;
}

/**
 * Recursively defined method to add the payloads of a node and its children to an array in-order. Because each
 * recursive call has no idea how many array slots its children will take up, we keep a reference to a singular
 * currentWriteIndex variable that increments every time we write to the array.
 * @tparam T
 * @param node
 * @param array The array to write into. Must be at least of length _lengthOfTree(node).
 * @param currentWriteIndex A reference to the write index to start writing from, will be updated as more values are
 * written into the array.
 */
template<class T>
void BinarySearchTree<T>::_addNodeToInOrderArray(Node *node, const T **array, int &currentWriteIndex) {
	if (node != nullptr) {
		_addNodeToInOrderArray(node->_left, array, currentWriteIndex);
		
		array[currentWriteIndex] = &(node->_payload);
		currentWriteIndex++;
		
		_addNodeToInOrderArray(node->_right, array, currentWriteIndex);
	}
}

/**
 * Copies all payloads specified in payloadPointers into a new array and returns it. This offers a way to take the
 * results from calls to asInOrderArray(int&) and asRepresentationalArray(int&) and convert the T* arrays into T
 * arrays with values that can be modified and managed themselves. If payloadPointers contains nulls, they will be
 * replaced with default constructed Ts and indiscernible from valid Payloads, so be careful. Manage that yourself.
 * @tparam T
 * @param payloadPointers
 * @param length The length of the payloadPointers array
 * @return An array of length 'length' with all the payloads specified by payloadPointers in it. In the same order as
 * payloadPointers as well.
 */
template<class T>
T *BinarySearchTree<T>::copyPayloads(const T **payloadPointers, const int &length) {
	T *outputArray = new T[length];
	
	for (int i=0; i < length; i++) {
		if (payloadPointers[i] != nullptr) {
			outputArray[i] = *(payloadPointers[i]);
		} else {
			// This is slightly unpredictable behavior, so the user is warned about it in the comment block above
			// If the pointer here is null, create a default constructed payload and add it
			outputArray[i] = {};
		}
	}
	
	return outputArray;
}

/**
 * Empties out the entire list - removes all nodes and their payloads. All information is destroyed (or invalidated) so
 * be careful.
 * @tparam T
 * @return Successful or not
 */
template<class T>
bool BinarySearchTree<T>::empty() {
	_deleteBinaryTree(root);
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
void BinarySearchTree<T>::_deleteBinaryTree(Node *startingNode) {
	if (startingNode != nullptr) {
		_deleteBinaryTree(startingNode->_left);
		_deleteBinaryTree(startingNode->_right);
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
	return _lengthOfTree(root);
}

/**
 * Recursively defined method that returns the num of elements of a binary tree including the supplied root element.
 * @tparam T
 * @param rootNode The root of the binary tree to count. Note: Could be a child node in some larger tree.
 * @return The number of elements in this tree.
 */
template<class T>
int BinarySearchTree<T>::_lengthOfTree(const BinarySearchTree::Node *rootNode) {
	if (rootNode == nullptr) {
		return 0;
	} else {
		return 1 + _lengthOfTree(rootNode->_left) + _lengthOfTree(rootNode->_right); // +1 for the current node
	}
}

/// Updating payloads

/**
 * Handles the process of updating the information contained inside a payload. As stated in the BST documentation,
 * any update to a payload that would change its position in the tree (considered an 'unsafe update') is discouraged.
 * Use this method to make safe updates to payloads.
 *
 * Payload updates should only be performed on payloads that are class types and only on members of the class type
 * that do not affect its storage key.
 *
 * @tparam T
 * @param payloadToUpdate Pointer to the payload in this tree to update
 * @param payloadUpdateTransaction A function that takes in a payload pointer and updates its value. This function will be
 * monitored and if it makes an unsafe update that changes the relative position of the payload, then refactoring
 * will occur. Refactoring is a process in which the updated payload will be removed from the tree and then
 * re-inserted back into its new, correct spot.
 * @return Whether an update was performed successfully. False if the payload was not found in the tree.
 */
template<class T>
bool BinarySearchTree<T>::updatePayload(const T &payloadToUpdate, void (*payloadUpdateTransaction)(T &)) {
	
	// Walk through the tree to find the node to update
	Node *nodeToUpdate = root;
	bool nodeFound = false;
	do {
		if (nodeToUpdate == nullptr) {
			// Node not found in the tree at all
			return false;
		}
		
		PayloadComparison comparison = _comparePayloads(payloadToUpdate, nodeToUpdate->_payload);
		switch (comparison) {
			case LESS_THAN:
				nodeToUpdate = nodeToUpdate->_left;
				break;
			case GREATER_THAN:
				nodeToUpdate = nodeToUpdate->_right;
				break;
			case EQUAL:
				nodeFound = true;
				break;
		}
	} while (!nodeFound);
	
	// Get the storage key before update
	StorageKey beforeUpdateKey = _storageKeyFor(payloadToUpdate);
	
	// Update the payload
	payloadUpdateTransaction(nodeToUpdate->_unsafeMutablePayload);
	
	// Check the storage key did not change
	if (_storageKeyFor(payloadToUpdate) != beforeUpdateKey) {
		// If they changed the relative position of the payload, we must delete the payload and re-insert it
		T payload = payloadToUpdate;
		remove(payload);
		insert(payload);
		return true;
	} else {
		return true;
	}
}


/**
 * Generates a new Node with payload and both children connections set to null.
 * @tparam T
 * @param payload
 */
template<class T>
BinarySearchTree<T>::Node::Node(const T &newPayload): _unsafeMutablePayload(newPayload), _payload(_unsafeMutablePayload) {
	_left = _right = nullptr;
}

/**
 * Destructs the node. Shouldn't have to do anything special because no pointers are managed by this Node. Does not
 * cascade delete nodes below it.
 * @tparam T
 */
template<class T>
BinarySearchTree<T>::Node::~Node() = default;
