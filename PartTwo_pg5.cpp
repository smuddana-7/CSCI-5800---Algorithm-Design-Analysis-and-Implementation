#include "utility.h"
#include "PartTwo.h"

// Recursive function to generate Huffman codes for each character and store them in a map
void PartTwo::generateCodes(HuffmanNode* root, string str, map<string, string>& huffmanCode) {
    if (!root) return;

    if (root->charStr != "")
        huffmanCode[root->charStr] = str;

    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

// Function to read character frequencies from a file and return a map of characters in string format to their frequencies
map<string, int> PartTwo::readFrequencies(string filename) {
    string line;
    ifstream freqFile(filename);
    map<string, int> frequencies;

    while (getline(freqFile, line)) {

        string character;
        if (line[0] == 'L' && line[1] == 'F') {
            character = "LF";
        }
        else if (line[0] == 'S' && line[1] == 'P') {
            character = "SP";
        }
        else {
            character = string() + line[0];
        }
        int freq = stoi(line.substr(line.find(' ') + 1));
        frequencies[character] = freq;
    }

    freqFile.close();
    return frequencies;
}

// Function to build the Huffman tree based on character frequencies
HuffmanNode* PartTwo::buildHuffmanTree(map<string, int> frequencies) {

    for (auto pair : frequencies)
        this->pq.push(new HuffmanNode(pair.first, pair.second));

    while (this->pq.size() != 1) {
        HuffmanNode* left = this->pq.top(); this->pq.pop();
        HuffmanNode* right = this->pq.top(); this->pq.pop();

        HuffmanNode* top = new HuffmanNode("", left->freq + right->freq);
        top->left = left;
        top->right = right;
        this->pq.push(top);
    }

    return this->pq.top();
}

// Function to write the generated Huffman codes to a file
void PartTwo::writeCodeTable(map<string, string> huffmanCode, string filename) {
    ofstream codeTableFile(filename);

    // Check if the input file was successfully opened
    if (!codeTableFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (auto pair : huffmanCode) {
        if (pair.first == "LF") {
            codeTableFile << "LF" << " " << pair.second << endl;
        }
        else if (pair.first == "SP") {
            codeTableFile << "SP" << " " << pair.second << endl;
        }
        else {
            codeTableFile << pair.first << " " << pair.second << endl;
        }
       
    }

    codeTableFile.close();
}

string PartTwo::partTwoMethod(string freqFilename, string codetableFilename){    // Read frequencies from the file

    try {
        map<string, int> frequencies = readFrequencies(freqFilename);

        // Build the Huffman tree
        HuffmanNode* root = buildHuffmanTree(frequencies);

        // Generate Huffman codes
        map<string, string> huffmanCode;
        generateCodes(root, "", huffmanCode);

        // Write the codes to a file
        writeCodeTable(huffmanCode, codetableFilename);

        return "Success";
    }
    catch (const exception& e) {
        (void)e; // avoids the warning about unreferenced local variable
        return "Error";
    }
   
}


//---------------------------------------Team 5 Gunturu, Khan, Manchikatla, Muddana, Richert---------------------------------------//