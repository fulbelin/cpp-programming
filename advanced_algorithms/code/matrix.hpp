#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cassert>
#include <initializer_list>
#include <memory>
#include <ostream>

template <class ElementType>
class Matrix {
public:
    class Invalid {};

    // Constructor: Create a matrix with all values initialized to default T()
    Matrix(std::size_t numberOfRows, std::size_t numberOfColumns)
        : Matrix(numberOfRows, numberOfColumns, ElementType()) {
    }

    // Constructor: Create a matrix with all values set to a specified value
    Matrix(std::size_t numberOfRows, std::size_t numberOfColumns, ElementType initialValue)
        : totalRows_(numberOfRows),
        totalColumns_(numberOfColumns),
        rawData_(std::make_unique<ElementType[]>(numberOfRows* numberOfColumns)) {

        if (numberOfRows == 0 || numberOfColumns == 0) {
            throw Invalid{};  // Invalid if any dimension is zero
        }

        // Fill the entire matrix with the initial value
        for (std::size_t linearIndex = 0, totalElements = numberOfRows * numberOfColumns;
            linearIndex < totalElements; ++linearIndex) {
            rawData_[linearIndex] = initialValue;
        }
    }

    // Constructor: Create matrix from a 2D initializer list
    Matrix(std::initializer_list<std::initializer_list<ElementType>> initElements)
        : totalRows_(initElements.size()), totalColumns_(0), rawData_() {

        if (initElements.size() == 0 || initElements.begin()->size() == 0) {
            throw Invalid{}; // Empty initializer list not allowed
        }

        totalColumns_ = initElements.begin()->size();
        rawData_ = std::make_unique<ElementType[]>(totalRows_ * totalColumns_);

        std::size_t linearIndex = 0;
        for (auto rowList : initElements) {
            // the number of columns must be the same for each row
            if (rowList.size() != totalColumns_) {
                throw Invalid{};  // All rows must have the same number of columns
            }

            for (auto value : rowList) {
                rawData_[linearIndex] = value; // Flatten and store values row by row
                ++linearIndex;
            }
        }
    }

    // Copy constructor: Create a deep copy of another matrix
    Matrix(const Matrix& originalMatrix)
        : totalRows_(originalMatrix.nrows()),
        totalColumns_(originalMatrix.ncols()),
        rawData_() {

        rawData_ = std::make_unique<ElementType[]>(totalRows_ * totalColumns_);
        std::copy(originalMatrix.rawData_.get(),
            originalMatrix.rawData_.get() + (totalRows_ * totalColumns_),
            rawData_.get());
    }

    // Copy assignment operator: Properly handle assignment, including self-assignment
    Matrix& operator=(const Matrix& sourceMatrix) {
        if (this == &sourceMatrix) {
            return *this; // Do nothing if assigning to self
        }

        // Allocate new space if current space is too small
        if (totalRows_ * totalColumns_ < sourceMatrix.nrows() * sourceMatrix.ncols()) {
            rawData_ = std::make_unique<ElementType[]>(sourceMatrix.nrows() * sourceMatrix.ncols());
        }

        totalRows_ = sourceMatrix.nrows();
        totalColumns_ = sourceMatrix.ncols();

        std::copy(sourceMatrix.rawData_.get(),
            sourceMatrix.rawData_.get() + (totalRows_ * totalColumns_),
            rawData_.get());

        return *this;
    }

    // Element access using matrix-style indexing 
    ElementType& operator()(std::size_t rowIndex, std::size_t columnIndex) {
        assert(rowIndex * totalColumns_ + columnIndex < totalRows_ * totalColumns_);
        return rawData_[rowIndex * totalColumns_ + columnIndex];
    }

    ElementType operator()(std::size_t rowIndex, std::size_t columnIndex) const {
        assert(rowIndex * totalColumns_ + columnIndex < totalRows_ * totalColumns_);
        return rawData_[rowIndex * totalColumns_ + columnIndex];
    }

    // Getter for number of rows
    std::size_t nrows() const { return totalRows_; }

    // Getter for number of columns
    std::size_t ncols() const { return totalColumns_; }

private:
    std::size_t totalRows_;
    std::size_t totalColumns_;
    std::unique_ptr<ElementType[]> rawData_;  // Raw matrix data stored in 1D array
};

// Overloaded output operator to print matrix in a clean row-wise format
template <typename ElementType>
std::ostream& operator<<(std::ostream& os, const Matrix<ElementType>& matrix) {
    for (std::size_t rowIndex = 0; rowIndex < matrix.nrows(); ++rowIndex) {
        for (std::size_t columnIndex = 0; columnIndex < matrix.ncols() - 1; ++columnIndex) {
            os << matrix(rowIndex, columnIndex) << ' ';
        }
        os << matrix(rowIndex, matrix.ncols() - 1) << '\n'; // End of row
    }
    return os;
}

#endif  // MATRIX_HPP
