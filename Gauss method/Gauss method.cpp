#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void columnDivison(vector<vector<double>>& matrix, int column, bool max) {
    for (int i = column; i < matrix.size(); i++) {
        double firstElem = matrix[i][column];
        bool nullColumn = false;
        if (firstElem == 0 && i == column) {
            for (int j = column + 1; j < matrix.size(); j++) {
                if (matrix[j][column] != 0) {
                    swap(matrix[i], matrix[j]);
                    firstElem = matrix[i][column];
                    nullColumn = false;
                    break;
                }
                else
                    nullColumn = true;
            }
        }
        if (nullColumn) {
            cout << "Error: Null column" << endl;
            exit(1);
        }
        if (max) {
            for (int j = column + 1; j < matrix.size(); j++) {
                if (matrix[j][column] > firstElem && matrix[j][column] != 0) {
                    swap(matrix[i], matrix[j]);
                    firstElem = matrix[i][column];
                }
            }
            max = false;
        }
        if (firstElem != 0)
            for (auto& elem : matrix[i]) {
                elem /= firstElem;
            }
    }
}

void strokeAddition(vector<vector<double>>& matrix, int column) {
    int sign = 1;
    for (int i = column + 1; i < matrix.size(); i++) {
        if (matrix[i][column] == 0)
            continue;
        if (matrix[column][column] * matrix[i][column] < 0)
            sign = 1;
        else
            sign = -1;
        for (int j = column; j < matrix[i].size(); j++) {
            matrix[i][j] += sign * matrix[column][j];
        }
    }
}

void strokeWrap(string & stroke, vector <double>& matrixStroke) {
    stroke += ' ';
    string buff = "";
    for (auto &it : stroke) {
        if (it == ' ' || it == '\n') {
            if (buff.size() != 0) {
                matrixStroke.push_back(stod(buff));
                buff = "";
            }
            continue;
        }
        else
            buff += it;
    }
}

void findRoots(vector<vector<double>>& matrix, vector <double>& roots) {
    int strokeNum = matrix.size() - 1;
    while (strokeNum >= 0) {
        int i = 0;
        for (i; i < matrix[strokeNum].size(); i++) {
            if (matrix[strokeNum][i] != 0)
                break;
        }
        if (i == matrix[strokeNum].size() - 1 && matrix[strokeNum][i] != 0) {
            cout << "Error: Incompatible system of equations" << endl;
            return;
        }
        int j = strokeNum - 1;
        while (j >= 0) {
            matrix[j][i] *= *(matrix[strokeNum].end() - 1) / matrix[strokeNum][i];
            *(matrix[j].end() - 1) -= matrix[j][i];
            j--;
        }
        roots.push_back(*(matrix[strokeNum].end() - 1) / matrix[strokeNum][i]);
        strokeNum--;
    }
}

int main()
{
    fstream matrixData("Matrix.txt");
    vector<vector<double>> matrix;
    vector <double> roots;

    while (!matrixData.eof()) {
        vector<double> tmp;
        matrix.push_back(tmp);
        string buff;
        getline(matrixData, buff);
        strokeWrap(buff, matrix[matrix.size() - 1]);
    }

    vector<vector<double>> matrixMax = matrix;

    cout.precision(4);
    cout << "Original Matrix:" << endl;
    for (auto& stroke : matrix) {
        for (auto& column : stroke) {
            if (column == *(stroke.end() - 1))
                cout << "| ";
            cout << fixed << column << " ";
        }
        cout << endl << endl;
    }

    for (int i = 0; i < matrix.size(); i++) {
        columnDivison(matrix, i, false);
        strokeAddition(matrix, i);
    }

    cout << "Triangle Matrix:" << endl;
    for (auto& column : matrix) {
        for (auto& stroke : column) {
            if (stroke == *(column.end() - 1))
                cout << "| ";
            cout << fixed << stroke << " ";
        }
        cout << endl << endl;
    }

    findRoots(matrix, roots);
    cout << "Roots" << endl;
    for (auto it = roots.rbegin(); it != roots.rend(); ++it) {
        cout << fixed << *it << " ";
    }
    cout << endl << endl;
    roots.clear();

    for (int i = 0; i < matrixMax.size(); i++) {
        columnDivison(matrixMax, i, true);
        strokeAddition(matrixMax, i);
    }

    cout << "Triangle MatrixMax:" << endl;
    for (auto& column : matrixMax) {
        for (auto& stroke : column) {
            if (stroke == *(column.end() - 1))
                cout << "| ";
            cout << fixed << stroke << " ";
        }
        cout << endl << endl;
    }

    findRoots(matrixMax, roots);
    cout << "Roots" << endl;
    for (auto it = roots.rbegin(); it != roots.rend(); ++it) {
        cout << fixed << *it << " ";
    }
    cout << endl;
}