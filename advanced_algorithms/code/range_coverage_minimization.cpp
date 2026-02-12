#include <iostream>
#include <vector>
using namespace std;

// Function to calculate the minimum number of beamers needed to cover all cities
// Beamers can cover a range of k cities to both left and right from their position

int finding_beamers(const vector<int>& cities, int k) {
    int n = cities.size();
    vector<bool> covered(n, false);  // Keeps track of whether each city is covered
    int count = 0; // Counter for number of beamers used
    int i = 0;   // Current city index

    while (i < n) {
        if (covered[i]) {
            ++i;  // If already covered, move to next city
            continue;
        }

        int found = -1;

        // Try to find a beamer that is represented by 1 within range that can cover city i
        for (int j = min(n - 1, i + (k - 1)); j >= max(0, i - (k - 1)); --j) {
            if (cities[j] == 1) {
                found = j;  // Found a beamer
                break;
            }
        }

        if (found == -1) {
            return -1; // No beamer can cover this city
        }

        // Mark all cities within the beamer's range as covered
        for (int j = max(0, found - (k - 1)); j <= min(n - 1, found + (k - 1)); ++j) {

            covered[j] = true;
        }

        count++;  // Increase beamer count
        i = found + k;  // Jump ahead to skip already covered range
    }

    return count;
}


// Function to calculate the minimum number of catapults needed to cover all cities
// Catapults are represented by 2 and have a coverage range of l to both directions

int finding_catapults(const vector<int>& cities, int l) {
    int n = cities.size();
    vector<bool> covered(n, false); // Keeps track of covered cities
    int count = 0; 
    int i = 0;

    while (i < n) {
        if (covered[i]) {
            ++i; // Already covered, move to next
            continue;
        }

        int found = -1;

        // Try to find a catapult (value 2) within coverage range

        for (int j = min(n - 1, i + (l - 1)); j >= max(0, i - (l - 1)); --j) {
            if (cities[j] == 2) {
                found = j;
                break;
            }
        }

        if (found == -1) {
            return -1;  // no catapult can cover this city
        }

        // Mark cities covered by this catapult


        for (int j = max(0, found - (l - 1)); j <= min(n - 1, found + (l - 1)); ++j) {
            covered[j] = true;
        }

        count++; // Increment catapult count

        i = found + l;  // Move forward to next uncovered city
    }

    return count;
}




int main(int argc, const char* argv[]) {

    size_t n, k, l;
    cin >> n >> k >> l; // Read number of cities, beamer range, and catapult range

    vector<int> cities(n);
    for (size_t i = 0; i < n; ++i) {
        cin >> cities[i]; // Read city values: 0 = empty, 1 = beamer, 2 = catapult
    }

    // Calculate number of beamers and catapults required to cover all cities

    int beamers = finding_beamers(cities, k);
    int catapults = finding_catapults(cities, l);

    cout << "beamer:" << beamers << endl;
    cout << "cata:" << catapults << endl;


    // For testing: print the parsed values
    //cout << "n = " << n << ", k = " << k << ", l = " << l << "\n";
    //cout << "cities: ";
    //for (int c : cities) cout << c << " ";
    //cout << endl;


  return 0;
}
