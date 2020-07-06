//
// Created by Aaron Kampmeier on 7/3/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#ifndef ASSIGNMENT_4_KAMPMEIER_AARON_FREQUENCYANALYZER_H
#define ASSIGNMENT_4_KAMPMEIER_AARON_FREQUENCYANALYZER_H

#include <ctime>
#include <cstring>
#include <iostream>
#include "kampmeier_aaron_BinarySearchTree.h"

/**
 * Performs a frequency analysis of words in an input text file. The FrequencyAnalyzer will present the
 * following stats by default when analyze(char*) is called:
 *  - Total Words
 *  - Total Unique Words
 *  - Most and least frequently used words
 *
 * To begin frequency analysis, you must call analyze(char*) with the input file.
 */
class FrequencyAnalyzer {

public:
	/**
	 * Represents the results of a FrequencyAnalyzer analysis
	 */
	class FrequencyAnalysisResults {
		friend class FrequencyAnalyzer;
	private:
		
		/**
		 * Represents the pairing of one word and the number of its occurrences in the input file. Hash stores a hash
		 * of the word which will be used to compare it to others in the binary search tree.
		 */
		class WordFrequency {
		private:
			/**
			 * Helper method that hashes a char* null-terminated string.
			 * @param word
			 * @return The hash of the word.
			 */
			static unsigned long _hash(const unsigned char *word);
			
		public:
			const char *word;
			unsigned long hash;
			int frequency;
			
			/**
			 * Sets in the word and creates a hash of it to store in hash. Starts frequency off at 0. Expects a
			 * null-terminated string for word.
			 *
			 * We use hash values in WordFrequency to better help with even distribution in the BST and to get an
			 * easy way to compare two strings against each other.
			 * @param word Pointer to the word char[]. The WordFrequency class will take over memory management of it
			 * . No one else should modify the word's memory directly.
			 */
			explicit WordFrequency(const char *word);
			~WordFrequency();
			
			/// Move handling
			/**
			 * Move constructor. Moves management of word over to this object from wordFreq.
			 * @param wordFreq
			 */
			WordFrequency(WordFrequency &&wordFreq);
			
			/**
			 * Move assignment operator. Moves just like the move constructor does.
			 * @param wordFreq
			 * @return
			 */
			WordFrequency &operator=(WordFrequency &&wordFreq);
			
			// Copy handling
			WordFrequency(const WordFrequency &wordFreq);
			WordFrequency &operator=(const WordFrequency &wordFreq);
			
			/**
			 * Increments the frequency value by one to signal another word instance found
			 */
			void incrementByOne();
			
		};
		
	private:
		BinarySearchTree<WordFrequency> *_wordFrequenciesTree;
		const int _fileWordCount;
		
		static const char *copyString(const char *inputString);
		
	public:
		const char * const inputFile;
		const time_t analyzedOn;
		
	public:
		FrequencyAnalysisResults(const char *inputFile, int fileWordCount,
								 BinarySearchTree<WordFrequency> *wordFrequencies);
		~FrequencyAnalysisResults();
		
		/**
		 * Returns the total number of processed words
		 * @return
		 */
		int wordCount() const;
		
		/**
		 * Returns the total number of unique words
		 * @return
		 */
		int totalUniqueWords() const;
		
		/**
		 * Returns the frequency of the specified word. Case-insensitive.
		 * @param word The word to check the frequency of
		 * @return
		 */
		int frequencyOf(const char *word) const;
		
		/**
		 * Returns all of the words and their frequencies in the file in no specific order.
		 * @param returnLength A reference to a variable that will store the length of the returned array.
		 * @return An array of WordFrequency pointers.
		 */
		const WordFrequency **allWordFrequencies(int &returnLength);
		
		/**
		 * Returns an array of pointers to word frequency objects in order of increasing frequency
		 * @param returnLength
		 * @return
		 */
		const WordFrequency **wordsOrderedByFrequency(int &returnLength);
		
		/**
		 * Exports a full report of the frequency analysis to a file for analysis.
		 * @param outputFilePath
		 * @return Successful
		 */
		bool exportReportTo(char *outputFilePath);
		
		/**
		 * Exports all the frequencies of words to a CSV file.
		 * @param outputFilePath
		 * @return Successful
		 */
		bool exportFrequenciesToCSV(char *outputFilePath);
	};
	
private:
	/**
	 * Increments the frequency of the specified wordFrequency by one
	 * @param wordFrequency
	 */
	static void _incrementWordFrequencyByOne(FrequencyAnalysisResults::WordFrequency &wordFrequency);
	
	/**
	 * Registers a null-terminated string inside the tree. If it does not exist in the tree, an entry with
	 * frequency=1 is added, if it does exist, then frequency is incremented by one.
	 * @param word
	 * @param tree
	 */
	static void _registerWordWithTree(char *word, BinarySearchTree<FrequencyAnalysisResults::WordFrequency> *tree);
public:
	FrequencyAnalyzer();
	~FrequencyAnalyzer();
	
	/**
	 * Begins the frequency analysis results on the input file. Can potentially run asynchronously and it should not be
	 * assumed that when the function returns the analysis is over. When the analysis is completed, completionHandler
	 * will be called with the results object which can be further queried for insight.
	 * @param inputFilePath The path to the file to perform analysis on
	 * @param completionHandler A pointer to a function that just takes in a FrequencyAnalysisResults object pointer.
	 * The Results pointer can be NULL if the analysis failed.
	 */
	void analyze(const char *inputFilePath, void (*completionHandler)(FrequencyAnalysisResults *));
};


char *concatStrings(char *str1, char *str2);

#endif //ASSIGNMENT_4_KAMPMEIER_AARON_FREQUENCYANALYZER_H
