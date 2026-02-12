#ifndef BLOOM_FILTER_HPP
#define BLOOM_FILTER_HPP

#include <bitset>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include "murmurhash.hpp"

// A hash wrapper that applies MurmurHash using a given seed
template <typename Key>
struct BloomHash {
    std::size_t operator()(Key key, unsigned int seed) const {
        return murmur3_32(reinterpret_cast<const uint8_t*>(&key), sizeof(Key), seed);
    }
};

// BloomFilter class template
// Key: type of elements stored (e.g., int, string hashable type)
// numBits: size of the bit array
// HashFunction: hashing strategy (defaults to BloomHash)
template <typename Key, unsigned int numBits, typename HashFunction = BloomHash<Key>>
class BloomFilter {
public:
    // Constructor: Initializes Bloom filter with a given number of hash functions
    explicit BloomFilter(unsigned int numberOfHashFunctions)
        : numberOfHashFunctions_(numberOfHashFunctions) {
        bitStorage_.reset(); // clear all bits to 0
    }

    // Constructor that takes an initializer list (e.g., {1, 2, 3}) and inserts keys into the filter
    BloomFilter(std::initializer_list<Key> initialKeys, unsigned int numberOfHashFunctions)
        : numberOfHashFunctions_(numberOfHashFunctions) {
        bitStorage_.reset();
        for (const Key& element : initialKeys) {
            insert(element);
        }
    }

    // Constructor that inserts keys using iterators from any container (e.g., vector, set)
    template <typename Iterator>
    BloomFilter(Iterator iteratorBegin, Iterator iteratorEnd, unsigned int numberOfHashFunctions)
        : numberOfHashFunctions_(numberOfHashFunctions) {
        bitStorage_.reset();
        for (Iterator current = iteratorBegin; current != iteratorEnd; ++current) {
            insert(*current);
        }
    }

    // Inserts a key into the Bloom filter
    // Returns true if at least one new bit was set (i.e., changed from 0 to 1)
    bool insert(const Key& element) {
        bool atLeastOneBitNewlySet = false;

        for (unsigned int hashIndex = 0; hashIndex < numberOfHashFunctions_; hashIndex++) {
            std::size_t bitPosition = hasher_(element, hashIndex) % numBits;
            if (!bitStorage_.test(bitPosition)) {
                atLeastOneBitNewlySet = true; // A new bit is set, which was previously 0
            }
            bitStorage_.set(bitPosition); // Set bit to 1
        }

        return atLeastOneBitNewlySet; // true if any new bit was changed from 0 to 1
    }

    // Checks if a key is *possibly* in the filter
    // Returns false if definitely not present, true if possibly present (could be a false positive)
    bool contains(const Key& element) const {
        for (unsigned int hashIndex = 0; hashIndex < numberOfHashFunctions_; hashIndex++) {
            std::size_t bitPosition = hasher_(element, hashIndex) % numBits;
            if (!bitStorage_.test(bitPosition)) {
                return false; // If any bit is 0, the key was not inserted
            }
        }
        return true; // possibly in the set (could be a false positive)
    }

    // Calculate false positive rate
    template <typename Iterator>
    double false_positive_rate(
        Iterator knownPositivesBegin, Iterator knownPositivesEnd,
        Iterator knownNegativesBegin, Iterator knownNegativesEnd) const {

        unsigned int falsePositiveCount = 0;
        unsigned int totalNegativeSamples = 0;

        for (Iterator it = knownPositivesBegin; it != knownPositivesEnd; ++it) {
            contains(*it);
        }

        for (Iterator it = knownNegativesBegin; it != knownNegativesEnd; ++it) {
            totalNegativeSamples++;
            if (contains(*it)) {
                falsePositiveCount++;
            }
        }

        if (totalNegativeSamples == 0) return 0.0; // Avoid division by zero

        return static_cast<double>(falsePositiveCount) / totalNegativeSamples;
    }

    // Compares space used by the filter vs. optimal theoretical minimum
    double space_ratio(uint64_t expectedElementCount) const {
        std::size_t actualBitMemory = sizeof(bitStorage_);  // Real memory usage
        std::size_t optimalBitMemory = expectedElementCount * sizeof(Key);  // Estimate
        return static_cast<double>(actualBitMemory) / optimalBitMemory;
    }

    // Estimates how many elements have been inserted, based on number of set bits
    uint64_t approx_size() const {
        int totalBitsSet = 0;

        for (unsigned int i = 0; i < numBits; i++) {
            if (bitStorage_.test(i)) {
                totalBitsSet++;
            }
        }

        double fractionOfBitsSet = static_cast<double>(totalBitsSet) / numBits;

        if (fractionOfBitsSet == 1.0) {
            return static_cast<uint64_t>(-1); // Bloom filter is saturated, can't estimate
        }

        // Standard formula for estimating cardinality in Bloom filters
        double estimatedInsertedElements =
            -static_cast<double>(numBits) / numberOfHashFunctions_ *
            std::log(1.0 - fractionOfBitsSet);

        return static_cast<uint64_t>(estimatedInsertedElements);
    }

private:
    std::bitset<numBits> bitStorage_;           // Bit array representing the Bloom filter
    unsigned int numberOfHashFunctions_;        // Number of hash functions used
    HashFunction hasher_;                       // hash function functor
};

#endif  // BLOOM_FILTER_HPP
