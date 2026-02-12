#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>

struct Gene {
    std::string chr;
    int start;
    int end;
    int mid;
    int group;
};

int main(int argc, const char* argv[])
{
    if (argc < 4) {
        std::cerr << "Usage: ./task3 <input_file> <distance> <output_file>\n";
        return 1;
    }

    std::string in_path = argv[1];
    std::string out_path = argv[3];
    int max_gap = std::stoi(argv[2]);

    std::ifstream in(in_path);
    std::ofstream out(out_path);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Error: Failed to open input or output file.\n";
        return 1;
    }

    std::string line;
    std::vector<Gene> data;

    while (std::getline(in, line)) {
        if (line == "") continue;

        std::istringstream ss(line);
        Gene item;

        if (!(ss >> item.chr >> item.start >> item.end)) {
            continue;
        }

        item.mid = (item.start + item.end) / 2;
        item.group = 0;
        data.push_back(item);
    }

    for (size_t i = 1; i < data.size(); ++i) {
        auto& before = data[i - 1];
        auto& now = data[i];

        if (now.chr < before.chr) {
            std::cerr << "Error: Chromosomes not sorted.\n";
            return 1;
        }

        if (now.chr == before.chr && now.start < before.start) {
            std::cerr << "Error: Start positions not sorted in chromosome " << now.chr << "\n";
            return 1;
        }
    }

    int g = 1;
    if (!data.empty()) {
        data[0].group = g;

        for (size_t i = 1; i < data.size(); ++i) {
            bool same = data[i].chr == data[i - 1].chr;
            int gap = std::abs(data[i].mid - data[i - 1].mid);

            if (same && gap <= max_gap) {
                data[i].group = g;
            } else {
                g += 1;
                data[i].group = g;
            }
        }
    }

    for (const Gene& item : data) {
        out << item.chr << '\t'
            << item.start << '\t'
            << item.end << '\t'
            << "cluster=" << item.group << '\n';
    }

    return 0;
}
