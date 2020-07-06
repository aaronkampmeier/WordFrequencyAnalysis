//
// Created by Aaron Kampmeier on 7/3/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include "kampmeier_aaron_FrequencyAnalyzer.h"

FrequencyAnalyzer::FrequencyAnalyzer() = default;

FrequencyAnalyzer::~FrequencyAnalyzer() = default;

void FrequencyAnalyzer::analyze(const char *inputFilePath, void (*completionHandler)(FrequencyAnalysisResults *)) {
	// Create a tree to hold word frequencies in with a lambda function that generates BST storage keys from the
	// WordFrequency's hash
	auto storageKeyCalc = [] (const FrequencyAnalysisResults::WordFrequency &freq) ->
			BinarySearchTree<FrequencyAnalysisResults::WordFrequency>::StorageKey {return freq.hash;};
	auto *wordFrequencies = new BinarySearchTree<FrequencyAnalysisResults::WordFrequency>(storageKeyCalc);
	
	// Open the file
	FILE *inFile = fopen(inputFilePath, "r");
	
	if (inFile == nullptr) {
		// Log: failed
		completionHandler(nullptr);
		return;
	}
	
	// Set up the file buffer to pull in 4 KB
	char readBuffer[4 * 1000];
	int validReadChunks;
	int processedBuffers = 0;
	
	// Tracks the current word being read in, wordLength stores the current length of the word being parsed in the
	// buffer. 0 indicates there is no word currently being parsed. wordLength and the strlen of currentReadingWord
	// can be different such as when a word spans a buffer change.
	char *currentReadingWord = nullptr;
	int wordLength = 0;
	
	int fileWordCount = 0;
	
	// Read the file in chunks
	do {
		// Read the file into a buffer
		validReadChunks = fread(readBuffer, sizeof(char), sizeof(readBuffer), inFile); //Returns how much was read
		
		// Walk through the buffer to to delineate words, a word starts with any alphabetic character and ends with
		// a character that is not alpha or an apostrophe
		for (int readIndex=0; readIndex < validReadChunks; readIndex++) {
			// Ignore case
			readBuffer[readIndex] = toupper(readBuffer[readIndex]);
			
			// Check the character we are at
			if (isalpha(readBuffer[readIndex]) || readBuffer[readIndex] == '\'') {
				// This could be the start or middle of a word, increment word length by 1
				wordLength++;
			} else {
				// Not part of a word, so the end of it
				if (wordLength > 0) {
					// Check if there is currently a word being read, possibly left over from a buffer change
					if (currentReadingWord != nullptr) {
						// Add the left-over word to the word currently being read in from buffer
						char *newWord = new char[wordLength + 1];
						newWord[wordLength] = '\0';
						memcpy(newWord, readBuffer + readIndex - wordLength, wordLength);
						
						char *concatted = concatStrings(currentReadingWord, newWord);
						delete [] currentReadingWord;
						delete [] newWord;
						currentReadingWord = concatted;
					} else {
						// Create a new null terminated string for it
						currentReadingWord = new char[wordLength + 1]; // +1 for \0
						currentReadingWord[wordLength] = '\0';
						// Copy the word into this new word memory location
						memcpy(currentReadingWord, readBuffer + readIndex - wordLength, wordLength);
					}
					
					_registerWordWithTree(currentReadingWord, wordFrequencies);
					fileWordCount++;
					
					// Reset our word looking
					delete [] currentReadingWord;
					wordLength = 0;
					currentReadingWord = nullptr;
				} else {
					// Not reading in word, do nothing
				}
			}
		}
		
		// Check if we're currently reading a word
		if (wordLength > 0) {
			// Currently reading a word but need to make a buffer transition so store what we got
			currentReadingWord = new char[wordLength + 1];
			currentReadingWord[wordLength] = '\0';
			memcpy(currentReadingWord, readBuffer + validReadChunks - wordLength, wordLength);
			wordLength = 0;
		}
		
		processedBuffers++;
	} while (validReadChunks == sizeof(readBuffer)); //Once valid read chunks is less than the whole buffer, we've
	// reached the end of the file
	
	// Check if there's a leftover word from the end of the buffer
	if (currentReadingWord != nullptr) {
		_registerWordWithTree(currentReadingWord, wordFrequencies);
		fileWordCount++;
		delete [] currentReadingWord;
		currentReadingWord = nullptr;
	}
	
	// Okay now the entire word frequencies tree is built, we can return a results object
	auto *results = new FrequencyAnalysisResults(inputFilePath, fileWordCount, wordFrequencies);
	
	completionHandler(results);
}

void FrequencyAnalyzer::_incrementWordFrequencyByOne(
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &wordFrequency) {
	wordFrequency.incrementByOne();
}

void
FrequencyAnalyzer::_registerWordWithTree(char *word, BinarySearchTree<FrequencyAnalysisResults::WordFrequency> *tree) {
	// Create a Word Frequency entry to store it in the tree
	FrequencyAnalysisResults::WordFrequency inputWordFreq(word);
	
	// Try to add 1 to the word freq entry in the tree
	bool successfulUpdate = tree->updatePayload(inputWordFreq, _incrementWordFrequencyByOne);
	// Check if successful
	if (!successfulUpdate) {
		// The payload wasn't found in the tree, so add it
		const auto treeWordFreq = tree->insert(inputWordFreq);
		
		// Now update it by one to mark the first entry
		tree->updatePayload(*treeWordFreq, _incrementWordFrequencyByOne);
	}
}

/// Frequency Analysis Results

/**
 * Constructs the FrequencyAnalysisResults object. Sets the analyzedOn date to now.
 * @param inputFile
 */
FrequencyAnalyzer::FrequencyAnalysisResults::FrequencyAnalysisResults(const char *inputFile, int fileWordCount,
																	  BinarySearchTree<WordFrequency> *wordFrequencies)
		: _fileWordCount(fileWordCount), analyzedOn(time(nullptr)), inputFile(copyString(inputFile)),
		_wordFrequenciesTree
		(wordFrequencies) {
	
}

/**
 * Frees the inputFile path name and the frequency tree
 */
FrequencyAnalyzer::FrequencyAnalysisResults::~FrequencyAnalysisResults() {
	delete [] inputFile;
	delete _wordFrequenciesTree;
}

const char *FrequencyAnalyzer::FrequencyAnalysisResults::copyString(const char *inputString) {
	char *newPath = new char[strlen(inputString) + 1];
	strcpy(newPath, inputString);
	return newPath;
}

int FrequencyAnalyzer::FrequencyAnalysisResults::wordCount() const {
	return _fileWordCount;
}

int FrequencyAnalyzer::FrequencyAnalysisResults::totalUniqueWords() const {
	return _wordFrequenciesTree->length();
}

int FrequencyAnalyzer::FrequencyAnalysisResults::frequencyOf(const char *word) const {
	char *uppercaseWord = new char[strlen(word) + 1];
	uppercaseWord[strlen(word)] = '\0';
	
	for (int i=0; i < strlen(word); i++) {
		uppercaseWord[i] = toupper(word[i]);
	}
	
	// To find the word frequency entry in the BST we'll create a dummy word frequency class and go find the payload
	// in the BST with the same word
	WordFrequency searchWord(uppercaseWord);
	
	const FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency *foundResult = _wordFrequenciesTree->find
			(searchWord);
	
	delete [] uppercaseWord;
	if (foundResult == nullptr) {
		return 0;
	} else {
		return foundResult->frequency;
	}
}

const FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency **
FrequencyAnalyzer::FrequencyAnalysisResults::allWordFrequencies(int &returnLength) {
	return _wordFrequenciesTree->asInOrderArray(returnLength);
}

const FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency **
FrequencyAnalyzer::FrequencyAnalysisResults::wordsOrderedByFrequency(int &returnLength) {
	// Generate another tree that stores the WordFrequency objects by their frequency but still has to be unique
	auto getFrequency = [](const WordFrequency &val) -> BinarySearchTree<WordFrequency>::StorageKey {
	
	};
	auto frequencyOrderedTree = new BinarySearchTree<WordFrequency>(getFrequency);
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::exportReportTo(char *outputFilePath) {
	//TODO:
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::exportFrequenciesToCSV(char *outputFilePath) {
	//TODO:
}


/// Word Frequency entry

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::WordFrequency(const char *word): hash(_hash(
		reinterpret_cast<const unsigned char *>(word))), word
		(FrequencyAnalyzer::FrequencyAnalysisResults::copyString(word)) {
	frequency = 0;
}

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::~WordFrequency() {
	delete [] word;
}

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::WordFrequency(
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &&wordFreq): word(wordFreq.word), hash(wordFreq
		.hash), frequency(wordFreq.frequency) {
	// Remove the old reference to the word so they can't delete it
	wordFreq.word = nullptr;
}

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::WordFrequency(
		const FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &wordFreq) {
	char *copiedWord = new char[strlen(wordFreq.word) + 1];
	strcpy(copiedWord, wordFreq.word);
	this->word = copiedWord;
	
	this->hash = wordFreq.hash;
	this->frequency = wordFreq.frequency;
}

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &
FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::operator=(
		const FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &wordFreq) {
	// Checking for self-assignment
	if (&wordFreq == this) return *this;
	
	// Free current word
	delete [] word;
	
	char *copiedWord = new char[strlen(wordFreq.word) + 1];
	strcpy(copiedWord, wordFreq.word);
	this->word = copiedWord;
	
	this->hash = wordFreq.hash;
	this->frequency = wordFreq.frequency;
	
	return *this;
}

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &
FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::operator=(
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &&wordFreq) {
	// Checking for self-assignment
	if (&wordFreq == this) return *this;
	
	// Release our current word
	delete [] word;
	
	// Move ownership over
	word = wordFreq.word;
	wordFreq.word = nullptr;
	
	return *this;
}

unsigned long FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::_hash(const unsigned char *word) {
	// Hashes the input word using the djb2 hash method which can be found here: http://www.cse.yorku.ca/~oz/hash.html
	
	unsigned long hash = 5381;
	int c;
	
	while (c = *word++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	
	return hash;
}

void FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::incrementByOne() {
	frequency++;
}


/**
 * Creates a new char[] and copies the two specified strings to it; concat str1 + str2.
 * @param str1
 * @param str2
 * @return A new char[]
 */
char *concatStrings(char *str1, char *str2) {
	char *newString = new char[strlen(str1) + strlen(str2) + 1];
	strcpy(newString, str1);
	// Second string goes at the end of the first string accomplished using pointer math
	strcpy(newString + strlen(str1), str2);
	return newString;
}