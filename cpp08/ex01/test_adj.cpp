#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

struct SpanDiff {
    unsigned int operator()(int a, int b) const {
        return static_cast<unsigned int>(a) - static_cast<unsigned int>(b);
    }
};

int main() {
    std::vector<int> nums;
    nums.push_back(-10);
    nums.push_back(5);
    nums.push_back(20);
    
    std::vector<unsigned int> diffs(nums.size());
    std::adjacent_difference(nums.begin(), nums.end(), diffs.begin(), SpanDiff());
    
    std::cout << "diffs size: " << diffs.size() << std::endl;
    for (size_t i = 1; i < diffs.size(); ++i) {
        std::cout << "diff " << i << ": " << diffs[i] << std::endl;
    }
    
    unsigned int min_span = *std::min_element(diffs.begin() + 1, diffs.end());
    std::cout << "min span: " << min_span << std::endl;
    return 0;
}
