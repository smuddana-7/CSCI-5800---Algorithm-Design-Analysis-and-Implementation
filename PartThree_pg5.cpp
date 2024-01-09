#include "utility.h"
#include "PartTwo.h"
#include "PartThree.h"


PartThree::PartThree(PartTwo partTwoObj) {
	this->partTwoObj = partTwoObj;
}


// ---------------------------------------------------
// Source:https://stackoverflow.com/questions/6163611/compare-two-files
// ---------------------------------------------------
void PartThree::compareFiles() {
	ifstream in1(this->clearTextFileName);
	ifstream in2(this->decodedOutputFileName);
	if (equalFiles(in1, in2)) {
		cout << "      Part 3 --> SUCCESS!  Clear text and decoded files are equal!" << endl;
	}
	else {
		cout << "      Part 3 --> WARNING!  Clear text and decoded files are NOT EQUAL! Fix!" << endl;
	}
	in1.close();
	in2.close();
}

bool PartThree::equalFiles(ifstream& in1, ifstream& in2) {
	ifstream::pos_type size1, size2;
	size1 = in1.seekg(0, ifstream::end).tellg();
	in1.seekg(0, ifstream::beg);
	size2 = in2.seekg(0, ifstream::end).tellg();
	in2.seekg(0, ifstream::beg);
	if (size1 != size2)
		return false;
	static const size_t BLOCKSIZE = 4096;
	size_t remaining = size1;
	while (remaining) {
		char buffer1[BLOCKSIZE], buffer2[BLOCKSIZE];
		size_t size = std::min(BLOCKSIZE, remaining);
		in1.read(buffer1, size);
		in2.read(buffer2, size);
		if (0 != memcmp(buffer1, buffer2, size))
			return false;
		remaining -= size;
	}
	return true;
}
// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------


void PartThree::readCodeTable() {
	ifstream codeTableFile(this->codeTableFileName);
	string charStr;
	string huffmanCode;
	while (codeTableFile >> charStr >> huffmanCode) {
		this->charstr_huffman_map[charStr] = huffmanCode;
		this->huffman_charstr_map[huffmanCode] = charStr;
	}
}


void PartThree::writeCodedClearTextFile() {
	ifstream clearTextFile(this->clearTextFileName);
	ofstream codedTextFile(this->codedOutputFileName);
	char currChar;
	string currCharStr;
	string currHuffmanCode;
	while (clearTextFile.get(currChar)) {

		if ((int)currChar == 10) { // If line feed (ascii 10) then "LF"
			currCharStr = "LF";
		}
		else if ((int)currChar == 32) { // If space (ascii 32) then "SP"
			currCharStr = "SP";
		}
		else {
			currCharStr = currChar;
		}

		// --- 1) Get Huffman code
		currHuffmanCode = this->charstr_huffman_map.find(currCharStr)->second;
		// --- 2) Write Huffman code into coded.txt file right after previously written item (if any)
		codedTextFile << currHuffmanCode;
	}
	clearTextFile.close();
	codedTextFile.close();
}


char getCharFromStr(string charStr) {
	int ascii;
	if (charStr == "LF") { // If line feed (ascii 10) then "LF"
		ascii = 10;
	}
	else if (charStr == "SP") { // If space (ascii 32) then "SP"
		ascii = 32;
	}
	else {
		ascii = (int)charStr[0];
	}
	return ((char)ascii);
}


void PartThree::writeDecodedTextFile() {

	ifstream codedTextFile(this->codedOutputFileName);
	ofstream decodedTextFile(this->decodedOutputFileName);

	char currChar;
	HuffmanNode* node = this->partTwoObj.pq.top();

	string currHuffmanCode = "";
	string currCharStr;
	char currASCII;

	char leftChar = '0';
	char rightChar = '1';

	while (codedTextFile.get(currChar)) {
		if (node->left != nullptr && node->right != nullptr) {
			currHuffmanCode += currChar;
			if (currChar == leftChar) {
				node = node->left;
			}
			else if (currChar == rightChar) {
				node = node->right;
			}
		}
		else {
			// first process current huffmancode
			currCharStr = this->huffman_charstr_map.find(currHuffmanCode)->second;
			decodedTextFile << getCharFromStr(currCharStr);


			// next, reset current huffman code including current char, and reset node
			currHuffmanCode = currChar;
			node = this->partTwoObj.pq.top();
			if (currChar == leftChar) {
				node = node->left;
			}
			else if (currChar == rightChar) {
				node = node->right;
			}
		}
	}
	// --- Write final character
	currCharStr = this->huffman_charstr_map.find(currHuffmanCode)->second;
	decodedTextFile << getCharFromStr(currCharStr);

	codedTextFile.close();
	decodedTextFile.close();
}


string PartThree::partThreeMethod(string clearTextFileName,
	string codeTableFileName,
	string codedOutputFileName,
	string decodedOutputFileName) {

	this->clearTextFileName = clearTextFileName;
	this->codeTableFileName = codeTableFileName;
	this->codedOutputFileName = codedOutputFileName;
	this->decodedOutputFileName = decodedOutputFileName;

	try {
		// --- Part A) Write coded.txt File
		readCodeTable();
		writeCodedClearTextFile();

		// --- Part B) Write decoded.txt File
		writeDecodedTextFile();

		// --- CHECK: that clear.txt and decoded.txt are equal
		compareFiles();

		return "Success";
	}
	catch (const exception& e) {
		(void)e; // avoids the warning about unreferenced local variable
		return "Error";
	}
}
