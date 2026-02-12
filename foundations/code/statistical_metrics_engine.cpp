#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

std::vector<int> all_kaprekar;
std::vector<int> all_lychrel;

// Calculates the sum of all numbers in the vector
double calculate_sum(const std::vector<double>& numbers) {
    double sum = 0;
    for (int i = 0; i < numbers.size(); i++) {
        sum += numbers[i];
    }
    return sum;
}

// Calculates the sum of the digits of a number
int calculate_digit_sum(double n) {
    std::string s = std::to_string(n);
    int digit_sum = 0;
    for (char c : s) {
        if (isdigit(c)) {
            digit_sum += c - '0';
        }
    }
    return digit_sum;
}

// Calculates the third quartile (Q3) using interpolation
double calculate_third_quartile(const std::vector<double>& numbers) {
    std::size_t numbers_count = numbers.size();
    std::vector<double> sorted_numbers = numbers;
    std::sort(sorted_numbers.begin(), sorted_numbers.end());

    double position_q3 = 0.75 * (numbers_count + 1);

    int whole_part = static_cast<int>(position_q3);
    double fractional_part = position_q3 - whole_part;
    int index = whole_part - 1;

    if (whole_part >= numbers_count) {
        return sorted_numbers[numbers_count - 1];
    }

    double lower_value = sorted_numbers[index];
    double upper_value = sorted_numbers[index + 1];
    double interpolated = lower_value + fractional_part * (upper_value - lower_value);

    return interpolated;
}

// Finds the nth smallest number in the vector
double find_nth_smallest(const std::vector<double>& numbers, int n) {
    std::vector<double> sorted_numbers = numbers;
    std::sort(sorted_numbers.begin(), sorted_numbers.end());
    return sorted_numbers[n - 1];
}

// Finds the mth largest number in the vector
double find_mth_largest(const std::vector<double>& numbers, int m) {
    std::vector<double> sorted_numbers = numbers;
    std::sort(sorted_numbers.begin(), sorted_numbers.end(), std::greater<>());
    return sorted_numbers[m - 1];
}

// Checks for Kaprekar numbers and stores them if found
void kaprekar_number_check(const std::vector<double>& numbers) {
    for (int i = 0; i < numbers.size(); i++) {
        int n = static_cast<int>(numbers[i]);
        if (n <= 0) continue;

        long long squared = static_cast<long long>(n) * n;
        std::string square_string = std::to_string(squared);

        for (size_t j = 1; j < square_string.length(); j++) {
            std::string left = square_string.substr(0, j);
            std::string right = square_string.substr(j);

            int left_part_number = std::stoi(left);
            int right_part_number = std::stoi(right);

            if (right_part_number == 0) continue;

            if (left_part_number + right_part_number == n) {
                if (std::find(all_kaprekar.begin(), all_kaprekar.end(), n) == all_kaprekar.end()) {
                    all_kaprekar.push_back(n);
                }
                break;
            }
        }

        if (n == 1 && std::find(all_kaprekar.begin(), all_kaprekar.end(), n) == all_kaprekar.end()) {
            all_kaprekar.push_back(n);
        }
    }
}

// Checks for Lychrel numbers and stores them if found
void lychrel_number_check(const std::vector<double>& numbers) {
    for (double d : numbers) {
        int num = static_cast<int>(d);
        int original = num;
        bool is_palindrome = false;

        for (int iter = 0; iter < 500 && num <= 1'000'000'000; ++iter) {
            int temp = num;
            std::string reversed_string;
            while (temp > 0) {
                reversed_string += std::to_string(temp % 10);
                temp /= 10;
            }

            int reversed_integer = std::stoi(reversed_string);
            num += reversed_integer;

            std::string num_str = std::to_string(num);
            std::string reversed_num_str = num_str;
            std::reverse(reversed_num_str.begin(), reversed_num_str.end());
            if (num_str == reversed_num_str) {
                is_palindrome = true;
                break;
            }
        }

        if (!is_palindrome) {
            if (std::find(all_lychrel.begin(), all_lychrel.end(), original) == all_lychrel.end()) {
                all_lychrel.push_back(original);
            }
        }
    }
}

// Prints all integers in a vector separated by spaces
void print_vector(const std::vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1) std::cout << " ";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./task4 <n> <m>\n";
        return 1;
    }

    const int n = std::stoi(argv[1]);
    const int m = std::stoi(argv[2]);
    std::string line;

    // Process input line by line
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            std::cout << "\r\n";
            continue;
        }

        std::istringstream iss(line);
        std::vector<double> numbers;
        std::string w;

        // Read all numbers in the line
        while (iss >> w) {
            numbers.push_back(std::stod(w));
        }

        // Collect unsigned integers for Kaprekar and Lychrel checks
        std::vector<double> unsigned_integers_only;
        for (double d : numbers) {
            if (d >= 0 && static_cast<int>(d) == d) {
                unsigned_integers_only.push_back(d);
            }
        }

        kaprekar_number_check(unsigned_integers_only);
        lychrel_number_check(unsigned_integers_only);

        // Handle case with only one number
        if (numbers.size() == 1) {
            int num_as_int = static_cast<int>(numbers[0]);
            std::cout << num_as_int << " ";
            std::cout << calculate_digit_sum(numbers[0]) << " Iq";
            if (n > 1) std::cout << " In";
            if (m > 1) std::cout << " Im";
            std::cout << "\r\n";
            continue;
        }

        // Handle invalid n or m values
        if (n > numbers.size() || m > numbers.size()) {
            double sum = calculate_sum(numbers);
            std::cout << std::fixed << std::setprecision(1) << sum << " ";
            std::cout << std::defaultfloat << calculate_digit_sum(sum);
            if (n > numbers.size()) std::cout << " In";
            if (m > numbers.size()) std::cout << " Im";
            std::cout << "\r\n";
            continue;
        }

        // Compute all statistics
        double sum = calculate_sum(numbers);
        double q3 = calculate_third_quartile(numbers);
        double nth = find_nth_smallest(numbers, n);
        double mth = find_mth_largest(numbers, m);

        std::ostringstream out;
        out << std::fixed << std::setprecision(1) << sum << " ";
        out << std::defaultfloat << calculate_digit_sum(sum) << " ";
        out << std::fixed << std::setprecision(1) << q3 << " " << nth << " " << mth;

        std::cout << out.str() << "\r\n";
    }

    // Print Kaprekar numbers
    if (!all_kaprekar.empty()) {
        print_vector(all_kaprekar);
        std::cout << "\r\n";
    }
    else {
        std::cout << "\r\n";
    }

    // Print Lychrel numbers
    if (!all_lychrel.empty()) {
        print_vector(all_lychrel);
        std::cout << "\r\n";
    }
    else {
        std::cout << "\r\n";
    }

    return 0;
}
