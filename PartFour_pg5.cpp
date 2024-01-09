#include "utility.h"
#include "PartFour.h"

bool equalFiles(ifstream& in1, ifstream& in2) {
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
void compareFiles(string clearTextFileName, string decodedOutputFileName) {
    ifstream in1(clearTextFileName);
    ifstream in2(decodedOutputFileName);
    if (equalFiles(in1, in2)) {
        cout << "      Part 4 --> SUCCESS! Clear text and decoded files are equal!" << endl;
    }
    else {
        cout << "      Part 4 --> WARNING! Clear text and decoded files are NOT EQUAL! Fix!" << endl;
    }
    in1.close();
    in2.close();
}
// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------

double encodeBits(const string clearFileName, const string codeTableFileName, const string codedAltFileName) {
    ifstream clearFile(clearFileName);
    ifstream codeTableFile(codeTableFileName);
    ofstream codedAltFile(codedAltFileName, ios::binary);

    if (!clearFile) {
        cerr << "Error: Unable to open input file - " << clearFileName << endl;
        exit(EXIT_FAILURE);
    }

    if (!codeTableFile) {
        cerr << "Error: Unable to open input file - " << codeTableFileName << endl;
        exit(EXIT_FAILURE);
    }

    if (!codedAltFile) {
        cerr << "Error: Unable to open output file - " << codedAltFileName << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<string, string> huffmanCodes;

    string ch;
    string code;
    while (codeTableFile >> ch >> code) {

        huffmanCodes[ch] = code;
    }
    codeTableFile.close();


    bitset<8> byte;
    int bitIndex = 0;
    double processedBits = 0;
    char inputChar;
    string inputStr;
    while (clearFile.get(inputChar)) {

        if (inputChar == ' ') {
            inputStr = "SP";
        }
        else if (inputChar == '\n') {
            inputStr = "LF";
        }
        else
            inputStr = inputChar;

        if (huffmanCodes.find(inputStr) != huffmanCodes.end()) {
            string huffmanCode = huffmanCodes[inputStr];

            // Process each bit in the Huffman code
            for (char bit : huffmanCode) {
                byte[bitIndex++] = (bit == '1');
                processedBits++;
                // If the byte is full, write it to the file
                if (bitIndex == 8) {

                    codedAltFile.put(static_cast<char>(byte.to_ulong()));
                    // Reset bitIndex and byte for the next byte
                    bitIndex = 0;
                    byte.reset();
                }
            }
        }
    }

    // Write the last byte to the file (if it is not empty)
    if (bitIndex > 0) {
        codedAltFile.put(static_cast<char>(byte.to_ulong()));

        byte.reset();
        bitIndex = 0;
    }

    clearFile.close();

    codedAltFile.close();

    return processedBits;
}

void decodeBits(const string codedAltFileName, const string codeTableFileName, const string decodedAltFileName, double processedbits) {
    ifstream codedAltFile(codedAltFileName, ios::binary);
    ifstream codeTableFile(codeTableFileName);
    ofstream decodedAltFile(decodedAltFileName);

    if (!codedAltFile) {
        cerr << "Error: Unable to open input file - " << codedAltFileName << endl;
        exit(EXIT_FAILURE);
    }

    if (!codeTableFile) {
        cerr << "Error: Unable to open input file - " << codeTableFileName << endl;
        exit(EXIT_FAILURE);
    }

    if (!decodedAltFile) {
        cerr << "Error: Unable to open output file - " << decodedAltFileName << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<string, string> huffmanCodes;
    string str;
    string code;
    while (codeTableFile >> str >> code) {
        huffmanCodes[code] = str;
    }
    codeTableFile.close();


    string currentCode = "";
    double verifyProcessedBits = 0;
    char inputChar;
    while (codedAltFile.get(inputChar)) {
        bitset<8> byte(inputChar);

        // Process each bit in the byte
        for (int i = 0; i <= 7; i++) {
            if (verifyProcessedBits > processedbits) {
                break;
            }

            verifyProcessedBits++;
            currentCode += byte[i] ? '1' : '0';
            if (huffmanCodes.find(currentCode) != huffmanCodes.end()) {
                if (huffmanCodes[currentCode] == "SP") {
                    // cout << ' ';
                    decodedAltFile.put(' ');
                }
                else if (huffmanCodes[currentCode] == "LF") {
                    //cout << '\n';
                    decodedAltFile.put('\n');
                }
                else {
                    //cout << huffmanCodes[currentCode][0];
                    decodedAltFile.put(huffmanCodes[currentCode][0]);
                }
                currentCode = "";
            }
        }


    }


    codedAltFile.close();
    codeTableFile.close();
    decodedAltFile.close();
}
string PartFour::partFourMethod(string clearFileName, string codeTableFileName, string codedAltFileName, string decodedAltFileName) {
    //const string clearFileName = "clear.txt";
    //const string codeTableFileName = "codetable.txt";
    //const string codedAltFileName = "codedalt.txt";
    try {
        double processedbits = encodeBits(clearFileName, codeTableFileName, codedAltFileName);

        //   cout << "Bits encoded and saved to " << codedAltFileName << endl;

        decodeBits(codedAltFileName, codeTableFileName, decodedAltFileName, processedbits);

        compareFiles(clearFileName, decodedAltFileName);
        //cout << "Bits decoded and saved to " << decodedAltFileName << endl;
        return "Success";
    }
    catch (const std::exception& e) {
        return "Error";
    }
}
