#ifndef ASSIGNMENT2_DATAFRAME_HPP
#define ASSIGNMENT2_DATAFRAME_HPP

#include <algorithm>
#include <iterator>
#include <list>
#include <ostream>
#include <string>
#include <vector>
#include <type_traits>
#include <functional>

#include "col_type.hpp"
#include "utils.hpp"

/**
 * Dataframe is a simplified table structure that stores heterogeneous columns.
 * Each column is a list of ColType objects, supporting generic types via type-erasure.
 * This class offers row/column access, renaming, mutation, and transformation utilities.
 */
class Dataframe {
    std::list<std::vector<ColType>> data;         // Stores columns as lists of ColType values
    std::vector<std::string> colnames;            // Optional column names
    std::vector<std::string> rownames;            // Optional row names

    // Type-safe cast helper to extract values from ColType
    template<typename T>
    T& get_value(ColType& c) {
        return static_cast<DataType<std::remove_reference_t<T>>&>(c.get()).get_value();
    }

    template<typename T>
    const T& get_value(const ColType& c) const {
        return static_cast<const DataType<std::remove_reference_t<T>>&>(c.get()).get_value();
    }

public:
    class Invalid {};  // Thrown on invalid indexing or missing names

    Dataframe();

    // Query row and column counts
    std::size_t nrows() const;
    std::size_t ncols() const;

    // Add a column of typed data
    template<typename T>
    void add_column(const std::vector<T>& values);

    // Remove columns by index or name
    void remove_column(std::size_t);
    void remove_column(const std::string&);

    // Swap two columns by index or name
    void swap_columns(std::size_t, std::size_t);
    void swap_columns(const std::string&, const std::string&);

    // Set, clear, and query column/row names
    void set_colnames(const std::vector<std::string>&);
    void set_rownames(const std::vector<std::string>&);
    void clear_colnames();
    void clear_rownames();
    bool has_colnames() const;
    bool has_rownames() const;
    const std::vector<std::string>& get_colnames() const;
    const std::vector<std::string>& get_rownames() const;

    // Generic data access by position or name
    template<typename T>       T& get(std::size_t, std::size_t);
    template<typename T> const T& get(std::size_t, std::size_t) const;
    template<typename T>       T& get(const std::string&, const std::string&);
    template<typename T> const T& get(const std::string&, const std::string&) const;

    // Apply transformation or action to a column
    template<typename T, typename Fun>
    void apply(std::size_t col, Fun f);

    template<typename T, typename Fun>
    void apply(const std::string& colname, Fun f);

    // Overloads for specific transformation types (int/string)
    void apply(std::size_t col, std::function<int(int&)> f);
    void apply(std::size_t col, std::function<std::string(std::string&)> f);

    // Sort rows by a given column
    void order_by(const std::string&);
    void order_by(std::size_t);

    // Equality operators
    bool operator==(const Dataframe&) const;
    bool operator!=(const Dataframe&) const;

    // Stream output for printing
    friend std::ostream& operator<<(std::ostream&, const Dataframe&);
};

// --- Template Implementations ---

// Get const value by (row, col) index
template<typename T>
inline const T& Dataframe::get(std::size_t i, std::size_t j) const {
    if (j >= ncols() || i >= nrows()) throw Invalid();
    auto it = data.begin(); std::advance(it, static_cast<long>(j));
    return get_value<T>((*it)[i]);
}

// Get mutable value by (row, col) index
template<typename T>
inline T& Dataframe::get(std::size_t i, std::size_t j) {
    if (j >= ncols() || i >= nrows()) throw Invalid();
    auto it = data.begin(); std::advance(it, static_cast<long>(j));
    return get_value<T>((*it)[i]);
}

// Get const value by (rowname, colname)
template<typename T>
inline const T& Dataframe::get(const std::string& r, const std::string& c) const {
    if (!has_rownames() || !has_colnames()) throw Invalid();
    auto ci = std::find(colnames.begin(), colnames.end(), c);
    auto ri = std::find(rownames.begin(), rownames.end(), r);
    if (ci == colnames.end() || ri == rownames.end()) throw Invalid();
    return get<T>(ri - rownames.begin(), ci - colnames.begin());
}

// Get mutable value by (rowname, colname)
template<typename T>
inline T& Dataframe::get(const std::string& r, const std::string& c) {
    if (!has_rownames() || !has_colnames()) throw Invalid();
    auto ci = std::find(colnames.begin(), colnames.end(), c);
    auto ri = std::find(rownames.begin(), rownames.end(), r);
    if (ci == colnames.end() || ri == rownames.end()) throw Invalid();
    return get<T>(ri - rownames.begin(), ci - colnames.begin());
}

// Add a new column of values to the dataframe
template<typename T>
inline void Dataframe::add_column(const std::vector<T>& values) {
    if (!data.empty() && values.size() != nrows()) throw Invalid();

    std::vector<ColType> col;
    for (const T& v : values) col.emplace_back(v);
    data.push_back(std::move(col));

    if (has_colnames()) colnames.emplace_back();  // Maintain column name alignment
}

// Apply transformation or action to all cells in column (by index)
template<typename T, typename Fun>
inline void Dataframe::apply(std::size_t col, Fun f) {
    if (col >= ncols()) throw Invalid();
    auto it = data.begin(); std::advance(it, static_cast<long>(col));
    for (ColType& cell : *it) {
        T& ref = get_value<T>(cell);
        if constexpr (std::is_void_v<std::invoke_result_t<Fun, T&>>) {
            f(ref);  // Action only
        }
        else {
            ref = f(ref);  // Transformation
        }
    }
}

// Apply transformation or action to column (by name)
template<typename T, typename Fun>
inline void Dataframe::apply(const std::string& colname, Fun f) {
    if (!has_colnames()) throw Invalid();
    auto it = std::find(colnames.begin(), colnames.end(), colname);
    if (it == colnames.end()) throw Invalid();
    apply<T>(it - colnames.begin(), f);
}

#endif  // ASSIGNMENT2_DATAFRAME_HPP