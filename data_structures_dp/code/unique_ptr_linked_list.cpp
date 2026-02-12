#include "simple_node.hpp"
#include <utility>

/**
 * Constructor: creates a node with the given value and no successor.
 */
SimpleNode::SimpleNode(double value) {
    this->content_ = value;
    this->successor_.reset();  // No successor initially
}

/**
 * Copy constructor: performs a deep copy of the node and its entire successor chain.
 */
SimpleNode::SimpleNode(const SimpleNode& other) {
    content_ = other.content_;
    if (other.successor_) {
        successor_ = std::make_unique<SimpleNode>(*other.successor_);
    }
    else {
        successor_ = nullptr;
    }
}

/**
 * Move constructor: transfers ownership of the successor chain.
 */
SimpleNode::SimpleNode(SimpleNode&& other) noexcept {
    this->content_ = other.content_;
    this->successor_ = std::move(other.successor_);
    other.successor_ = nullptr;  // Clear moved-from object
}

/**
 * Move assignment: safely transfers content and successor from another node.
 */
SimpleNode& SimpleNode::operator=(SimpleNode&& other) noexcept {
    if (this != &other) {
        this->content_ = other.content_;
        this->successor_ = std::move(other.successor_);
        other.successor_ = nullptr;
    }
    return *this;
}

/**
 * Inserts a copy of the given node as the direct successor.
 * The old successor chain is appended to the end of the new one.
 */
void SimpleNode::insert(SimpleNode& successor_node) {
    std::unique_ptr<SimpleNode> temp = std::move(successor_);
    successor_ = std::make_unique<SimpleNode>(successor_node);

    // Traverse to the tail of the new chain
    SimpleNode* walker = successor_.get();
    while (!walker->is_tail()) {
        walker = walker->successor_.get();
    }

    // Reattach the original successor
    walker->successor_ = std::move(temp);
}

/**
 * Returns a reference to the next node in the chain.
 * Throws an exception if called on a tail node.
 */
SimpleNode& SimpleNode::next() const {
    if (successor_ == nullptr) {
        throw Invalid();  // No next node exists
    }
    return *successor_;
}

/**
 * Returns true if the node has no successor (i.e., it's the tail).
 */
bool SimpleNode::is_tail() const {
    return successor_ == nullptr;
}

/**
 * Computes and returns the length of the linked list starting from this node.
 */
std::size_t SimpleNode::length() const {
    std::size_t count = 1;
    const SimpleNode* node = this;

    while (node->successor_ != nullptr) {
        count++;
        node = node->successor_.get();
    }

    return count;
}

/**
 * Accessor (non-const): allows modifying the node's content via function-call syntax.
 */
double& SimpleNode::operator()() {
    return content_;
}

/**
 * Accessor (const): provides read-only access to the node's content.
 */
double SimpleNode::operator()() const {
    return content_;
}
