#include "utility.h"
#include "PartOne.h"
#include "PartTwo.h"
#include "PartThree.h"
#include "PartFour.h"

int main() {

    cout << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Note: throughout our code, \n";
    cout << "line feed (ASCII 10) is represented by 'LF, and \n";
    cout << "space (ASCII 32) is represented by 'SP' \n";
    cout << "for optimal interpretability." << endl;
    cout << "------------------------------------------------" << endl;
    cout << endl;

    string response;

    PartOne partOneObj;
    response = partOneObj.partOneMethod("clear.txt", "freq.txt");
    cout << "Part One Execution Status: " << response << endl;

    PartTwo partTwoObj;
    response = partTwoObj.partTwoMethod("freq.txt", "codetable.txt");
    cout << "Part Two Execution Status: " << response << endl;

    PartThree partThreeObj(partTwoObj); // need to reference the priority queue from P2 to decode
    response = partThreeObj.partThreeMethod("clear.txt", "codetable.txt", "coded.txt", "decoded.txt");
    cout << "Part Three Execution Status: " << response << endl;

    PartFour partFourObj;
    response = partFourObj.partFourMethod("clear.txt", "codetable.txt", "codedalt.txt", "decodedalt.txt");
    cout << "Part Four Execution Status: " << response << endl;

    return 0;
}
