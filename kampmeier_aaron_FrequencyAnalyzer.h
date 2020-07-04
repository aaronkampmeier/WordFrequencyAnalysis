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

private:
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
			const unsigned long hash;
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
			
			/**
			 * Compares this word frequency with another one. Compares them by their hash to check if their words are
			 * equal. Note that the frequency does not affect equality.
			 * @param other
			 * @return
			 */
			bool operator<(const WordFrequency &other);
			/**
			 * Compares this word frequency with another one. Compares them by their hash to check if their words are
			 * equal. Note that the frequency does not affect equality.
			 * @param other
			 * @return
			 */
			bool operator>(const WordFrequency &other);
			/**
			 * Compares this word frequency with another one. Compares them by their hash to check if their words are
			 * equal. Note that the frequency does not affect equality.
			 * @param other
			 * @return
			 */
			bool operator==(const WordFrequency &other);
			
		};
		
	private:
		BinarySearchTree<WordFrequency> *_wordFrequenciesTree;
		const int _fileWordCount;
		
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
		 * Returns the frequency of the specified word
		 * @param word The word to check the frequency of
		 * @return
		 */
		int frequencyOf(const char *word) const;
		
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
public:
	FrequencyAnalyzer();
	~FrequencyAnalyzer();
	
	/**
	 * Begins the frequency analysis results on the input file. Can potentially run asynchronously and it should not be
	 * assumed that when the function returns the analysis is over. When the analysis is completed, completionHandler
	 * will be called with the results object which can be further queried for insight.
	 * @param inputFilePath The path to the file to perform analysis on
	 * @param completionHandler A pointer to a function that just takes in a FrequencyAnalysisResults object.
	 */
	void analyze(const char *inputFilePath, void (*completionHandler)(FrequencyAnalysisResults));
};


#endif //ASSIGNMENT_4_KAMPMEIER_AARON_FREQUENCYANALYZER_H
