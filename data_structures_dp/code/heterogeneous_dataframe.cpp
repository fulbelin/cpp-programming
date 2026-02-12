#include "dataframe.hpp"
#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <iostream>

// Default constructor: creates an empty dataframe
Dataframe::Dataframe() = default;



// Returns the number of columns in the dataframe
std::size_t Dataframe::ncols() const {
    return data.size();
}

// Returns the number of rows (based on first column)
std::size_t Dataframe::nrows() const {
    return data.empty() ? 0 : data.front().size();
}

// Sets column names; throws if name count mismatches column count or contains duplicates
void Dataframe::set_colnames(const std::vector<std::string>& v) {
    if (v.size() != ncols() || utils::contains_duplicate(v)) throw Invalid();
    colnames = v;
}

// Sets row names; throws if name count mismatches row count or contains duplicates
void Dataframe::set_rownames(const std::vector<std::string>& v) {
    if (v.size() != nrows() || utils::contains_duplicate(v)) throw Invalid();
    rownames = v;
}

// Removes column names
void Dataframe::clear_colnames() {
    colnames.clear();
}

// Removes row names
void Dataframe::clear_rownames() {
    rownames.clear();
}

// Returns whether column names are set
bool Dataframe::has_colnames() const {
    return !colnames.empty();
}

// Returns whether row names are set
bool Dataframe::has_rownames() const {
    return !rownames.empty();
}

// Provides access to column names
const std::vector<std::string>& Dataframe::get_colnames() const {
    return colnames;
}

// Provides access to row names
const std::vector<std::string>& Dataframe::get_rownames() const {
    return rownames;
}

// Removes the column at a given index and its associated name if present
void Dataframe::remove_column(std::size_t idx) {
    if (idx >= ncols()) throw Invalid();
    auto it = data.begin();
    std::advance(it, static_cast<long>(idx));
    data.erase(it);
    if (has_colnames()) colnames.erase(colnames.begin() + static_cast<long>(idx));
}

// Removes a column by name
void Dataframe::remove_column(const std::string& name) {
    if (!has_colnames()) throw Invalid();
    auto it = std::find(colnames.begin(), colnames.end(), name);
    if (it == colnames.end()) throw Invalid();
    remove_column(static_cast<std::size_t>(it - colnames.begin()));
}

// Swaps two columns by index, along with their names
void Dataframe::swap_columns(std::size_t idx1, std::size_t idx2) {
    if (idx1 >= ncols() || idx2 >= ncols()) throw Invalid();
    if (idx1 == idx2) return;
    auto it1 = data.begin(); std::advance(it1, static_cast<long>(idx1));
    auto it2 = data.begin(); std::advance(it2, static_cast<long>(idx2));
    std::swap(*it1, *it2);
    if (has_colnames()) std::swap(colnames[idx1], colnames[idx2]);
}

// Swaps two columns by name
void Dataframe::swap_columns(const std::string& n1, const std::string& n2) {
    if (!has_colnames()) throw Invalid();
    std::size_t idx1 = ncols(), idx2 = ncols();
    for (std::size_t j = 0; j < colnames.size(); ++j) {
        if (colnames[j] == n1) idx1 = j;
        if (colnames[j] == n2) idx2 = j;
    }
    if (idx1 == ncols() || idx2 == ncols()) throw Invalid();
    swap_columns(idx1, idx2);
}

// Orders the dataframe by the values in the named column
void Dataframe::order_by(const std::string& cname) {
    if (!has_colnames()) throw Invalid();
    std::size_t key_idx = ncols();
    for (std::size_t j = 0; j < colnames.size(); ++j)
        if (colnames[j] == cname) { key_idx = j; break; }
    if (key_idx == ncols()) throw Invalid();
    order_by(key_idx);
}

// Orders the dataframe rows by the values in the given column index
void Dataframe::order_by(std::size_t key_idx) {
    if (key_idx >= ncols()) throw Invalid();

    // Generate permutation vector
    std::vector<std::size_t> perm(nrows());
    for (std::size_t i = 0; i < perm.size(); ++i) perm[i] = i;

    const std::vector<ColType>& key_col = *std::next(data.begin(), static_cast<long>(key_idx));

    // Ensure all types in key column are the same
    const std::type_info& ref_type = typeid(key_col[0].get());
    for (const ColType& cell : key_col) {
        if (typeid(cell.get()) != ref_type) throw Invalid();
    }

    // Sort permutation based on key column values
    std::stable_sort(perm.begin(), perm.end(), [&key_col](std::size_t a, std::size_t b) {
        return key_col[a] < key_col[b];
        });

    // Apply permutation to each column
    for (std::vector<ColType>& col : data) {
        std::vector<ColType> new_col;
        new_col.reserve(col.size());
        for (std::size_t r : perm) new_col.push_back(col[r]);
        col = std::move(new_col);
    }

    // Apply permutation to rownames, if present
    if (has_rownames()) {
        std::vector<std::string> new_names;
        new_names.reserve(rownames.size());
        for (std::size_t r : perm) new_names.push_back(rownames[r]);
        rownames = std::move(new_names);
    }
}

// Outputs the dataframe in tab-separated format
std::ostream& operator<<(std::ostream& os, const Dataframe& df) {
    const char sep = '\t';

    // Output column headers
    if (df.has_colnames()) {
        if (df.has_rownames()) os << sep;
        for (std::size_t j = 0; j < df.ncols(); ++j) {
            os << df.get_colnames()[j];
            if (j + 1 < df.ncols()) os << sep;
        }
        os << '\n';
    }

    // Output each row
    for (std::size_t i = 0; i < df.nrows(); ++i) {
        if (df.has_rownames()) os << df.get_rownames()[i] << sep;
        std::size_t j = 0;
        for (const std::vector<ColType>& col : df.data) {
            os << col[i].get();
            if (j + 1 < df.ncols()) os << sep;
            ++j;
        }
        os << '\n';
    }
    return os;
}

// Compares all structural and content aspects of the dataframe
bool Dataframe::operator==(const Dataframe& other) const {
    if (nrows() != other.nrows() || ncols() != other.ncols()) return false;
    if (colnames != other.colnames) return false;
    if (rownames != other.rownames) return false;

    auto it1 = data.begin(), it2 = other.data.begin();
    for (; it1 != data.end(); ++it1, ++it2) {
        const std::vector<ColType>& c1 = *it1;
        const std::vector<ColType>& c2 = *it2;
        for (std::size_t r = 0; r < c1.size(); ++r)
            if (c1[r] != c2[r]) return false;
    }
    return true;
}

// Logical negation of equality operator
bool Dataframe::operator!=(const Dataframe& other) const {
    return !(*this == other);
}

// Applies transformation to integer column using std::function
void Dataframe::apply(std::size_t col, std::function<int(int&)> f) {
    std::cerr << "[DEBUG] int apply called on col " << col << "\n";
    if (col >= ncols()) throw Invalid();
    auto it = data.begin(); std::advance(it, static_cast<long>(col));
    for (ColType& cell : *it) {
        int& ref = static_cast<DataType<int>&>(cell.get()).get_value();
        ref = f(ref);
    }
}

// Applies transformation to string column using std::function
void Dataframe::apply(std::size_t col, std::function<std::string(std::string&)> f) {
    std::cerr << "[DEBUG] string apply called on col " << col << "\n";
    if (col >= ncols()) throw Invalid();
    auto it = data.begin(); std::advance(it, static_cast<long>(col));
    for (ColType& cell : *it) {
        std::string& ref = static_cast<DataType<std::string>&>(cell.get()).get_value();
        ref = f(ref);
    }
}
