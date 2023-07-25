#include <iostream>
#include <vector>

// Function to perform element-wise addition with broadcasting
std::vector<std::vector<int>> broadcasted_addition(const std::vector<std::vector<int>>& a, const std::vector<int>& b) {
    // Get the shapes of arrays 'a' and 'b'
    int rows_a = static_cast<int>(a.size());
    int cols_a = static_cast<int>(a[0].size());
    int rows_b = static_cast<int>(b.size());

    // Resulting broadcasted array with the same shape as 'a'
    std::vector<std::vector<int>> result(rows_a, std::vector<int>(cols_a));

    // Perform broadcasting and element-wise addition
    for (int i = 0; i < rows_a; ++i) {
        for (int j = 0; j < cols_a; ++j) {
            result[i][j] = a[i][j] + b[i % rows_b];
        }
    }

    return result;
}

int main() {
    // Example arrays
    std::vector<std::vector<int>> a = {{1}, {2}, {3}};
    std::vector<int> b = {10, 20, 30};

    // Perform broadcasting and addition
    std::vector<std::vector<int>> result = broadcasted_addition(a, b);

    // Print the result
    for (const auto& row : result) {
        for (int val : row) {
            std::cout << val << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
