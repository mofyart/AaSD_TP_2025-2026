// Дано множество целых чисел из
// [0..10^9] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
// 10% перцентиль
// медиана
// 90% перцентиль

#include <cassert>
#include <iostream>

template <typename T, typename Comparator = std::less<T>>
size_t Partition(T* array, size_t left, size_t right, Comparator comparator = Comparator()) {
    assert(array != nullptr);

    size_t middle = (left + right) / 2;

    size_t partitionPos = 0;

    if ((comparator(array[middle], array[left]) && comparator(array[right], array[middle])) || (comparator(array[left], array[middle]) && comparator(array[middle], array[right]))) {
        partitionPos = middle;
    } else if (comparator(array[left], array[middle]) && comparator(array[right], array[middle])) {
        partitionPos = comparator(array[left], array[right]) ? right : left;
    } else {
        partitionPos = comparator(array[left], array[right]) ? left : right;
    }

    int partition = array[partitionPos];
    std::swap(array[partitionPos], array[right]);

    size_t i = left;
    size_t j = left;


    while (true) {
        if (j >= right) {
            break;
        }

        if (comparator(array[j], partition)) {
            std::swap(array[i++], array[j++]);
        } else {
            ++j;
        }
    }

    std::swap(array[right], array[i]);

    return i;
}

template <typename T, typename Comparator = std::less<T>>
int FindKthElementStatistic(T* array, size_t posParcentile, size_t left, size_t right, Comparator comparator = Comparator()) {
    assert(array != nullptr);

    size_t posPartition = Partition(array, left, right);

    while (posParcentile != posPartition) {
        if (posParcentile < posPartition) {
            right = posPartition - 1;
            posPartition = Partition(array, left, right);
        } else {
            left = posPartition + 1;
            posPartition = Partition(array, posPartition + 1, right);
        }
    }

    return array[posPartition];
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int quantityElement = 0;
    std::cin >> quantityElement;

    assert(!std::cin.fail() && quantityElement >= 1);

    int* array = new int[quantityElement];

    for (size_t i = 0; i < quantityElement; ++i) {
        std::cin >> array[i];
        assert(!std::cin.fail());
    }

    size_t posTenParcentile = 0.1 * quantityElement;
    size_t posMiddleParcentile = 0.5 * quantityElement;
    size_t posNinetyParcentile = 0.9 * quantityElement;

    int tenParcentile = FindKthElementStatistic(array, posTenParcentile, 0, quantityElement - 1);
    int middleParcentile = FindKthElementStatistic(array, posMiddleParcentile, 0, quantityElement - 1);
    int ninetyParcentile = FindKthElementStatistic(array, posNinetyParcentile, 0, quantityElement - 1);

    std::cout << tenParcentile << '\n';
    std::cout << middleParcentile << '\n';
    std::cout << ninetyParcentile << '\n';

    delete[] array;

    return 0;
}
