//
// Created by Aaron Kampmeier on 7/3/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include "kampmeier_aaron_FrequencyAnalyzer.h"

FrequencyAnalyzer::FrequencyAnalyzer() = default;

FrequencyAnalyzer::~FrequencyAnalyzer() = default;

bool FrequencyAnalyzer::analyze(const char *inputFilePath, void (*completionHandler)(FrequencyAnalysisResults *)) {
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
		return false;
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
	
	// Support for utf-8 char
	unsigned int utf8Char = 0;
	int utf8CharByteIndex = 0;
	int utf8CharByteLength = 0;
	
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
			}
			
			// All of this utf-8 code has been shelved because I realized it introduced more problems than solutions.
			// The nature of it trying to read in a utf-8 char by reading it along with the movement of readIndex
			// means that once it reaches the end of a utf-8 char and realizes that the read char is not actually
			// wanted as part of a word, then there's no way to go back and change wordLength to avoid the utf-8 char
			
//			// Check if it's a utf-8 wide character
//			else if (readBuffer[readIndex] < 0) {
//				// It is probably a utf-8 character so check how many buffer spots to skip
//				// The only utf-8 characters we care about are the general punctuation ones which are three bytes
//				// long so check if that's what we have
//
//				// Check if we are currently reading in a wide character
//				if (utf8CharByteIndex > 0) {
//					// Splice in the next bits
//					unsigned char mask = 0b00111111;
//					unsigned int nextBits = ((unsigned int) readBuffer[readIndex] & mask) << ((utf8CharByteLength -
//							utf8CharByteIndex) * 6);
//
//					if () {
//						utf8CharByteIndex = 0;
//						utf8CharByteLength = 0;
//					}
//				}
//
//				unsigned char mask = 0b11110000;
//				unsigned char maskedChar = readBuffer[readIndex] & mask;
//				if (maskedChar == 0b11100000) {
//					// It is a three byte wide utf-8 char, try to read in the full utf-8 character which should take
//					// up 11 bits
//					utf8Char = 0;
//					utf8CharByteLength = 3;
//
//					// Splice in the first 4 bits into the 12-15 bits in the utf8Char with zero-indexing
//					// First flip the mask and get the actual utf-8 char bits
//					mask = ~mask;
//					unsigned int first4Bits = ((unsigned int) readBuffer[readIndex] & mask) << 12;
//					utf8Char = utf8Char | first4Bits;
//					utf8CharByteIndex = 1;
//					// Go on to read more characters
//				}
//			}
			else {
				// Not part of a word, so the end of it
				if (wordLength > 0 || currentReadingWord != nullptr) {
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
					
					// TESTING-------
//					if (strcmp(currentReadingWord, "LT") == 0) {
//						int j = 0;
//					}
					//--------
					
					
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
	
	return true;
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
	_wordFrequenciesTreeOrdered = nullptr;
}

/**
 * Frees the inputFile path name and the frequency tree
 */
FrequencyAnalyzer::FrequencyAnalysisResults::~FrequencyAnalysisResults() {
	delete [] inputFile;
	delete _wordFrequenciesTree;
	delete _wordFrequenciesTreeOrdered;
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
	BinarySearchTree<WordFrequency> *frequencyOrderedTree = _wordFrequenciesTreeOrdered;
	
	if (frequencyOrderedTree == nullptr) {
		// Generate another tree that stores the WordFrequency objects by their frequency but still has to be unique
		
		auto getUniqueFrequencyKey = [](const WordFrequency &val) -> BinarySearchTree<WordFrequency>::StorageKey {
			typedef BinarySearchTree<WordFrequency>::StorageKey StorageKey;
			//Combine the frequency into the hash value to get a new "freq + hash" storage key that still maintains a
			// unique hash for every word but is now ordered by frequency
			
			//The storage key (a long) for the normal tree is the hash of the word. The hash is unlikely to take up all
			// 8 bytes of the long, so we will cut off the most significant 4 bytes of the hash and splice the integer
			// value of the frequency (4 bytes) into its place. This allows us to order by frequency (the most
			// significant 4 bytes of the storage key) while still having part of the hash trailing the frequency (the
			// least significant 4 bytes of the storage key) to uniquely identify this word and its frequency apart from
			// others.
			
			// The byte index at which the frequency will be spliced into, should be 4 normally. So bytes 0-3 are part of
			// the hash and bytes 4-7 are the frequency.
			int maskStart = sizeof(BinarySearchTree<WordFrequency>::StorageKey) - sizeof(int);
			
			if (maskStart < 2) {
				// The frequency cannot take up the whole storage key, some of the hash must be left
				exit(STORAGE_KEY_HASHING_FAILED_EXIT);
			}
			
			// Mask the hash: cut away the most significant 4 bytes
			long hashMask = ~(((1ul << (sizeof(int) * 8)) - 1) << ((maskStart * 8)));
			long maskedHash = val.hash & hashMask;
			
			// Shift frequency bits into place
			long shiftedFreq = ((unsigned long) val.frequency) << (maskStart * 8);
			
			// Return the frequency spliced into the hash
			return maskedHash | shiftedFreq;
		};
		
		frequencyOrderedTree = new BinarySearchTree<WordFrequency>(getUniqueFrequencyKey);
		
		// Add all of the nodes from the other tree to this tree
		int length;
		const WordFrequency **allFreqs = _wordFrequenciesTree->asInOrderArray(length);
		frequencyOrderedTree->insert(length, allFreqs);
		delete [] allFreqs;
		
		// Save it for future use, so we don't have to regenerate it
		_wordFrequenciesTreeOrdered = frequencyOrderedTree;
		
	}
	
	return frequencyOrderedTree->asInOrderArray(returnLength);
}

std::ostream &operator<<(std::ostream &os, FrequencyAnalyzer::FrequencyAnalysisResults &results) {
	os << "\nAnalysis for file: " << results.inputFile << std::endl;
	
	// Generate time string
	tm *localTime = localtime(&(results.analyzedOn));
	char *timeString = new char[20]; //Length is always 20
	timeString[19] = '\0';
	sprintf(timeString, "%04d-%02d-%02d %02d:%02d:%02d", localTime->tm_year + 1900, localTime->tm_mon + 1,
			localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
	os << "Analyzed on: " << timeString << std::endl;
	
	os << "\nFile word count: " << results.wordCount();
	os << "\nTotal unique word count: " << results.totalUniqueWords() << std::endl;
	
	// Output the top 5 most and least frequently used words
	// To do so just look at the ordered array and take the beginning and ending 5 elements
	int length;
	const FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency **freqs = results.wordsOrderedByFrequency(length);
	
	os << "Lest frequently used words: ";
	for (int i=0; i < 5 && i < length; i++) {
		os << "\n" << freqs[i]->word << ": " << freqs[i]->frequency << " times";
	}
	os << std::endl;
	
	os << "\nMost frequently used words: ";
	for (int i = length-5; i < length; i++) {
		os << "\n" << freqs[i]->word << ": " << freqs[i]->frequency << " times";
	}
	os << std::endl;
	
	delete [] timeString;
	delete [] freqs;
	
	return os;
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::exportReportTo(const char *outputFilePath) {
	FILE *outputFile = fopen(outputFilePath, "w");
	
	if (outputFile == nullptr) {
		return false;
	}
	
	// Print out all the information contained in the results object
	fprintf(outputFile, "Word Frequency Analysis!\nGenerated by a C++ program written by Aaron Kampmeier.");
	
	fprintf(outputFile, "\n\nAnalysis for file: %s", inputFile);
	
	// Generate time string
	tm *localTime = localtime(&analyzedOn);
	char *timeString = new char[20]; //Length is always 20
	timeString[19] = '\0';
	sprintf(timeString, "%04d-%02d-%02d %02d:%02d:%02d", localTime->tm_year + 1900, localTime->tm_mon + 1,
			localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
	fprintf(outputFile, "\nAnalyzed on %s", timeString);
	
	
	fprintf(outputFile, "\n\n\n----------Analysis----------");
	fprintf(outputFile, "\n%-22s %d", "File Word Count:", wordCount());
	fprintf(outputFile, "\n%-22s %d ", "Total Unique Words:", totalUniqueWords());
	
	fprintf(outputFile, "\n\nAll words ordered by frequency:");
	// Get all the words ordered by frequency and put them in here
	int length;
	const WordFrequency **wordsOrdered = wordsOrderedByFrequency(length);
	
	for (int i=0; i < length; i++) {
		fprintf(outputFile, "\n%-14s: %d", wordsOrdered[i]->word, wordsOrdered[i]->frequency);
	}
	
	fprintf(outputFile, "\n\nEnd of analysis.");
	
	fclose(outputFile);
	delete [] wordsOrdered;
	delete [] timeString;
	return true;
}

bool FrequencyAnalyzer::FrequencyAnalysisResults::exportFrequenciesToCSV(const char *outputFilePath) {
	FILE *outputFile = fopen(outputFilePath, "w");
	
	if (outputFilePath == nullptr) {
		return false;
	}
	
	fprintf(outputFile, "word,frequency");
	
	int length;
	const WordFrequency **allWords = allWordFrequencies(length);
	
	for (int i=0; i < length; i++) {
		fprintf(outputFile, "\n%s,%d", allWords[i]->word, allWords[i]->frequency);
	}
	
	fclose(outputFile);
	delete [] allWords;
	return true;
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
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &&wordFreq) noexcept : word(wordFreq.word), hash(wordFreq
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
		FrequencyAnalyzer::FrequencyAnalysisResults::WordFrequency &&wordFreq) noexcept {
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
char *concatStrings(const char *str1, const char *str2) {
	char *newString = new char[strlen(str1) + strlen(str2) + 1];
	strcpy(newString, str1);
	// Second string goes at the end of the first string accomplished using pointer math
	strcpy(newString + strlen(str1), str2);
	return newString;
}