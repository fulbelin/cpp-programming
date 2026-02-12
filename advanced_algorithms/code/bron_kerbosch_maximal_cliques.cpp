#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// Type alias for easier reference to an adjacency matrix
using AdjacencyMatrix = vector<vector<int>>;

// Bron–Kerbosch recursive algorithm to find all maximal cliques in a graph
void findCliques(
    set<int> currentClique, // Nodes currently in the growing clique
    set<int> possibleNodes,  // Nodes that can still be added to the clique
    set<int> excludedNodes,   // Nodes that were already considered and excluded
    const AdjacencyMatrix& graph, // Graph as an adjacency matrix
    vector<set<int>>& allCliques  // Output: list of all maximal cliques found
) {
    // If no possible or excluded nodes left, currentClique is maximal
    if (possibleNodes.empty() && excludedNodes.empty()) {
        allCliques.push_back(currentClique);  // Save the found maximal clique
        return;
    }

    // Work on a copy of possibleNodes to safely modify the original set
    set<int> candidateNodes = possibleNodes;

    // Try adding each candidate node to the current clique
    for (int candidateNode : candidateNodes) {

        // Find neighbors of the current node (connected nodes)
        set<int> neighborNodes;
        for (size_t neighborIdx = 0; neighborIdx < graph.size(); ++neighborIdx) {
            if (graph[candidateNode][neighborIdx] == 1) {
                neighborNodes.insert(neighborIdx);
            }
        }

        // Prepare the sets for the recursive call
        set<int> updatedClique = currentClique;
        updatedClique.insert(candidateNode);

        // Limit future candidates to only neighbors of the current node
        set<int> newPossibleNodes;
        set_intersection(possibleNodes.begin(), possibleNodes.end(),
            neighborNodes.begin(), neighborNodes.end(),
            inserter(newPossibleNodes, newPossibleNodes.begin()));

        // Limit excluded set similarly to neighbors only
        set<int> newExcludedNodes;
        set_intersection(excludedNodes.begin(), excludedNodes.end(),
            neighborNodes.begin(), neighborNodes.end(),
            inserter(newExcludedNodes, newExcludedNodes.begin()));

        // Recursive call with updated sets
        findCliques(updatedClique, newPossibleNodes, newExcludedNodes, graph, allCliques);

        // Remove node from possible and add to excluded to avoid rechecking
        possibleNodes.erase(candidateNode);
        excludedNodes.insert(candidateNode);
    }
}

// Read an adjacency matrix from standard input
AdjacencyMatrix readAdjacencyMatrix() {
    AdjacencyMatrix matrix;
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        vector<int> row;
        int val;
        while (ss >> val) {
            row.push_back(val);
        }
        if (!row.empty())
            matrix.push_back(row); // Only add non-empty rows
    }
    return matrix;
}

// Print all found cliques in a readable format
void printCliques(const vector<set<int>>& cliques) {
    for (const auto& clique : cliques) {
        cout << "{";
        for (auto it = clique.begin(); it != clique.end(); ++it) {
            if (it != clique.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}" << endl;
    }
}

int main() {
    AdjacencyMatrix graph = readAdjacencyMatrix();

    // Create a set of all node indices from 0 to n-1
    set<int> nodeIndices;
    for (int node = 0; node < graph.size(); ++node) {
        nodeIndices.insert(node);
    }

    // Initialize data structures for the Bron–Kerbosch algorithm
    set<int> currentClique;  // Start with an empty clique
    set<int> excludedNodes;  // Start with no excluded nodes
    vector<set<int>> foundCliques;  // Store all found cliques

    //find all maximal cliques
    findCliques(currentClique, nodeIndices, excludedNodes, graph, foundCliques);

    printCliques(foundCliques);

    return 0;
}
