#include <iostream>
using namespace std;

int main() {
    int n = 0;
    cin >> n;

    int nums[n];

    for (size_t i = 0; i < n; ++i) {
        cin >> nums[i];
    }

    int left = 0;
    int right = n - 1;

    while (left < right) {
        int middle = (left + right) / 2;

        if (nums[middle] > nums[right]) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    cout << left << '\n';
    return 0;
}
