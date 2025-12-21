// В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода
// и ухода покупателей (два целых числа). Каждому покупателю необходимо показать минимум 2 рекламы.
// Рекламу можно транслировать только в целочисленные моменты времени.
// Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
// В каждый момент времени может показываться только одна реклама.
// Считается, что реклама показывается мгновенно.
// Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
// Требуется определить минимальное число показов рекламы.


#include <cassert>
#include <functional>
#include <iostream>
#include <algorithm>

struct Buyer {
    int in;
    int out;

    Buyer() : in(0), out(0) {}

    Buyer(int inVal, int outVal) : in(inVal), out(outVal) {}
};

template <typename T, typename Comparator = std::less<T>>
class ListBuyers {
    private:
        int capacity;
        int currentSize;
        T* database;
        Comparator comparator;

        void ResizeDatabase();
    public:
        ListBuyers() : capacity(1), currentSize(0), database(new T[capacity]){}

        ListBuyers(Comparator comparatorVal) : capacity(1), currentSize(0), database(new T[capacity]), comparator(comparatorVal){}

        ~ListBuyers();

        ListBuyers(const ListBuyers& other);

        ListBuyers& operator=(const ListBuyers& other);

        void AddElement(T element);

        void Sort(size_t l, size_t r);

        T* MergeSort(size_t l, size_t r);

        template <typename F, typename Cmp>
        friend int FindMinQuantityAdvertisement(const ListBuyers<F, Cmp>& list);
};

template <typename T, typename Comparator>
ListBuyers<T, Comparator>::~ListBuyers() {
    delete[] database;
}

template <typename T, typename Comparator>
ListBuyers<T, Comparator>::ListBuyers(const ListBuyers& other) {
    T* newDatabase = new T[other.capacity];

    std::copy(other.database, other.database + other.capacity, newDatabase);

    database = newDatabase;
    capacity = other.capacity;
    currentSize = other.currentSize;
    comparator = other.comparator;
}

template <typename T, typename Comparator>
ListBuyers<T, Comparator>& ListBuyers<T, Comparator>::operator=(const ListBuyers& other) {
    if (this != &other) {
        T* newDatabase = new T[other.capacity];
        T* oldDatabase = database;

        std::copy(other.database, other.database + other.capacity, newDatabase);

        database = newDatabase;
        capacity = other.capacity;
        currentSize = other.currentSize;
        comparator = other.comparator;

        delete[] oldDatabase;
    }

    return *this;
}

template <typename T, typename Comparator>
void ListBuyers<T, Comparator>::ResizeDatabase() {
    T* newDatabase = new T[capacity * 2];
    T* oldDatabase = database;

    std::copy(oldDatabase, oldDatabase + currentSize, newDatabase);
    database = newDatabase;
    capacity *= 2;

    delete[] oldDatabase;
}

template <typename T, typename Comparator>
void ListBuyers<T, Comparator>::AddElement(T element) {
    if (currentSize == capacity) {
        ResizeDatabase();
    }

    database[currentSize] = element;

    ++currentSize;
}

template <typename T, typename Comparator>
void ListBuyers<T, Comparator>::Sort(size_t l, size_t r) {
    T* newDatabase = MergeSort(l, r);
    T* oldDatabase = database;

    database = newDatabase;

    delete[] oldDatabase;
}

template <typename T, typename Comparator>
T* ListBuyers<T, Comparator>::MergeSort(size_t l, size_t r) {
    if (l == r) {
        return new T[1]{database[l]};
    }

    size_t middle = (l + r) / 2;

    T* leftList = MergeSort(l, middle);
    T* rightList = MergeSort(middle + 1, r);

    T* newDatabase = new T[r - l + 1];
    size_t mainIter;
    size_t leftIter = 0;
    size_t rightIter = 0;

    for (mainIter = 0; (leftIter < middle - l + 1) && (rightIter < r - middle); ++mainIter) {
        newDatabase[mainIter] = comparator(leftList[leftIter], rightList[rightIter]) ? leftList[leftIter++] : rightList[rightIter++];
    }

    for (size_t i = leftIter; i < middle - l + 1; ++i) {
        newDatabase[mainIter++] = leftList[i];
    }

    for (size_t i = rightIter; i < r - middle; ++i) {
        newDatabase[mainIter++] = rightList[i];
    }

    delete[] leftList;
    delete[] rightList;

    return newDatabase;
}

template <typename T, typename Comparator>
int FindMinQuantityAdvertisement(const ListBuyers<T, Comparator>& list) {
    int countAdvertisements = 2;
    int firstLastEnd = list.database[0].out;
    int secondLastEnd = list.database[0].out - 1;

    for (int i = 1; i < list.currentSize; ++i) {
        bool seeFirstLast = (list.database[i].in <= firstLastEnd);
        bool seeSecondLast = (list.database[i].in <= secondLastEnd);

        if (seeFirstLast && seeSecondLast) {
            continue;
        } else if (!seeFirstLast && !seeSecondLast) {
            countAdvertisements += 2;
            firstLastEnd = list.database[i].out;
            secondLastEnd = list.database[i].out - 1;
        } else  {
            ++countAdvertisements;

            int newFirstLastEnd = list.database[i].out;
            int newSecondLastEnd = firstLastEnd;

            if (newFirstLastEnd == newSecondLastEnd) {
                newSecondLastEnd = newFirstLastEnd - 1;
            }

            firstLastEnd = newFirstLastEnd;
            secondLastEnd = newSecondLastEnd;
        }
    }

    return countAdvertisements;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int quantityBuyer = 0;
    std::cin >> quantityBuyer;

    if (quantityBuyer == 0) {
        std::cout << quantityBuyer << '\n';
        return 0;
    }

    auto comporatorBuyer = [](const Buyer& firstBuyer, const Buyer& secondBuyer){return firstBuyer.out < secondBuyer.out;};
    ListBuyers<Buyer, decltype(comporatorBuyer)>listBuyers(comporatorBuyer);

    for (int i = 0; i < quantityBuyer; ++i) {
        int in = 0;
        int out = 0;

        std::cin >> in >> out;
        assert(!std::cin.fail() && in != out);

        listBuyers.AddElement({in, out});
    }

    listBuyers.Sort(0, quantityBuyer - 1);

    int result = FindMinQuantityAdvertisement(listBuyers);

    std::cout << result << '\n';

    return 0;
}
