//
// Created by Aaron Kampmeier on 6/30/20.
// Copyright © 2020 Aaron Kampmeier. All rights reserved.
// ASU ID: 1217750807
// For Class: CSE 240 with Prof. Selgrad
//

#include <iostream>
#include <ios>
#include <limits>
#include "kampmeier_aaron_FrequencyAnalyzer.h"

using namespace std;

void handleCompletedFileAnalysis(FrequencyAnalyzer::FrequencyAnalysisResults *results);
void exportResultsToFile(FrequencyAnalyzer::FrequencyAnalysisResults *results);
bool scanIntSafe(int &outputVal, char *inputStr = nullptr);

/**
 * Program that performs frequency analysis on an input text file.
 *
 * Can invoke from the command line using: wordAnalyze [filePath]
 * @return
 */
int main(int argc, char **argv) {
	cout << "Welcome to frequency analysis written by Aaron Kampmeier!" << endl;
	
	char *inputFileToAnalyze = new char[100];
	
	// Pull in command line argument file path
	bool validCommandLineInput = false;
	if (argc > 1) {
		strcpy(inputFileToAnalyze, argv[1]);
		validCommandLineInput = true;
	}
	
	bool fileInputSuccess = false;
	
	do {
		// Grab in a file to analyze
		if (!validCommandLineInput) {
			cout << "Enter path to a file to analyze: " << endl;
			cin.getline(inputFileToAnalyze, 100);
		}
		validCommandLineInput = false;
		
		// Analyze
		FrequencyAnalyzer analyzer = FrequencyAnalyzer();
		fileInputSuccess = analyzer.analyze(inputFileToAnalyze, handleCompletedFileAnalysis);
		
		if (!fileInputSuccess) {
			cout << "Error reading in file. Try again." << endl;
		}
	} while (!fileInputSuccess);
	
	delete [] inputFileToAnalyze;
	
	return 0;
}

/**
 * Prints a little summary and prompts for commands to perform.
 * @param results
 */
void handleCompletedFileAnalysis(FrequencyAnalyzer::FrequencyAnalysisResults *results) {
	if (results == nullptr) {
		cout << "There was an error performing the analysis." << endl;
		return;
	}
	
	cout << "Analysis completed. Woohoo!" << endl;
	cout << "Results: " << endl;
	
	cout << *results << endl;
	
	bool exit = false;
	
	
	char *queryWord = new char[50];
	
	do {
		//Ask for some commands to perform:
		// 1. Query frequency of word
		// 2. Export to file
		
		cout << "\nEnter command to perform on analysis: " << endl;
		cout << "1. Query frequency of word\n2. Export to file\n3. Exit" << endl;
		
		int userChoice = -1;
		scanIntSafe(userChoice);
		
		switch (userChoice) {
			case 1:
				// Ask to enter in word
				cout << "Query word to check frequency: " << endl;
				
				// Clear whitespace from buffer
				cin.ignore(numeric_limits<streamsize>::max(),'\n');
				
				cin.getline(queryWord, 50);
				
				cout << queryWord << " occurs " << results->frequencyOf(queryWord) << " times." << endl;
				break;
			case 2:
				exportResultsToFile(results);
				break;
			case 3:
				exit = true;
				break;
			default:
				cout << "Invalid input" << endl;
		}
	} while (!exit);
	
	delete results;
	delete [] queryWord;
}

/**
 * Handles the outputting of the results to the specified file type
 * @param results
 */
void exportResultsToFile(FrequencyAnalyzer::FrequencyAnalysisResults *results) {
	// Ask which file type to output
	cout << "What type of file do you want to output?\n1. Analysis ordered by frequency\n2. CSV" << endl;
	int choice = -1;
	while (!scanIntSafe(choice) || !(choice == 1 || choice == 2)) {
		cout << "Invalid input. Try again." << endl;
	}
	
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	cout << "Enter file name for output file: ./Analysis Outputs/";
	
	char *outputFile = new char[100];
	cin.getline(outputFile, 100);
	char *outputFilePath = concatStrings("Analysis Outputs/", outputFile);
	
	bool success = false;
	switch (choice) {
		case 1:
			success = results->exportReportTo(outputFilePath);
			break;
		case 2:
			success = results->exportFrequenciesToCSV(outputFilePath);
			break;
		default:
			cout << "Unknown file output choice." << endl;
	}
	
	if (success) {
		cout << "Output successful to " << outputFilePath << endl;
	} else {
		cout << "Error outputting file." << endl;
	}
	
	delete [] outputFilePath;
	delete [] outputFile;
}


/**
 * Scans an input string or the std cin and converts it to an int if possible, will fail gracefully if any chars
 * other than digits are present.
 * I ported this function over from my assignment 2 code with just the change of making outputVal a pass by reference
 * parameter, so look there for some more explanation.
 * @param outputVal A reference to the int where the scanned value should be stored.
 * @param inputStr Can be NULL. If provided, input will be scanned from this char array instead of cin.
 * @return Successful or not
 */
bool scanIntSafe(int &outputVal, char *inputStr) {
	char* end; // strtol will put the pointer of where it ended scanning on the input string in here
	
	// If it was not called with an input string then allocate one and read in from cin.
	bool readFromCin = false;
	if (inputStr == nullptr) {
		inputStr = (new char[20]);
		cin >> inputStr;
		readFromCin = true;
	}
	
	// Originally I used this sscanf to convert a str to an int, but after doing some reading I decided strtol is the
	// widely accepted choice for safe conversion.
	int val = (int) strtol(inputStr, &end, 0);
	
	// Check if it succeeded, if the end ptr points to something other than \0, then the conversion didn't go
	// through the entire arg string meaning some characters other than digits got in the way. Consider this
	// invalid input. Thus if end is not pointing to \0 then the conversion failed.
	bool success = *end == '\0';
	
	//Clean up the memory allocated for the array if we created it
	if (readFromCin) {
		delete[] inputStr;
	}
	
	// Set the read integer into the specified output location
	if (success) {
		outputVal = val;
	}
	
	return success;
}
