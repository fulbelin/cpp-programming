#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int hamming_distance_calculator(std::string a, std::string b) {
    int sum = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            ++sum;
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "You must run the program like this: <n> <k>\n";
        std::cerr << "Where <n> is the position of the reference word (starting from 1),\n";
        std::cerr << "and <k> is the substring to search for.\n";
        return 1;
    }

    const int n = std::stoi(argv[1]);
    const std::string k = argv[2];
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            std::cout << '\n';
            continue;
        }

        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string w;
        while (iss >> w) {
            words.push_back(w);
        }

        int wordCount = words.size();
        if (n > wordCount) {
            std::cout << '\n';
            continue;
        }

        std::string reference_word = words[n - 1];
        std::vector<int> hamming_distance_vector;

        for (int i = 0; i < wordCount; ++i) {
            if (i == n - 1) continue;
            int distance = hamming_distance_calculator(reference_word, words[i]);
            hamming_distance_vector.push_back(distance);
        }

        if (hamming_distance_vector.empty()) {
            std::cout << '\n';
            continue;
        }

        int minimum_distance = hamming_distance_vector[0];
        int maximum_distance = hamming_distance_vector[0];
        int sum_distance = 0;

        for (int d : hamming_distance_vector) {
            if (d < minimum_distance) minimum_distance = d;
            if (d > maximum_distance) maximum_distance = d;
            sum_distance += d;
        }

        double average_distance = static_cast<double>(sum_distance) / hamming_distance_vector.size();

        std::sort(hamming_distance_vector.begin(), hamming_distance_vector.end());
        int size = hamming_distance_vector.size();
        double median_distance;
        if (size % 2 == 1) {
            median_distance = hamming_distance_vector[size / 2];
        }
        else {
            median_distance = (hamming_distance_vector[size / 2 - 1] + hamming_distance_vector[size / 2]) / 2.0;
        }

        int kCount = 0;
        for (std::string word : words) {
            std::size_t pos = 0;
            while ((pos = word.find(k, pos)) != std::string::npos) {
                ++kCount;
                ++pos;
            }
        }

        // Format average and median output like the sample
        std::cout << minimum_distance << ' ' << maximum_distance << ' ';

        if (average_distance == static_cast<int>(average_distance))
            std::cout << static_cast<int>(average_distance) << ' ';
        else
            std::cout.precision(1), std::cout << std::fixed << average_distance << ' ';

        if (median_distance == static_cast<int>(median_distance))
            std::cout << static_cast<int>(median_distance) << ' ';
        else
            std::cout.precision(1), std::cout << std::fixed << median_distance << ' ';

        std::cout << kCount << '\n';
    }

    return 0;
}
