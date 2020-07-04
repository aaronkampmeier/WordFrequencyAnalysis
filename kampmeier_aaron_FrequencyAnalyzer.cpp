//
// Created by Aaron Kampmeier on 7/3/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include "kampmeier_aaron_FrequencyAnalyzer.h"

FrequencyAnalyzer::FrequencyAnalyzer() = default;

FrequencyAnalyzer::~FrequencyAnalyzer() = default;

void FrequencyAnalyzer::analyze(const char *inputFilePath, void (*completionHandler)(FrequencyAnalysisResults)) {
	// Create a tree to hold word frequencies in
	auto *wordFrequencies = new BinarySearchTree<FrequencyAnalysisResults::WordFrequency>();
	
	// Open the file
	FILE *inFile = fopen(inputFilePath, "r");
	
	// Set up the file buffer to pull in 4 KB
	char readBuffer[4 * 1000];
	int validReadChunks;
	int processedBuffers = 0;
	
	char *currentReadingWord = nullptr;
	int wordLength;
	
	// Read the file in chunks
	do {
		// Read the file into a buffer
		validReadChunks = fread(readBuffer, sizeof(char), sizeof(readBuffer), inFile); //Returns how much was read
		
		// Walk through the buffer to to delineate words, a word starts with any alphabetic character and ends with
		// a character that is not alpha or '-'
		for (int wordStart=0; wordStart < validReadChunks; wordStart++) {
			// Go until we find an alphanum character
			if (isalpha(readBuffer[wordStart])) {
				// Now move another itr until we find the end which is not alphanum or '-'
				int wordEnd;
				for (wordEnd = wordStart + 1; wordEnd < validReadChunks; wordEnd++) {
					
					if (!(isalpha(readBuffer[wordEnd]) || readBuffer[wordEnd] == '-')) {
						// We've found the end of a word
						wordLength = wordEnd - wordStart;
						// Create a new null terminated string for it
						currentReadingWord = new char[wordLength + 1]; // +1 for \0
						currentReadingWord[wordLength] = '\0';
						// Copy the word into this new word memory location
						memcpy(currentReadingWord, readBuffer + wordStart, wordEnd - wordStart);
						
						// Create a Word Frequency entry to store it in the tree
						FrequencyAnalysisResults::WordFrequency inputWordFreq(currentReadingWord);
						//Add the word freq entry into the tree and store the output. If this is a new word, then
						// the *treeWordFreq should be binary equivalent to the inputWordFreq. If it is already a
						// recorded word, then *treeWordFreq stores the previously added record.
						const FrequencyAnalysisResults::WordFrequency *treeWordFreq = wordFrequencies->insert
								(inputWordFreq);
						
						// In either case, add 1 to the current frequency.
						
						
					}
					
				}
				
				// Check if we reached the end of the buffer
				if (wordEnd == validReadChunks) {
				
				}
			}
		}
		
		processedBuffers++;
	} while (validReadChunks == sizeof(readBuffer)); //Once valid read chunks is less than the whole buffer, we've
	// reached the end of the file
}

/// Frequency Analysis Results

/**
 * Constructs the FrequencyAnalysisResults object. Sets the analyzedOn date to now.
 * @param inputFile
 */
FrequencyAnalyzer::FrequencyAnalysisResults::FrequencyAnalysisResults(const char *inputFile, int fileWordCount,
																	  BinarySearchTree<WordFrequency> *wordFrequencies)
		: _fileWordCount(fileWordCount), analyzedOn(time(nullptr)), inputFile(inputFile), _wordFrequenciesTree(wordFrequencies) {
	
}

/**
 * Frees the inputFile path name and the frequency tree
 */
FrequencyAnalyzer::FrequencyAnalysisResults::~FrequencyAnalysisResults() {
	delete [] inputFile;
	delete _wordFrequenciesTree;
}

int FrequencyAnalyzer::FrequencyAnalysisResults::wordCount() const {
	return _fileWordCount;
}

int FrequencyAnalyzer::FrequencyAnalysisResults::totalUniqueWords() const {
	return _wordFrequenciesTree->length();
}

int FrequencyAnalyzer::FrequencyAnalysisResults::frequencyOf(const char *word) const {
	// To find the word frequency entry in the BST we'll create a dummy word frequency class and go find the payload
	// in the BST with the same word
	WordFrequency searchWord(word);
	
	return _wordFrequenciesTree->find(searchWord)->frequency;
}

const FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency **
FrequencyAnalyzer::FrequencyAnalysisResults::wordsOrderedByFrequency(int &returnLength) {
	//TODO:
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::exportReportTo(char *outputFilePath) {
	//TODO:
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::exportFrequenciesToCSV(char *outputFilePath) {
	//TODO:
}


/// Word Frequency entry

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::WordFrequency(const char *word): hash(_hash(
		reinterpret_cast<const unsigned char *>(word))), word(word) {
	frequency = 0;
}

FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::~WordFrequency() {
	delete [] word;
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::operator<(const
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &other) {
	return this->hash < other.hash;
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::operator>(const
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &other) {
	return this->hash > other.hash;
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency::operator==(const
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &other) {
	return this->hash == other.hash;
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
