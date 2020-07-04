//
// Created by Aaron Kampmeier on 6/30/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#ifndef ASSIGNMENT_4_KAMPMEIER_AARON_BINARYSEARCHTREE_H
#define ASSIGNMENT_4_KAMPMEIER_AARON_BINARYSEARCHTREE_H

#define NODE_COMPARISON_FAILURE_EXIT 1

#include <cmath>

/**
 * Manages a Binary Search tree with nodes that store a templated payload type. Nodes are hidden to the user; the user
 * cannot access or manipulate them directly. The only public interfaces operate on "payloads" which are the values
 * nodes store in this BST.
 *
 * There should be no assumed organization of how the BST stores its payloads nor should there be any modification of
 * payload data directly in a way that would change its relative position to other payloads. The BST is free to
 * reorganize nodes / re-structure the tree at any time it sees fit.
 *
 * Modifying Payloads: The only changes allowed to payloads are ones that don't change its relative position to other
 * payloads. If any payload needs to be modified in a way that would change its position in the tree, it should be
 * removed and re-inserted. To make a safe change to a payload one must use the updatePayload(T*, void(T*)). See
 * method documentation for more information.
 *
 * @tparam T The type that this BST stores instances of in its nodes. T must be comparable, i.e. have the <, >, and ==
 * 	operators defined on it.
 *
 */
template<class T>
class BinarySearchTree {
protected:
	typedef int StorageKey;
	/**
	 * Represents a singular node in the binary search tree. Nodes hold some payload and point to two other nodes, a
	 * left and a right.
	 *
	 * A node is intended to be a simple wrapper around a payload with connections to the next left and right
	 * children. Nodes are married to their payloads at instantiation and never separated until destruction. In other
	 * words, payloads will never be shuffled in and out of nodes or around the tree separate from a node. If a
	 * payload needs to move, so will it's enclosing node. If a payload is being deleted, so will it's node. To this
	 * end, there is no need for the payload to be referenced by a pointer (T*) as that only implies the payload
	 * could be changed separate from the node.
	 */
	class Node {
		friend class BinarySearchTree;
	private:
		/**
		 * Not to be used except for within the updatePayload method on BinarySearchTree.
		 */
		T _unsafeMutablePayload;
	protected:
		Node *_left, *_right;
		const T _payload;
	public:
		explicit Node(const T &newPayload);
		~Node();
	};
	
	/**
	 * Represents the comparison between one payload and another
	 */
	enum PayloadComparison {GREATER_THAN = 1, LESS_THAN = -1, EQUAL = 0};
	
private:
	int (*_storageKeyFor)(T *payload);
	
protected:
	Node *root;
	void _deleteBinaryTree(Node *root);
	int _lengthOfTree(const Node *rootNode);
	int _maxDepthOfTree(const Node *rootNode);
	void _addNodeToRepresentationalArray(const Node *node, const T **array, int nodeIndex);
	void _addNodeToInOrderArray(Node *node, const T **array, int &currentWriteIndex);
	
	PayloadComparison _comparePayloads(T *first, T *second);
	
public:
	BinarySearchTree();
	explicit BinarySearchTree(StorageKey (*storageKeyFor)(T *payload));
	~BinarySearchTree();
	
	const T *insert(const T &payload);
	bool remove(const T &payloadToRemove);
	bool elementExists(const T &payload);
	const T *find(const T &payload);
	bool empty();
	int length();
	
	// Modifying payloads
	bool updatePayload(T *payloadToUpdate, void(*payloadUpdater)(T *payloadToUpdate));
	
	// Exporting
	const T **asRepresentationalArray(int &returnLength);
	const T **asInOrderArray(int &returnLength);
	T *copyPayloads(const T **payloadPointers, const int &length);
};

//A little trick I picked up from https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp
// Puts all the cpp definitions in this file to fix the template issue while still allowing me to treat it as a
// separate file
#include "kampmeier_aaron_BinarySearchTree.cpp"

#endif //ASSIGNMENT_4_KAMPMEIER_AARON_BINARYSEARCHTREE_H
