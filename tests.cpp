//
// Created by Aaron Kampmeier on 7/3/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>
#include <dirent.h>
#include "kampmeier_aaron_BinarySearchTree.h"
#include "kampmeier_aaron_FrequencyAnalyzer.h"

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
	
	
	cout << "\nTesting with Struct payload..." << endl;
	
	// Test the tree with structs
	struct MyStruct {
		char letter;
		int value;
	};
	
	auto storageKeyFunc = [](const MyStruct &val) -> BinarySearchTree<MyStruct>::StorageKey {return val.letter;};
	auto *structTree = new BinarySearchTree<MyStruct>(storageKeyFunc);
	structTree->insert({'a', 5});
	structTree->insert({'z',1});
	structTree->insert({'c',3});
	structTree->insert({'q',22});
	
	cout << "Val of 'z': " << structTree->find({'z'})->value << endl;
	structTree->updatePayload({'z'}, [](MyStruct &val) {val.value = -3;});
	cout << "Val of 'z': " << structTree->find({'z'})->value << endl;
	
	cout << "'b' exists: " << structTree->elementExists({'b'}) << endl;
	cout << "'A' exists: " << structTree->elementExists({'A'}) << endl;
	cout << "'a' exists: " << structTree->elementExists({'a'}) << endl;
	
	const MyStruct **payloads = structTree->asInOrderArray(length);
	for (int i=0; i < length; i++) {
		cout << payloads[i]->letter << ": " << payloads[i]->value << ", ";
	}
	cout << endl;
	
	delete [] payloads;
	delete structTree;
	
	/// Test frequency analysis
	cout << "\n\nTesting frequency analysis..." << endl;
	FrequencyAnalyzer analyzer;
	
	
	char testDir[] = "Tests/Cases/";
	// Since I couldn't get std::filesystem to work I am using these POSIX file commands from here: https://pubs.opengroup.org/onlinepubs/7908799/xsh/readdir.html
	DIR *directoryPtr = opendir(testDir);
	dirent *dirEntry;
	errno = 0;
	
	while ((dirEntry = readdir(directoryPtr)) != nullptr) {
		// Skip the "." and ".." entries in the directory if they exist
		if ((strcmp(dirEntry->d_name, ".") != 0) && (strcmp(dirEntry->d_name, "..") != 0)) {
			
			char *testFile = concatStrings(testDir, dirEntry->d_name);
			
			void (*completionHandler)(FrequencyAnalyzer::FrequencyAnalysisResults*) = []
					(FrequencyAnalyzer::FrequencyAnalysisResults *results) {
				if (results == nullptr) {
					cout << "Analysis failed" << endl;
					return;
				}
				
				cout << "Completed frequency analysis on file " << results->getInputFilePath() << endl;
				
				cout << "Frequency of 'the': " << results->frequencyOf("the") << endl;
				cout << "Frequency of 'baz': " << results->frequencyOf("baz") << endl;
				cout << "Frequency of 'quick': " << results->frequencyOf("quick") << endl;
				cout << "Frequency of 'dog': " << results->frequencyOf("dog") << endl;
				cout << "Frequency of 'pglaf': " << results->frequencyOf("pGlAF") << endl;
				cout << "Frequency of '3/4': " << results->frequencyOf("3/4") << endl;
				
				// Output to a file
				char testOutputDir[] = "Tests/Testing Output/";
				const char *inputFile = (results->getInputFilePath()) + 12;
				const char *outputFilePath = concatStrings(testOutputDir, inputFile);
				
				// Output to file
				const char *outputAnalysisPath = concatStrings(outputFilePath, ".analysis.txt");
				results->exportReportTo(outputAnalysisPath);
				
				// Output to csv
				const char *outputCsvPath = concatStrings(outputFilePath, ".csv");
				results->exportFrequenciesToCSV(outputCsvPath);
				
//				int returnLength;
//				auto allWords = results->allWordFrequencies(returnLength);
//				cout << "\nFrequency of all words: " << endl;
//				for (int i=0; i < returnLength; i++) {
//					cout << allWords[i]->word << ": " << allWords[i]->frequency << "" << endl;
//				}
//
//				cout << "\nWords ordered by Frequency: " << endl;
//				auto wordsByFreq = results->wordsOrderedByFrequency(returnLength);
//				for (int i=0; i < returnLength; i++) {
//					cout << wordsByFreq[i]->word << ": " << wordsByFreq[i]->frequency << "" << endl;
//				}

//				delete [] allWords;
				delete [] outputFilePath;
				delete [] outputCsvPath;
				delete [] outputAnalysisPath;
				delete results;
			};
			
			analyzer.analyze(testFile, completionHandler);
			
			delete [] testFile;
		}
	}
	
	if (errno != 0) {
		// There was an error reading the directory
		cerr << "There was an error reading the " << testDir << " directory: " << errno << endl;
	}
	
	closedir(directoryPtr);
	
	cout << "Tests completed, manually check output files now." << endl;
	
	return 0;
}
