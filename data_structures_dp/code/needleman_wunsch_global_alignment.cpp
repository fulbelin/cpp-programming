#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const int match = 1;
const int mismatch = -1;
const int gap_penalty = -1;

void init_table(std::vector<std::vector<int>>& table)
{
    int rows = table.size();
    int cols = table[0].size();

    for (int i = 0; i < rows; ++i)
        table[i][0] = i * gap_penalty;

    for (int j = 0; j < cols; ++j)
        table[0][j] = j * gap_penalty;
}

void fill_table(std::vector<std::vector<int>>& table,
    const std::string& seq1,
    const std::string& seq2)
{
    int rows = table.size();
    int cols = table[0].size();

    for (int i = 1; i < rows; ++i) {
        for (int j = 1; j < cols; ++j) {

            int diag = table[i - 1][j - 1];
            if (seq1[i - 1] == seq2[j - 1])
                diag += match;
            else
                diag += mismatch;

            int up = table[i - 1][j] + gap_penalty;
            int left = table[i][j - 1] + gap_penalty;

            table[i][j] = std::max({ diag, up, left });
        }
    }
}

void backtrack(const std::vector<std::vector<int>>& table,
    const std::string& seq1,
    const std::string& seq2,
    int i, int j,
    std::string a1,
    std::string a2,
    std::vector<std::pair<std::string, std::string>>& out)
{
    if (i == 0 && j == 0) {
        std::reverse(a1.begin(), a1.end());
        std::reverse(a2.begin(), a2.end());
        out.emplace_back(a1, a2);
        return;
    }

    if (i > 0 && table[i][j] == table[i - 1][j] + gap_penalty) {
        backtrack(table, seq1, seq2, i - 1, j, a1 + seq1[i - 1], a2 + '-', out);
    }

    if (j > 0 && table[i][j] == table[i][j - 1] + gap_penalty) {
        backtrack(table, seq1, seq2, i, j - 1, a1 + '-', a2 + seq2[j - 1], out);
    }

    if (i > 0 && j > 0) {
        int sc = (seq1[i - 1] == seq2[j - 1]) ? match : mismatch;
        if (table[i][j] == table[i - 1][j - 1] + sc) {
            backtrack(table, seq1, seq2, i - 1, j - 1, a1 + seq1[i - 1], a2 + seq2[j - 1], out);
        }
    }
}

void print_out(int score, const std::vector<std::pair<std::string, std::string>>& out)
{
    std::cout << score << '\n';
    for (std::size_t i = 0; i < out.size(); ++i) {
        std::cout << out[i].first << '\n' << out[i].second << '\n';
        if (i != out.size() - 1)
            std::cout << '\n';
    }
    std::cout << '\n';
}

int main(int argc, const char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: ./task4 <sequence1> <sequence2>\n";
        return 1;
    }

    std::string seq1 = argv[1];
    std::string seq2 = argv[2];

    int rows = seq1.length() + 1;
    int cols = seq2.length() + 1;

    std::vector<std::vector<int>> table(rows, std::vector<int>(cols, 0));
    init_table(table);
    fill_table(table, seq1, seq2);

    int score = table[rows - 1][cols - 1];

    std::vector<std::pair<std::string, std::string>> result;
    backtrack(table, seq1, seq2, seq1.length(), seq2.length(), "", "", result);

    print_out(score, result);
    return 0;
}
