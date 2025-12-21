#include <iostream>

using namespace std;

struct Point {
    int x, y;
};

int main() {
    int n;
    cin >> n;

    Point database[n];

    for (size_t i = 0; i < n; ++i) {
        cin >> database[i].x >> database[i].y;
    }

    int baseIndex = 0;

    for (size_t i = 1; i < n; ++i) {
        if (database[i].x < database[baseIndex].x || (database[i].x == database[baseIndex].x && database[i].y < database[baseIndex].y)) {
            baseIndex = i;
        }
    }

    Point base = database[baseIndex];

    if (baseIndex != 0) {
        Point tmp = database[0];
        database[0] = database[baseIndex];
        database[baseIndex] = tmp;
    }

    for (size_t i = 2; i < n; ++i) {
        Point cur = database[i];
        int j = i;

        while (j > 1) {
            long long cross = (database[j - 1].x - base.x) * (cur.y - base.y) - (cur.x - base.x) * (database[j - 1].y - base.y);

            if (cross > 0) {
                break;
            }

            database[j] = database[j - 1];
            --j;
        }

        database[j] = cur;
    }

    for (size_t i = 0; i < n; ++i) {
        cout << database[i].x << " " << database[i].y << '\n';
    }

    return 0;
}
