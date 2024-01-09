// --------------------------------------------------
// ------- CS580 Program 5 - Team 5 - Frannie Richert
// --------------------------------------------------


#include "utility.h"
#include "PartTwo.h"

#ifndef  PARTTHREE_H
#define  PARTTHREE_H


class PartThree {

public:

	string codeTableFileName;
	string clearTextFileName;
	string codedOutputFileName;
	string decodedOutputFileName;

	map<string, string> charstr_huffman_map; // hashmap---> key is char string repr, value is huffmanCode
	map<string, string> huffman_charstr_map; // hashmap---> key is huffman code, value is char string rep

	PartTwo partTwoObj;

	PartThree(PartTwo partTwoObj);

	string partThreeMethod(string codeTableFileName,
		string clearTextFileName,
		string codedOutputFileName,
		string decodedOutputFileName);

private:
	void readCodeTable();
	void writeCodedClearTextFile();
	void writeDecodedTextFile();

	// Code source URL in .cpp file for these two functions
	void compareFiles();
	bool equalFiles(ifstream& in1, ifstream& in2);
};

#endif