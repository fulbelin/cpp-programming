#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;



// This is a list of FASTA records, where each record is a (header, sequence) pair

using FastaRecords = vector<std::pair<string, string>>;



// Read a FASTA file and return only valid DNA sequences (A, C, G, T)

FastaRecords read_fasta(const string& path)
{
    std::ifstream file(path);


    if (!file.is_open()) {


        cerr << "Error: could not open " << path << endl;
        std::exit(1);


    }

    FastaRecords records;
    string line, header, seq;



    // This helper saves a sequence only if it's valid DNA

    auto store_record_if_valid = [&]() {



        if (header.empty()) return;

        // Check if all characters in the sequence are A, C, G, or T

        bool is_dna = std::all_of(seq.begin(), seq.end(), [](char c) {
            c = std::toupper(c);
            return c == 'A' || c == 'C' || c == 'G' || c == 'T';
            });



        if (is_dna)

            records.emplace_back(header, seq);// Add the pair to the result
        else
            cerr << header << endl;   // If invalid, print header as a warning                
        seq.clear(); // Clear for the next record
        };

    while (std::getline(file, line)) {


        if (line.empty()) continue;

        // A new sequence starts with '>'

        if (line[0] == '>') {     


            store_record_if_valid();// Save previous record


            header = line.substr(1);  // Remove '>' to get the header             
        }
        else {

            // Add sequence line and convert to uppercase

            for (char c : line) seq += std::toupper(c);
        }
    }


    store_record_if_valid();    // Save last record after loop                     
    return records;
}



// Convert DNA base character to an integer (A=0, C=1, G=2, T=3)


inline int base_to_int(char b)
{
    switch (b) {
    case 'A': return 0; case 'C': return 1;
    case 'G': return 2; case 'T': return 3;
    }
    return -1;                                       
}

// Convert integer back to base (0=A, 1=C, 2=G, 3=T)

inline char int_to_base(int v)
{


    static const char lookup[4] = { 'A', 'C', 'G', 'T' };
    return lookup[v & 3];  // Only last 2 bits matter (0-3)
}


// Turn a k-mer string into a unique integer (bitwise hash)

uint64_t hash_kmer(const string& kmer)
{
    uint64_t h = 0;
    int k = static_cast<int>(kmer.size());


    for (int i = 0; i < k; ++i) {


        int val = base_to_int(kmer[i]); // Convert A/C/G/T to 0/1/2/3
        
        // Shift each value to the left by 2 * position (for base-4 encoding)

        h |= static_cast<uint64_t>(val) << (2 * (k - i - 1));
    }
    return h;
}

// Turn an integer back into the original k-mer string


string unhash_kmer(uint64_t id, int k)
{

    string kmer(k, 'A');

    for (int i = k - 1; i >= 0; --i) {


        kmer[i] = int_to_base(static_cast<int>(id & 3)); // Get last 2 bits
        id >>= 2; // Move to the next base
    }
    return kmer;
}


// Count k-mers in the sequence using a skip pattern

unordered_map<uint64_t, int>
count_skip_kmers(const string& seq, int k, int skip)
{
    unordered_map<uint64_t, int> counts;
    int step = skip + 1; // How many positions to jump between bases
    int window_len = step * (k - 1) + 1;  // Total span of one k-mer

    // If the sequence is too short, return an empty map

    if (seq.size() < static_cast<size_t>(window_len)) return counts;

    // Loop over all possible start positions for k-mers

    for (size_t start = 0; start + window_len <= seq.size(); ++start)
    {

        string kmer;

        kmer.reserve(k);

        // Collect every (skip+1)th base to make the k-mer

        for (int j = 0; j < k; ++j)

            kmer += seq[start + j * step];


        // Hash the k-mer and count it

        counts[hash_kmer(kmer)]++;
    }
    return counts;
}

// Convert the hash map into a sorted list of (kmer, count)

vector<std::pair<uint64_t, int>>

sorted_kmers(const unordered_map<uint64_t, int>& counts, int k)
{

    // Move map into a vector so we can sort it

    vector<std::pair<uint64_t, int>> vec(counts.begin(), counts.end());
    
    
    // Sort by the string version of the k-mer (alphabetically)

    std::sort(vec.begin(), vec.end(),
        [k](auto a, auto b) {
            return unhash_kmer(a.first, k) < unhash_kmer(b.first, k);
        });
    return vec;
}


int main(int argc, char* argv[])
{

    // Check input arguments

    if (argc != 4) {
        cerr << "Usage: ./task3 <fasta_file> <k> <skip>\n";
        return 1;
    }
    string fasta_path = argv[1]; //Path to input file
    int     k = std::stoi(argv[2]); // Length of k-mers
    int     skip = std::stoi(argv[3]); // Number of characters to skip


    // Read valid sequences from FASTA file

    FastaRecords records = read_fasta(fasta_path);


    // Loop through each sequence in the file

    for (const auto& [header, seq] : records)
    {

        // Count k-mers with skips

        auto counts = count_skip_kmers(seq, k, skip);


        if (counts.empty()) continue; // If no valid k - mers, skip it

        cout << header << '\n';  //Print the header first

        // Print all k-mers and their counts

        for (auto [hash, cnt] : sorted_kmers(counts, k))
            cout << unhash_kmer(hash, k) << ' ' << cnt << '\n';

        cout << '\n';                                 
    }
    return 0;
}
