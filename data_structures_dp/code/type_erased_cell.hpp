#ifndef ASSIGNMENT2_COLTYPE_HPP
#define ASSIGNMENT2_COLTYPE_HPP

#include <memory>
#include <iostream>
#include <type_traits>

/**
 * Abstract base class that defines a generic data type interface.
 * Used to store values of different types under a common interface.
 */
class GenericDataType {
public:
    virtual ~GenericDataType() = default;

    // Creates a deep copy of the object (used for polymorphic copying).
    virtual std::unique_ptr<GenericDataType> clone() const = 0;

    // Allows generic printing through polymorphism.
    friend std::ostream& operator<<(std::ostream& os, const GenericDataType& v);

    // Enables comparison between different GenericDataType instances.
    virtual bool operator==(const GenericDataType& other) const = 0;
    virtual bool operator<(const GenericDataType& other) const = 0;

protected:
    // Internal implementation of printing logic (must be overridden).
    virtual void print(std::ostream& os) const = 0;
};

/**
 * Delegates streaming operator to internal print function.
 */
inline std::ostream& operator<<(std::ostream& os, const GenericDataType& v)
{
    v.print(os);
    return os;
}

/**
 * Templated implementation of GenericDataType for holding actual typed values.
 * This class wraps a value of type T and provides cloning and comparison.
 */
template <typename T>
class DataType : public GenericDataType {
    T value;

public:
    explicit DataType(const T& val) : value(val) {}

    std::unique_ptr<GenericDataType> clone() const override {
        return std::make_unique<DataType<T>>(*this);
    }

    const T& get_value() const { return value; }
    T& get_value() { return value; }
    explicit operator T() const { return value; }

    bool operator==(const GenericDataType& other) const override {
        auto* dt = dynamic_cast<const DataType<std::remove_reference_t<T>>*>(&other);
        return dt && value == dt->value;
    }

    bool operator<(const GenericDataType& other) const override {
        auto* dt = dynamic_cast<const DataType<std::remove_reference_t<T>>*>(&other);
        return dt && value < dt->value;
    }

protected:
    void print(std::ostream& os) const override {
        os << value;
    }
};

/**
 * Type-erased wrapper that can hold any DataType<T> and supports copying, moving, printing, and comparing.
 * Used to store column values of any type in a unified container.
 */
class ColType {
    std::unique_ptr<GenericDataType> container;

public:
    // Constructor that wraps any type into a GenericDataType.
    template <typename ValueType>
    explicit ColType(const ValueType& v)
        : container(std::make_unique<DataType<ValueType>>(v)) {
    }

    // Copy constructor performs deep copy via clone.
    ColType(const ColType& other)
    {
        if (other.container)
            container = other.container->clone();
        else
            container.reset();
    }

    // Copy assignment operator using deep cloning.
    ColType& operator=(const ColType& other)
    {
        if (this != &other) {
            if (other.container)
                container = other.container->clone();
            else
                container.reset();
        }
        return *this;
    }

    // Move constructors/assignments: default behavior is safe and efficient.
    ColType(ColType&&) noexcept = default;
    ColType& operator=(ColType&&) noexcept = default;

    // Accessors for internal polymorphic object
    GenericDataType& get() { return *container; }
    const GenericDataType& get() const { return *container; }

    // Equality and ordering compare the wrapped values using virtual interface
    bool operator==(const ColType& other) const {
        return *container == *other.container;
    }

    bool operator<(const ColType& other) const {
        return *container < *other.container;
    }
};

#endif // ASSIGNMENT2_COLTYPE_HPP