#include "munkres_algorithm.hpp"
#include <vector>
#include <limits>
#include <iostream>

Matrix<int> run_munkres_algorithm(Matrix<int> costMatrix) {

    // Check if the input matrix is square (n x n)
    if (costMatrix.nrows() != costMatrix.ncols()) {
        throw Matrix<int>::Invalid{};
    }

    int matrixSize = costMatrix.nrows();

    // markMatrix will track marked zeros:
    // 0 = unmarked, 1 = starred zero, 2 = primed zero
    Matrix<int> zeroMarkMatrix(matrixSize, matrixSize, 0);

    // These arrays track which rows and columns are covered
    std::vector<bool> isRowCurrentlyCovered(matrixSize, false);
    std::vector<bool> isColumnCurrentlyCovered(matrixSize, false);

    // subtract the minimum value from each row
    for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
        int minimumRowValue = costMatrix(currentRow, 0);

        for (int currentColumn = 1; currentColumn < matrixSize; currentColumn++) {
            if (costMatrix(currentRow, currentColumn) < minimumRowValue) {
                minimumRowValue = costMatrix(currentRow, currentColumn);
            }
        }

        for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
            costMatrix(currentRow, currentColumn) -= minimumRowValue;
        }
    }

    // Subtract the minimum value from each column
    for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
        int minimumColumnValue = costMatrix(0, currentColumn);

        for (int currentRow = 1; currentRow < matrixSize; currentRow++) {
            if (costMatrix(currentRow, currentColumn) < minimumColumnValue) {
                minimumColumnValue = costMatrix(currentRow, currentColumn);
            }
        }

        for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
            costMatrix(currentRow, currentColumn) -= minimumColumnValue;
        }
    }

    // Only star a zero if it's the first in its row and column
    for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
        for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
            if (costMatrix(currentRow, currentColumn) == 0 &&
                !isRowCurrentlyCovered[currentRow] &&
                !isColumnCurrentlyCovered[currentColumn]) {

                zeroMarkMatrix(currentRow, currentColumn) = 1; // Star this zero
                isRowCurrentlyCovered[currentRow] = true;
                isColumnCurrentlyCovered[currentColumn] = true;
            }
        }
    }

    // Uncover all rows and columns
    for (int i = 0; i < matrixSize; i++) {
        isRowCurrentlyCovered[i] = false;
        isColumnCurrentlyCovered[i] = false;
    }

    // Cover columns with starred zeros
    for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
        for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
            if (zeroMarkMatrix(currentRow, currentColumn) == 1) {
                isColumnCurrentlyCovered[currentColumn] = true;
            }
        }
    }

    // Count the number of covered columns
    int numberOfCoveredColumns = 0;
    for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
        if (isColumnCurrentlyCovered[currentColumn]) {
            numberOfCoveredColumns++;
        }
    }

    // Repeat until all columns are covered
    while (numberOfCoveredColumns < matrixSize) {
        int uncoveredZeroRowIndex = -1;
        int uncoveredZeroColumnIndex = -1;
        bool uncoveredZeroFound = false;

        // Find an uncovered zero and prime it
        for (int currentRow = 0; currentRow < matrixSize && !uncoveredZeroFound; currentRow++) {
            if (!isRowCurrentlyCovered[currentRow]) {
                for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
                    if (costMatrix(currentRow, currentColumn) == 0 &&
                        !isColumnCurrentlyCovered[currentColumn]) {

                        zeroMarkMatrix(currentRow, currentColumn) = 2; // Prime this zero

                        // Check for a starred zero in the same row
                        int starredZeroColumnIndex = -1;
                        for (int j = 0; j < matrixSize; j++) {
                            if (zeroMarkMatrix(currentRow, j) == 1) {
                                starredZeroColumnIndex = j;
                                break;
                            }
                        }

                        // If found, cover the row and uncover the star's column
                        if (starredZeroColumnIndex != -1) {
                            isRowCurrentlyCovered[currentRow] = true;
                            isColumnCurrentlyCovered[starredZeroColumnIndex] = false;
                        }
                        else {
                            uncoveredZeroRowIndex = currentRow;
                            uncoveredZeroColumnIndex = currentColumn;
                            uncoveredZeroFound = true;
                        }
                        break;
                    }
                }
            }
        }

        if (!uncoveredZeroFound) {
            // Adjust the matrix if no uncovered zero was found
            int smallestUncoveredValue = std::numeric_limits<int>::max();

            for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
                if (!isRowCurrentlyCovered[currentRow]) {
                    for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
                        if (!isColumnCurrentlyCovered[currentColumn] &&
                            costMatrix(currentRow, currentColumn) < smallestUncoveredValue) {
                            smallestUncoveredValue = costMatrix(currentRow, currentColumn);
                        }
                    }
                }
            }

            // Add and subtract minVal as needed
            for (int currentRow = 0; currentRow < matrixSize; currentRow++) {
                for (int currentColumn = 0; currentColumn < matrixSize; currentColumn++) {
                    if (isRowCurrentlyCovered[currentRow]) {
                        costMatrix(currentRow, currentColumn) += smallestUncoveredValue;
                    }
                    if (!isColumnCurrentlyCovered[currentColumn]) {
                        costMatrix(currentRow, currentColumn) -= smallestUncoveredValue;
                    }
                }
            }
        }
        else {
            // Build alternating path
            std::vector<int> pathRowIndices;
            std::vector<int> pathColumnIndices;

            pathRowIndices.push_back(uncoveredZeroRowIndex);
            pathColumnIndices.push_back(uncoveredZeroColumnIndex);
            bool pathCompleted = false;

            while (!pathCompleted) {
                int lastColumnInPath = pathColumnIndices.back();
                int rowWithStarredZero = -1;

                for (int row = 0; row < matrixSize; row++) {
                    if (zeroMarkMatrix(row, lastColumnInPath) == 1) {
                        rowWithStarredZero = row;
                        break;
                    }
                }

                if (rowWithStarredZero == -1) {
                    pathCompleted = true;
                }
                else {
                    pathRowIndices.push_back(rowWithStarredZero);
                    pathColumnIndices.push_back(lastColumnInPath);

                    int columnWithPrimedZero = -1;
                    for (int col = 0; col < matrixSize; col++) {
                        if (zeroMarkMatrix(rowWithStarredZero, col) == 2) {
                            columnWithPrimedZero = col;
                            break;
                        }
                    }

                    pathRowIndices.push_back(rowWithStarredZero);
                    pathColumnIndices.push_back(columnWithPrimedZero);
                }
            }

            // Flip stars and primes
            for (size_t i = 0; i < pathRowIndices.size(); i++) {
                int r = pathRowIndices[i];
                int c = pathColumnIndices[i];

                if (zeroMarkMatrix(r, c) == 1) {
                    zeroMarkMatrix(r, c) = 0;
                }
                else if (zeroMarkMatrix(r, c) == 2) {
                    zeroMarkMatrix(r, c) = 1;
                }
            }

            // Clear primes and covers
            for (int row = 0; row < matrixSize; row++) {
                isRowCurrentlyCovered[row] = false;
                for (int col = 0; col < matrixSize; col++) {
                    if (zeroMarkMatrix(row, col) == 2) {
                        zeroMarkMatrix(row, col) = 0;
                    }
                }
            }

            for (int col = 0; col < matrixSize; col++) {
                isColumnCurrentlyCovered[col] = false;
            }

            // Update column covers based on new starred zeros
            numberOfCoveredColumns = 0;
            for (int row = 0; row < matrixSize; row++) {
                for (int col = 0; col < matrixSize; col++) {
                    if (zeroMarkMatrix(row, col) == 1) {
                        isColumnCurrentlyCovered[col] = true;
                    }
                }
            }

            for (int col = 0; col < matrixSize; col++) {
                if (isColumnCurrentlyCovered[col]) {
                    numberOfCoveredColumns++;
                }
            }
        }
    }

    // Return the final assignment matrix
    Matrix<int> resultAssignmentMatrix(matrixSize, matrixSize, 0);

    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            if (zeroMarkMatrix(row, col) == 1) {
                resultAssignmentMatrix(row, col) = 1; // Only keep starred assignments
            }
        }
    }

    return resultAssignmentMatrix;
}
