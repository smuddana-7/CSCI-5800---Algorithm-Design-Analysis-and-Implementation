#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
// Function that finds peak elements value and index
int peakElement(vector<int>& nums, int left, int right){
    if (left == right){
        return left;
    }
    int mid = left + (right - left) / 2;
    if (nums[mid] > nums[mid + 1]){
        return peakElement(nums, left, mid);
    }
    else{
        return peakElement(nums, mid + 1, right);
    }
}

int main(){

    // Opening the text file
    ifstream file("peak.txt");

    // Checking whether the filed is opened or not
    if (!file.is_open()){
        cerr << "Error: Could not open input file.";
        return 1;
    }
    vector<int> nums;
    int value;

    // Pushing the file input values to the vector
    while (file >> value) {
        nums.push_back(value);
    }
    file.close();
    if (nums.empty()){
        cerr << "Empty file";
        return 1;
    }

    // Assigning function result to variable
    int index = peakElement(nums, 0, nums.size() - 1);
    cout << "The Peak element " << nums[index] << " is at index " << index << endl;
    return 0;
}

