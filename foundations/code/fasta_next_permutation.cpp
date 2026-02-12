#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

// Converts RNA to DNA and removes invalid sequences
void RNA_DNA_converter(std::vector<std::string>& words) {
    std::vector<std::string> cleaned;

    for (int i = 0; i < words.size(); i++) {
        std::string seq = words[i];
        bool isValid = true;

        std::string cleaned_seq = "";
        // Remove spaces and make letters uppercase
        for (char c : seq) {
            if (!isspace(c)) {
                cleaned_seq += std::toupper(c);
            }
        }

        // Convert U to T (RNA to DNA)
        for (int j = 0; j < cleaned_seq.length(); j++) {
            if (cleaned_seq[j] == 'U') {
                cleaned_seq[j] = 'T';
            }
        }

        // Check if sequence contains only valid DNA letters
        for (int j = 0; j < cleaned_seq.length(); j++) {
            char c = cleaned_seq[j];
            if (c != 'A' && c != 'T' && c != 'C' && c != 'G') {
                isValid = false;
                break;
            }
        }

        // Add valid sequence to cleaned list
        if (!isValid) {
            continue;
        }

        cleaned.push_back(cleaned_seq);
    }

    words = cleaned;
}

// Tries to find the next lexicographic permutation
bool nextPermutationIfPossible(std::string& sequence) {
    return std::next_permutation(sequence.begin(), sequence.end());
}

int main(int argc, char* argv[]) {
    // Check for input file argument
    if (argc < 2) {
        std::cerr << "Usage: ./task6 <input_file.fasta>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);

    // If file cannot be opened
    if (!inputFile) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<std::string> sequences;
    std::string line;
    std::string currentSequence;

    // Read and parse FASTA file
    while (std::getline(inputFile, line)) {
        if (!line.empty() && line[0] == '>') {
            // Save previous sequence when a new header is found
            if (!currentSequence.empty()) {
                sequences.push_back(currentSequence);
                currentSequence = "";
            }
        }
        else if (!line.empty()) {
            // Add sequence data
            currentSequence += line;
        }
    }

    // Add the last sequence (if any)
    if (!currentSequence.empty()) {
        sequences.push_back(currentSequence);
    }

    // Convert RNA to DNA and remove invalid sequences
    RNA_DNA_converter(sequences);

    std::vector<std::string> results;

    // Generate next permutation if possible
    for (int i = 0; i < sequences.size(); i++) {
        std::string temp = sequences[i];
        if (nextPermutationIfPossible(temp)) {
            results.push_back(temp);
        }
    }

    // Sort results by length, then lexicographically
    std::sort(results.begin(), results.end(), [](const std::string& a, const std::string& b) {
        if (a.length() == b.length()) return a < b;
        return a.length() < b.length();
        });

    // Print the final sorted results
    for (int i = 0; i < results.size(); i++) {
        std::cout << results[i] << std::endl;
    }

    return 0;
}
