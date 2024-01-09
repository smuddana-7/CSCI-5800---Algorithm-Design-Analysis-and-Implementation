#include "utility.h"

#ifndef  PARTONE_H
#define  PARTONE_H

class PartOne {

public:
	string inputFilename = inputFilename;
	string outputFilename = outputFilename;
	PartOne();
	string partOneMethod(string inputFilename, string outputFilename);
private:
	void processFile();
};


#endif