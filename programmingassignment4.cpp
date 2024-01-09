#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <cmath>

using namespace std;

// Function to compute line parameters
pair<double, double> computeLineParams(const vector<pair<double, double>>& dataSet, int startIdx, int endIdx) {
    double xSum = 0, ySum = 0, xySum = 0, x2Sum = 0;
    int dataCount = endIdx - startIdx + 1;
    for (int idx = startIdx; idx <= endIdx; ++idx) {
        xSum += dataSet[idx].first;
        ySum += dataSet[idx].second;
        xySum += dataSet[idx].first * dataSet[idx].second;
        x2Sum += dataSet[idx].first * dataSet[idx].first;
    }
    double slope = (dataCount * xySum - xSum * ySum) / (dataCount * x2Sum - xSum * xSum);
    double intercept = (ySum - slope * xSum) / dataCount;
    return {slope, intercept};
}

// Function to calculate fit error
double calcFitError(const vector<pair<double, double>>& dataSet, int startIdx, int endIdx) {
    if (startIdx >= endIdx) return 0;

    auto lineVals = computeLineParams(dataSet, startIdx, endIdx);
    double err = 0;
    for (int idx = startIdx; idx <= endIdx; ++idx) {
        double estY = lineVals.first * dataSet[idx].first + lineVals.second;
        err += (dataSet[idx].second - estY) * (dataSet[idx].second - estY);
    }
    return err;
}

int main() {
    ifstream inputFile("points.txt");
    if (!inputFile) {
        cerr << "File open error." << endl;
        return 1;
    }

    vector<pair<double, double>> points;
    double x, y;
    while (inputFile >> x >> y) {
        points.emplace_back(x, y);
    }
    inputFile.close();

    const int pointCount = points.size();
    const double costConst = 1;

    vector<vector<double>> errMatrix(pointCount, vector<double>(pointCount, 0));
    for (int i = 0; i < pointCount; ++i) {
        for (int j = i; j < pointCount; ++j) {
            errMatrix[i][j] = calcFitError(points, i, j);
        }
    }

    vector<std::pair<double, double>> lineParams(pointCount + 1);
    vector<double> minCosts(pointCount + 1, numeric_limits<double>::max());
    vector<int> parent(pointCount + 1, 0);
    minCosts[0] = minCosts[1] = 0;
    parent[0] = parent[1] = 0;

    cout << "\nSegment End Cost Breakpoint" << endl;

    for (int j = 0; j <= pointCount; ++j) {
        for (int i = 1; i <= j; ++i) {
            double cost = errMatrix[i - 1][j - 1] + costConst + minCosts[i];
            if (cost < minCosts[j]) {
                minCosts[j] = cost;
                parent[j] = i;
            }
        }
        cout << " " << j << " " << minCosts[j] << " " << parent[j] << endl;
    }

    vector<int> breaks;
    for (int j = pointCount; j > 0; j = parent[j]) {
        breaks.push_back(j);
    }

    for (size_t i = 1; i < breaks.size(); ++i) {
        int start = breaks[i] - 1;
        int end = breaks[i - 1] - 1;
        auto coeffs = computeLineParams(points, start, end);
        lineParams[end] = coeffs;
        cout << "Line " << start + 1 << " to " << end + 1 << " is y = " << coeffs.first << "x + " << coeffs.second << endl;
    }

    return 0;
}
