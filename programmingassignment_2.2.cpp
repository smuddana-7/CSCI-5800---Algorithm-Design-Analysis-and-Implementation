#include "utility.h"
using namespace std;

class Coordinates {
public:
    double a, b;
};
double calculateDistanceBetweenPoints(const Coordinates& p1, const Coordinates& p2)
{
    double dx = p1.a - p2.a;
    double dy = p1.b - p2.b;
    return sqrt(dx * dx + dy * dy);
}
bool compareXplane(const Coordinates& p1, const Coordinates& p2) {
    if (p1.a == p2.a) {
        return p1.b < p2.b; // Compare y-coordinates if x-coordinates are equal
    }
    return p1.a < p2.a;
}
bool compareYplane(const Coordinates& p1, const Coordinates& p2) {
    return p1.b < p2.b;
}
pair<Coordinates, Coordinates> findClosestPairOfPoints(vector<Coordinates>& points, int left, int right) {
    //base condition
    if (right - left <= 3) {
        double minDistance = numeric_limits<double>::max();
        pair<Coordinates, Coordinates> closest;
        for (int i = left; i <= right - 1; i++) {
            for (int j = i + 1; j <= right - 1; j++) {
                double dist = calculateDistanceBetweenPoints(points[i], points[j]);
                if (dist < minDistance) {
                    minDistance = dist;
                    closest = make_pair(points[i], points[j]);
                }
            }
        }
        return closest;
    }
    int mid = (left + right) / 2;
    vector<Coordinates> leftHalf(points.begin() + left, points.begin() + mid);
    vector<Coordinates> rightHalf(points.begin() + mid, points.begin() + right);
    pair<Coordinates, Coordinates> closestLeft = findClosestPairOfPoints(points, left, mid);
    pair<Coordinates, Coordinates> closestRight = findClosestPairOfPoints(points, mid, right);
    double minDist = min(calculateDistanceBetweenPoints(closestLeft.first, closestLeft.second),
        calculateDistanceBetweenPoints(closestRight.first, closestRight.second));
    vector<Coordinates> strip;
    for (int i = left; i <= right - 1; i++) {
        if (abs(points[i].a - points[mid].a) < minDist) {
            strip.push_back(points[i]);
        }
    }
    sort(strip.begin(), strip.end(), compareYplane);
    double minStripDist = minDist;
    pair<Coordinates, Coordinates> closestStrip;
    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].b - strip[i].b) < minStripDist; j++) {
            double dist = calculateDistanceBetweenPoints(strip[i], strip[j]);
            if (minStripDist > dist) {
                minStripDist = dist;
                closestStrip = make_pair(strip[i], strip[j]);
            }
        }
    }
    if (minStripDist < minDist) {
        return closestStrip;
    }
    return (calculateDistanceBetweenPoints(closestRight.first, closestRight.second) >= calculateDistanceBetweenPoints(closestLeft.first, closestLeft.second)) ? closestLeft : closestRight;
}
pair<Coordinates, Coordinates> findClosestPair(vector<Coordinates>& points) {
    sort(points.begin(), points.end(), compareXplane);
    return findClosestPairOfPoints(points, 0, points.size());
}
int main() {
    ifstream inputFile("points.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening file.\n";
        return 1;
    }
    vector<Coordinates> points;
    double a, b;
    while (inputFile >> a >> b) {
        Coordinates point = { a, b };
        points.push_back(point);
    }
    inputFile.close();
    pair<Coordinates, Coordinates> closestPoints = findClosestPair(points);
    cout << "Distance between closest points: " << calculateDistanceBetweenPoints(closestPoints.first, closestPoints.second) << endl;
    cout << "Closest points: (" << closestPoints.first.a << ", " << closestPoints.first.b << ") and (" << closestPoints.second.a << ", " << closestPoints.second.b << ")\n";
    return 0;
}
