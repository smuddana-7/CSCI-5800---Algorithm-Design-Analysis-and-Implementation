#include "utility.h"

class Timer
{
public:
    Timer();
    double elapsed_time();
    void reset();

private:
    clock_t start_time;
};

Timer::Timer()
{
    start_time = clock();
}
double Timer::elapsed_time()
{
    clock_t end_time = clock();
    return ((double)(end_time - start_time)) / ((double)CLOCKS_PER_SEC);
}
void Timer::reset()
{
    start_time = clock();
};

const int NEG_INF = -32766;
const int POS_INF = 32767;

void merge(std::vector<short int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }

    while (i < n1)
    {
        arr[k++] = L[i++];
    }

    while (j < n2)
    {
        arr[k++] = R[j++];
    }
}

void mergeSort(std::vector<short int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void sortColumns(std::vector<std::vector<short int>> &matrix, int ci)
{
    int rows = matrix.size();
    vector<short int> column(rows);
    for (int ri = 0; ri < rows; ++ri)
    {
        column[ri] = matrix[ri][ci];
    }
    // std::sort(column.begin(), column.end());
    mergeSort(column, 0, rows - 1);
    for (int ri = 0; ri < rows; ++ri)
    {
        matrix[ri][ci] = column[ri];
    }
}

bool sortCheck(const std::vector<std::vector<short int>> &matrix, int r, int s)
{
    for (int j = 0; j < s; j++)
    {
        for (int i = 1; i < r; i++)
        {
            if (matrix[i][j] < matrix[i - 1][j])
            {
                return false;
            }
        }
    }
    return true;
}

void transpose(std::vector<std::vector<short int>> &matrix)
{
    std::vector<std::vector<short int>> transposed(matrix[0].size(), std::vector<short int>(matrix.size()));
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < matrix[0].size(); ++j)
        {
            transposed[j][i] = matrix[i][j];
        }
    }
    matrix = transposed;
}

void reverse_transpose(std::vector<std::vector<short int>> &matrix, int r, int s)
{
    transpose(matrix);
}

std::vector<std::vector<short int>> shiftDown(const std::vector<std::vector<short int>> &matrix)
{
    int r = matrix.size();
    int s = matrix[0].size();
    vector<vector<short int>> shifted(r, vector<short int>(s + 1, 0));
    int shiftValue = r / 2;

    // Fill the first half of the first column with NEG_INF
    for (int i = 0; i < shiftValue; i++)
    {
        shifted[i][0] = NEG_INF;
    }

    // Shift values
    for (int j = 0; j < s; j++)
    {
        for (int i = 0; i < r; i++)
        {
            if (i < shiftValue)
            {
                shifted[i][j + 1] = matrix[i][j];
            }
            else
            {
                shifted[i][j] = matrix[i][j];
            }
        }
    }

    // Fill the bottom half of the last column with POS_INF
    for (int i = shiftValue; i < r; i++)
    {
        shifted[i][s] = POS_INF;
    }

    return shifted;
}

std::vector<std::vector<short int>> shiftUp(const std::vector<std::vector<short int>> &matrix)
{
    int r = matrix.size();
    int s = matrix[0].size();
    vector<vector<short int>> reverseShifted(r, vector<short int>(s, 0));
    int shiftValue = r / 2;

    for (int j = 0; j < s; j++)
    {
        for (int i = 0; i < r; i++)
        {
            if (i < shiftValue)
            {
                reverseShifted[i][j] = matrix[i][j + 1];
            }
            else
            {
                reverseShifted[i][j] = matrix[i][j];
            }
        }
    }

    return reverseShifted;
}

std::vector<std::vector<short int>> column_sort(const std::vector<std::vector<short int>> &inputMatrix, int r, int s)
{
    std::vector<std::vector<short int>> matrix = inputMatrix;

    for (int j = 0; j < s; j++)
        sortColumns(matrix, j);

    transpose(matrix);

    for (int j = 0; j < r; j++)
        sortColumns(matrix, j);

    reverse_transpose(matrix, r, s);

    for (int j = 0; j < s; j++)
        sortColumns(matrix, j);

    matrix = shiftDown(matrix);

    for (int j = 0; j < s + 1; j++)
        sortColumns(matrix, j);

    matrix = shiftUp(matrix);

    return matrix;
}

int main()
{
    for (int k = 1; k <= 3; k++)
    {
        string infileName = "Part1Data" + to_string(k) + ".txt";
        string outfileName = "Results" + to_string(k) + ".txt";

        int r = 4000, s = 25;
        std::ifstream inputFile(infileName);
        std::vector<vector<short int>> matrix(r, vector<short int>(s));
        for (int j = 0; j < s; j++)
        {
            for (int i = 0; i < r; i++)
            {
                inputFile >> matrix[i][j];
            }
        }
        inputFile.close();

        Timer timer;
        std::vector<std::vector<short int>> finalMatrix = column_sort(matrix, r, s);
        double elapsed_time = timer.elapsed_time();

        cout.setf(ios::fixed);
        cout.setf(ios::showpoint);
        cout.precision(3);
        ofstream outputFile(outfileName);

        for (int j = 0; j < s; j++)
        {
            for (int i = 0; i < r; i++)
            {
                outputFile << finalMatrix[i][j] << "\n";
            }
        }
        if (sortCheck(finalMatrix, r, s))
        {
            cout << "Values in " << infileName << " are sorted. Elapsed time: " << elapsed_time << " seconds" << endl;
        }
        else
        {
            cout << "Values in " << infileName << " are not sorted. Elapsed time: " << elapsed_time << " seconds" << endl;
        }
    }
    return 0;
}