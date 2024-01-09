#include "utility.h"

#ifndef  PARTTWO_H
#define  PARTTWO_H

// Class for Huffman tree nodes
class HuffmanNode {
public:
	string charStr;
	unsigned freq;
	HuffmanNode* left, * right;

	HuffmanNode(string charStr, unsigned freq) : charStr(charStr), freq(freq), left(nullptr), right(nullptr) {}
};

// Comparator for the priority queue to ensure the node with the lowest frequency is at the top
class Compare {
public:
	bool operator()(HuffmanNode* l, HuffmanNode* r) {
		return (l->freq > r->freq);
	}
};



class PartTwo {

public:
	string inputFilename = inputFilename;
	string outputFilename = outputFilename;
	priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

	string partTwoMethod(string inputFilename, string outputFilename);

private:
	void generateCodes(HuffmanNode* root, string str, map<string, string>& huffmanCode);
	map<string, int> readFrequencies(string filename);
	HuffmanNode* buildHuffmanTree(map<string, int> frequencies);
	void writeCodeTable(map<string, string> huffmanCode, string filename);


};


#endif