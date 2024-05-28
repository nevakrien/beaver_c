#include <vector>
#include <algorithm>
#include <iostream>

extern "C" {
    void getTopk(int* arr, int n,int k, int* result) {
        try {
            // Create a vector of pairs (value, index)
            std::vector<std::pair<int, int>> vec;
            for (int i = 0; i < n; ++i) {
                vec.push_back(std::make_pair(arr[i], i));
            }

            // Make a heap based on the value part of the pair
            std::make_heap(vec.begin(), vec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                return a.first < b.first; // compare the first element (value) to make max-heap
            });

            //int k = n / 2; // Top 50%
            for (int i = 0; i < k; ++i) {
                // Pop the maximum element from the heap
                std::pop_heap(vec.begin(), vec.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                    return a.first < b.first; // compare the first element (value) to maintain max-heap
                });
                vec.pop_back();
            }

            // Copy the top 50% element indices to the result array
            for (long unsigned int i = 0; i < vec.size(); ++i) {
                result[i] = vec[i].second; // store the second element (index)
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
            printf("C++ exception\n");
            exit(1);
        }
    }
}
