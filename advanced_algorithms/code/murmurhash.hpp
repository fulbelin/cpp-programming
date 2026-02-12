#ifndef MURMURHASH_HPP
#define MURMURHASH_HPP

#include <cstdint>

// implementation of mumurhash 3 from wikipedia
// https://en.wikipedia.org/wiki/MurmurHash
// usage:
// int i = 50;
// uint32_t seed = 42;
// murmur3_32(reinterpret_cast<uint8_t*>(&i), sizeof(i), seed);


// This function takes a pointer to some raw data (`key`),
// its length (`len`), and a seed value (`seed`).
// It returns a 32-bit unsigned integer as the hash.
uint32_t murmur3_32(const uint8_t* key, std::size_t len, uint32_t seed) {

  uint32_t h = seed; // Initialize hash with the seed value

  // Process 4 bytes at a time
  if (len > 3) {
      const uint32_t* blocks = reinterpret_cast<const uint32_t*>(key);
      std::size_t num_blocks = len / 4;

      for (std::size_t i = 0; i < num_blocks; ++i) {
          uint32_t k = blocks[i];  // Read 4 bytes (one block)

          // Mix the bits with constants to scramble them
          k *= 0xcc9e2d51;
          k = (k << 15) | (k >> 17); // rotate left 15 bits
          k *= 0x1b873593;

          h ^= k; // combine with running hash
          h = (h << 13) | (h >> 19);  // rotate left 13 bits
          h = h * 5 + 0xe6546b64;
      }

      key += num_blocks * 4; // move pointer to remaining bytes
      len &= 3;              // get number of leftover bytes (len % 4)
  }

  // Process any remaining bytes (less than 4)
  if (len > 0) {
      uint32_t k = 0;

      // Read remaining bytes in reverse order
      for (std::size_t i = 0; i < len; ++i) {
          k |= key[i] << (i * 8); // shift byte and add to k
      }

      // Mix the remaining bits like above
      k *= 0xcc9e2d51;
      k = (k << 15) | (k >> 17);
      k *= 0x1b873593;
      h ^= k;
  }

  // Finalize the hash by mixing in the total length
  h ^= static_cast<uint32_t>(len);

  // Final steps to ensure good distribution
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h; // Return the final hash value
}

#endif  // MURMURHASH_HPP