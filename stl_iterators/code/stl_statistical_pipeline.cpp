#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <cstdint>
#include <string>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <sstream>

using namespace std;

// This function finds the average of a vector

double calculating_mean(const vector<double>& v) {
    return accumulate(v.begin(), v.end(), 0.0) /
        static_cast<double>(v.size());
}

// This function finds the standard deviation of a vector
double calculating_stddeviation(const vector<double>& v)
{
    double mean = calculating_mean(v);

    double sum_squared = accumulate(
        v.begin(), v.end(), 0.0,
        [mean](double sum, double x) {
            double diff = x - mean;
            return sum + diff * diff;
        }
    );
    return sqrt(sum_squared / static_cast<double>(v.size() - 1));
}

// This function scales the numbers using log2
vector<double> scaler(const vector<double>& v)
{
    vector<double> scaled;
    scaled.reserve(v.size());

    auto log2_scale = [](double value) {
        return log2(std::abs(value) + 1.0);// +1 so log2(0) doesn't happen
        };

    transform(v.begin(), v.end(),
        back_inserter(scaled),
        log2_scale);

    return scaled;
}

// This function calculates the pooled standard deviation for two groups
double calculate_pooled_standard_deviation(double s1, size_t n1,
    double s2, size_t n2)
{
    double weighted =
        (n1 - 1) * s1 * s1 +
        (n2 - 1) * s2 * s2;

    double df = static_cast<double>(n1 + n2 - 2);
    return sqrt(weighted / df);
}
// This function calculates the t-value for comparing two groups
double calculate_student_t_equal_variance(double m1, double s1, size_t n1,
    double m2, double s2, size_t n2)
{
    double sp = calculate_pooled_standard_deviation(s1, n1, s2, n2);

    double inv_sizes =
        1.0 / static_cast<double>(n1) +
        1.0 / static_cast<double>(n2);

    double denom = sp * sqrt(inv_sizes);
    return (m1 - m2) / denom;
}

// This gives us the total degrees of freedom
inline size_t degrees_of_freedom_equal_variance(size_t n1, size_t n2)
{
    return n1 + n2 - 2;
}

// Formats a number to max 2 decimal places and removes unnecessary zeros
string format_two_decimals_trim(double value)
{
    ostringstream out;
    out << fixed << setprecision(2) << value;
    string s = out.str();
    while (!s.empty() && s.back() == '0') s.pop_back();
    if (!s.empty() && s.back() == '.')   s.pop_back();
    return s;
}

// Formats to 1 or 2 decimal places depending on which looks better
string format_one_or_two_decimals(double value)
{
    double two = round(value * 100.0) / 100.0;
    double one = round(value * 10.0) / 10.0;

    double chosen = (fabs(two - one) < 0.015) ? two : one;

    ostringstream out;
    out << fixed << setprecision(2) << chosen;
    string s = out.str();
    while (!s.empty() && s.back() == '0') s.pop_back();
    if (!s.empty() && s.back() == '.')   s.pop_back();
    return s;
}

// This prints the top numbers from a vector
void print_top_elements(const vector<double>& values,
    size_t               numToShow)
{
    size_t count = min(values.size(), numToShow);
    for (size_t idx = 0; idx < count; ++idx) {
        double rounded = round(values[idx] * 100.0) / 100.0;
        cout << format_two_decimals_trim(rounded);
        if (idx + 1 < count) cout << ", ";
    }
    cout << '\n';
}


int main(int argc, const char* argv[])
{
    if (argc < 6) {// Check if the user gave enough arguments
        cerr << "Usage: " << argv[0]
            << " <n> <m> <seed> <p> <k>\n";
        return 1;
    }

    size_t n = stoul(argv[1]);// size of V1
    size_t m = stoul(argv[2]);// size of V2
    size_t seed = stoul(argv[3]);// random seed
    double p = stod(argv[4]);// success probability
    unsigned k = stoul(argv[5]); // number of successes


    // Check if the input values are valid

    if (n < 2 || m < 2 || k == 0 || p <= 0.0 || p >= 1.0) {
        cerr << "Parameters entered are invalid.\n";
        return 1;
    }


    // Generate random values for V1 using negative binomial

    mt19937_64 gen1(seed);
    negative_binomial_distribution<> negBin1(k, p);


    // Calculate statistics for V1

    vector<double> V1(n);
    generate(V1.begin(), V1.end(), [&] { return negBin1(gen1); });

    double mean1 = calculating_mean(V1);
    double sd1 = calculating_stddeviation(V1);

    vector<double> V1_scaled_unsorted = scaler(V1);// keep original order for later
    vector<double> V1_scaled = V1_scaled_unsorted;
    sort(V1_scaled.begin(), V1_scaled.end(), greater<>());


    // Print V1 stats

    cout << "V1 Mean: "
        << static_cast<int>(round(mean1)) << '\n';
    cout << "V1 Sample standard deviation: "
        << fixed << setprecision(2) << sd1 << '\n';
    cout << "V1 Top 5 elements: ";
    print_top_elements(V1_scaled, 5);


    // Generate random values for V2

    mt19937_64 gen2(seed * 2ULL);
    negative_binomial_distribution<> negBin2(k, p);

    vector<double> V2(m);
    generate(V2.begin(), V2.end(), [&] { return negBin2(gen2); });



    // Calculate statistics for V2

    double mean2 = calculating_mean(V2);
    double sd2 = calculating_stddeviation(V2);

    vector<double> V2_scaled = scaler(V2);
    sort(V2_scaled.begin(), V2_scaled.end(), greater<>());


    // Print V2 stats

    cout << "V2 Mean: "
        << fixed << setprecision(1)
        << round(mean2 * 10.0) / 10.0 << '\n';
    cout << "V2 Sample standard deviation: "
        << fixed << setprecision(2) << sd2 << '\n';
    cout << "V2 Top 8 elements: ";
    print_top_elements(V2_scaled, 8);


    // Compare V1 and V2 using unscaled values

    cout << "\nComparing V1 and V2...\n";                 

    double t_raw =
        calculate_student_t_equal_variance(mean1, sd1, n,
            mean2, sd2, m);
    cout << "Unscaled t-statistic: "
        << fixed << setprecision(3) << t_raw << '\n';



    // Compare using scaled values (log2)

    cout << "Comparing log2 scaled V1 and V2...\n";        

    double mean1s = calculating_mean(V1_scaled_unsorted);
    double sd1s = calculating_stddeviation(V1_scaled_unsorted);
    double mean2s = calculating_mean(V2_scaled);
    double sd2s = calculating_stddeviation(V2_scaled);

    double t_scaled =
        calculate_student_t_equal_variance(mean1s, sd1s, n,
            mean2s, sd2s, m);
    cout << "Scaled t-statistic: "                         
        << fixed << setprecision(3) << t_scaled << '\n';


    // Print degrees of freedom

    size_t df = degrees_of_freedom_equal_variance(n, m);
    cout << "Total degrees of freedom: " << df << '\n';


    // Create a shuffled sequence from 1 to n

    vector<double> sequence(n);
    iota(sequence.begin(), sequence.end(), 1);

    mt19937_64 gen3(seed * 7ULL);
    shuffle(sequence.begin(), sequence.end(), gen3);


    // Multiply each element in the sequence with V1_scaled values

    vector<double> mixed(n);
    for (size_t i = 0; i < n; ++i)
        mixed[i] = sequence[i] * V1_scaled_unsorted[i];


    // Sort mixed vector using even-odd rule

    stable_sort(mixed.begin(), mixed.end(),
        [](double a, double b)
        {
            int ar = static_cast<int>(nearbyint(a));
            int br = static_cast<int>(nearbyint(b));
            bool a_even = ar % 2 == 0;
            bool b_even = br % 2 == 0;
            if (a_even != b_even) return a_even; // even values go first
            return a_even ? a > b : a < b; // if both even: descending, else ascending
        });
    // Print the final sorted vector

    cout << "\nSorted vector: ";
    for (size_t i = 0; i < mixed.size(); ++i) {
        cout << format_one_or_two_decimals(mixed[i]);
        if (i + 1 < mixed.size()) cout << ", ";
    }
    cout << '\n';
}
