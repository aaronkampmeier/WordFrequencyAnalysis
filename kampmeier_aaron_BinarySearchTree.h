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
 * payload data directly in a way that would change it's relative position to other payloads. The BST is free to
 * reorganize nodes / re-structure the tree at any time it sees fit. TODO: Make specifications for modding payloads
 *
 * @tparam T The type that this BST stores instances of in its nodes. T must be comparable, i.e. have the <, >, and ==
 * 	operators defined on it.
 *
 */
template<class T>
class BinarySearchTree {
protected:
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
		Node *left, *right;
		const T payload;
	public:
		explicit Node(const T &newPayload);
		~Node();
		
		//Some operators to compare payloads across payloads
		bool operator<(const Node &other);
		bool operator>(const Node &other);
		bool operator==(const Node &other);
	};
	
	
protected:
	Node *root;
	void deleteBinaryTree(Node *root);
	int lengthOfTree(const Node *rootNode);
	int maxDepthOfTree(const Node *rootNode);
	void addNodeToRepresentationalArray(const Node *node, const T **array, int nodeIndex);
	void addNodeToInOrderArray(Node *node, const T **array, int &currentWriteIndex);
	
public:
	BinarySearchTree();
	~BinarySearchTree();
	
	const T *insert(const T &payload);
	bool remove(const T &payloadToRemove);
	bool elementExists(const T &payload);
	const T *find(const T &payload);
	bool empty();
	int length();
	
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
