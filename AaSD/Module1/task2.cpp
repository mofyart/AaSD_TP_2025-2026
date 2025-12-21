// 2_2. Дан массив целых чисел А[0..n-1]. Известно, что на интервале [0, m] значения массива строго
// возрастают, а на интервале [m, n-1] строго убывают. Найти m за O(log m).
// Требования:  Время работы O(log m). Внимание! В этой задаче сначала нужно определить диапазон для
// бинарного поиска размером порядка m с помощью экспоненциального поиска,
//  а потом уже в нем делать бинарный поиск.

#include <iostream>
#include <cassert>

void FindBoundaryMonotony(int* array, size_t quantityElements, size_t& leftBorder, size_t& rightBorder) {
    assert(quantityElements >= 2 && array != nullptr);

    while (rightBorder < quantityElements && array[rightBorder - 1] < array[rightBorder]) {
        leftBorder = rightBorder;
        rightBorder *= 2;
    }

    rightBorder = std::min(quantityElements - 1, rightBorder);
}

int ExponentialBinarysearch(int* array, size_t quantityElements) {
    assert(quantityElements >= 2 && array != nullptr);

    size_t middlePosition = 0;
    size_t leftBorder = 0;
    size_t rightBorder = 1;

    FindBoundaryMonotony(array, quantityElements, leftBorder, rightBorder);

    while (rightBorder > leftBorder) {
        middlePosition = (rightBorder + leftBorder) / 2;
        int valMiddle = array[middlePosition];

        if (valMiddle < array[middlePosition + 1]) {
            leftBorder = middlePosition + 1;
        } else {
            rightBorder = middlePosition;
        }
    }

    return leftBorder;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
    size_t quantityElements = 0;
    std::cin >> quantityElements;
    assert(!std::cin.fail() && quantityElements >= 2);

    int* array = new int[quantityElements];
    assert(array != nullptr);

    for (size_t i = 0; i < quantityElements; ++i) {
        std::cin >> array[i];
        assert(!std::cin.fail());
    }

    int result = ExponentialBinarysearch(array, quantityElements);
    std::cout << result << '\n';

    delete[] array;
}
