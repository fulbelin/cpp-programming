#ifndef SIMPLE_NODE_HPP
#define SIMPLE_NODE_HPP

#include <vector>
#include <memory>

/**
 * SimpleNode implements a singly linked node that holds a double value.
 * Each node owns its successor via a unique_ptr, enabling automatic memory management.
 * Nodes can be deep-copied or moved, and form chains through insertion.
 */
class SimpleNode
{
public:
    // Exception class for invalid operations (e.g., accessing next on tail).
    class Invalid {};

    /**
     * Constructs a new node with the given value and no successor.
     */
    explicit SimpleNode(double content);

    /**
     * Copy constructor: performs a deep copy of the entire chain.
     */
    SimpleNode(const SimpleNode& other_node);

    /**
     * Move constructor and assignment: transfers ownership of the successor chain.
     */
    SimpleNode(SimpleNode&& other_node) noexcept;
    SimpleNode& operator=(SimpleNode&& other_node) noexcept;

    /**
     * Inserts a copy of the given node as the current node's successor.
     * Any existing successor is preserved and attached to the new tail.
     */
    void insert(SimpleNode& successor_node);

    /**
     * Returns a reference to the next node. Throws Invalid if this is the tail.
     */
    SimpleNode& next() const;

    /**
     * Returns true if the node has no successor.
     */
    bool is_tail() const;

    /**
     * Returns the length of the node chain starting from this node (including this node).
     */
    std::size_t length() const;

    /**
     * Allows read and write access to the node's content via function call syntax.
     */
    double& operator()();

    /**
     * Read-only version of the function call operator.
     */
    double operator()() const;

private:
    double content_;  // Value held by the node
    std::unique_ptr<SimpleNode> successor_;  // Owned next node in the chain
};

#endif // SIMPLE_NODE_HPP
