#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cassert>        
#include <cstddef>        
#include <initializer_list> 
#include <memory>          
#include <ostream>         
#include <iterator>        
#include <algorithm>       

// A simple matrix class
template <class T>
class Matrix {
public:
    // Thrown when matrix construction is invalid
    class Invalid {};

    // Constructor: creates a matrix with rows and cols, filled with default T()
    Matrix(std::size_t rows, std::size_t cols) : Matrix(rows, cols, T()) {}

    // Constructor with initial value
    Matrix(std::size_t rows, std::size_t cols, T init)
        : rows_(rows), cols_(cols), data_(std::make_unique<T[]>(rows* cols)) {
        if (rows == 0 || cols == 0) throw Invalid{};
        for (std::size_t i = 0; i < rows * cols; ++i)
            data_[i] = init; // Fill with initial value
    }

    // Constructor from initializer list (like {{1,2},{3,4}})
    Matrix(std::initializer_list<std::initializer_list<T>> rows)
        : rows_(rows.size()), cols_(0) {
        if (rows.size() == 0 || rows.begin()->size() == 0) throw Invalid{};
        cols_ = rows.begin()->size();
        data_ = std::make_unique<T[]>(rows_ * cols_);
        std::size_t idx = 0;
        for (auto r : rows) {
            if (r.size() != cols_) throw Invalid{};
            for (auto val : r) data_[idx++] = val;
        }
    }

    // Copy constructor
    Matrix(const Matrix& other)
        : rows_(other.rows_), cols_(other.cols_) {
        data_ = std::make_unique<T[]>(rows_ * cols_);
        std::copy(other.data_.get(), other.data_.get() + rows_ * cols_, data_.get());
    }

    // Copy assignment
    Matrix& operator=(const Matrix& other) {
        if (this == &other) return *this;
        if (rows_ * cols_ < other.rows_ * other.cols_)
            data_ = std::make_unique<T[]>(other.rows_ * other.cols_);
        rows_ = other.rows_;
        cols_ = other.cols_;
        std::copy(other.data_.get(), other.data_.get() + rows_ * cols_, data_.get());
        return *this;
    }

    // Access an element (non-const version))
    T& operator()(std::size_t r, std::size_t c) {
        assert(r < rows_ && c < cols_);
        return data_[r * cols_ + c];
    }

    // Access an element (const version)
    const T& operator()(std::size_t r, std::size_t c) const {
        assert(r < rows_ && c < cols_);
        return data_[r * cols_ + c];
    }

    // Get number of rows
    std::size_t nrows() const { return rows_; }

    // Get number of columns
    std::size_t ncols() const { return cols_; }

   
    // Column reverse iterator: goes upward in a column
    class ColReverseIt {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        ColReverseIt(Matrix* owner, std::size_t col, std::size_t row)
            : M(owner), col_(col), row_(row) {
        }

        reference operator*() { return (*M)(row_, col_); }
        pointer operator->() { return &(**this); }

        // Move one row up
        ColReverseIt& operator++() {
            if (row_ == 0) row_ = static_cast<std::size_t>(-1);
            else --row_;
            return *this;
        }

        ColReverseIt operator++(int) {
            ColReverseIt tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const ColReverseIt& a, const ColReverseIt& b) {
            return a.M == b.M && a.col_ == b.col_ && a.row_ == b.row_;
        }

        friend bool operator!=(const ColReverseIt& a, const ColReverseIt& b) {
            return !(a == b);
        }

    private:
        Matrix* M;
        std::size_t col_;
        std::size_t row_;
    };

    // Const version of reverse column iterator
    class ConstColReverseIt {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstColReverseIt(const Matrix* owner, std::size_t col, std::size_t row)
            : M(owner), col_(col), row_(row) {
        }

        reference operator*() const { return (*M)(row_, col_); }
        pointer operator->() const { return &(**this); }

        ConstColReverseIt& operator++() {
            if (row_ == 0) row_ = static_cast<std::size_t>(-1);
            else --row_;
            return *this;
        }

        ConstColReverseIt operator++(int) {
            ConstColReverseIt tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const ConstColReverseIt& a, const ConstColReverseIt& b) {
            return a.M == b.M && a.col_ == b.col_ && a.row_ == b.row_;
        }

        friend bool operator!=(const ConstColReverseIt& a, const ConstColReverseIt& b) {
            return !(a == b);
        }

    private:
        const Matrix* M;
        std::size_t col_;
        std::size_t row_;
    };

    // Diagonal iterator (non-const)
    class DiagIt {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        DiagIt(Matrix* owner, std::size_t pos) : M(owner), pos_(pos) {}

        reference operator*() { return (*M)(pos_, pos_); }
        pointer operator->() { return &(**this); }

        DiagIt& operator++() { ++pos_; return *this; }
        DiagIt operator++(int) { DiagIt tmp = *this; ++pos_; return tmp; }

        friend bool operator==(const DiagIt& a, const DiagIt& b) {
            return a.M == b.M && a.pos_ == b.pos_;
        }

        friend bool operator!=(const DiagIt& a, const DiagIt& b) {
            return !(a == b);
        }

    private:
        Matrix* M;
        std::size_t pos_;
    };

    // Const version of diagonal iterator
    class ConstDiagIt {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstDiagIt(const Matrix* owner, std::size_t pos) : M(owner), pos_(pos) {}

        reference operator*() const { return (*M)(pos_, pos_); }
        pointer operator->() const { return &(**this); }

        ConstDiagIt& operator++() { ++pos_; return *this; }
        ConstDiagIt operator++(int) { ConstDiagIt tmp = *this; ++pos_; return tmp; }

        friend bool operator==(const ConstDiagIt& a, const ConstDiagIt& b) {
            return a.M == b.M && a.pos_ == b.pos_;
        }

        friend bool operator!=(const ConstDiagIt& a, const ConstDiagIt& b) {
            return !(a == b);
        }

    private:
        const Matrix* M;
        std::size_t pos_;
    };


    using col_reverse_iterator = ColReverseIt;
    using const_col_reverse_iterator = ConstColReverseIt;
    using diag_iterator = DiagIt;
    using const_diag_iterator = ConstDiagIt;

    // Start and end of column in reverse (from bottom to before top)
    col_reverse_iterator col_rbegin(std::size_t col) {
        assert(col < cols_);
        return col_reverse_iterator(this, col, rows_ - 1);
    }

    col_reverse_iterator col_rend(std::size_t col) {
        assert(col < cols_);
        return col_reverse_iterator(this, col, static_cast<std::size_t>(-1));
    }

    const_col_reverse_iterator col_rbegin(std::size_t col) const {
        assert(col < cols_);
        return const_col_reverse_iterator(this, col, rows_ - 1);
    }

    const_col_reverse_iterator col_rend(std::size_t col) const {
        assert(col < cols_);
        return const_col_reverse_iterator(this, col, static_cast<std::size_t>(-1));
    }

    // Diagonal iterator (for square matrix)
    diag_iterator diag_begin() {
        assert(rows_ == cols_);
        return diag_iterator(this, 0);
    }

    diag_iterator diag_end() {
        assert(rows_ == cols_);
        return diag_iterator(this, rows_);
    }

    const_diag_iterator diag_begin() const {
        assert(rows_ == cols_);
        return const_diag_iterator(this, 0);
    }

    const_diag_iterator diag_end() const {
        assert(rows_ == cols_);
        return const_diag_iterator(this, rows_);
    }

private:
    std::size_t rows_ = 0;
    std::size_t cols_ = 0;
    std::unique_ptr<T[]> data_; // Smart pointer for memory safety
};


// Print the matrix nicely to output
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& M) {
    for (std::size_t r = 0; r < M.nrows(); ++r) {
        for (std::size_t c = 0; c < M.ncols(); ++c) {
            os << M(r, c);
            if (c + 1 < M.ncols()) os << ' ';
        }
        if (r + 1 < M.nrows()) os << '\n';
    }
    return os;
}

#endif // MATRIX_HPP