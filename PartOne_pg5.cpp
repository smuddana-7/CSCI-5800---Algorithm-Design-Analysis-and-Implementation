#include "utility.h"
#include "PartOne.h"

//--------------------------------------PartOne----------------------------------------------------------------------------------//

PartOne::PartOne() {
    this->inputFilename = "";
    this->outputFilename = "";
}

/**
 * Function to process the given text file and output the frequency of each character.
 *
 * @param inputFilePath The path to the input text file.
 * @param outputFilePath The path to the output file where character frequencies will be written.
 */
void PartOne::processFile() {
    // Open the input file for reading
    ifstream inFile(this->inputFilename);
    // Open the output file for writing
    ofstream outFile(this->outputFilename);

    // Check if the input file was successfully opened
    if (!inFile.is_open()) {
        cerr << "Error opening file: " << this->inputFilename << endl;
        return;
    }

    // Vector to store the frequency of each ASCII character
    vector<int> freq(128, 0);
    char ch; // Variable to store each character read from the file

    // Read the file character by character
    while (inFile.get(ch)) {
        // Increment the frequency of the read character
        freq[static_cast<int>(ch)]++;
    }

    // Close the input file
    inFile.close();

    // Write the frequencies to the output file
    for (int i = 0; i < 128; ++i) {
        if (freq[i] > 0) { // Check if the character occurred in the text
            if (i == 10) { // Special handling for line feed character 
                outFile << "LF" << " " << freq[i] << endl;
            }
            else if (i == 32) { // and space character for easier interpretation
                outFile << "SP" << " " << freq[i] << endl;
            }
            else {
                // Write the character and its frequency
                outFile << static_cast<char>(i) << " " << freq[i] << endl;
            }
        }
    }

    // Close the output file
    outFile.close();
}

string PartOne::partOneMethod(string inputFilename, string outputFilename) {
    try {
        this->inputFilename = inputFilename;
        this->outputFilename = outputFilename;
        processFile();
        return "Success";
    }
    catch (const exception& e) {
        (void)e; // avoids the warning about unreferenced local variable
        return "Error";
    }
}




//---------------------------------------Team 5 Gunturu, Khan, Manchikatla, Muddana, Richert---------------------------------------//