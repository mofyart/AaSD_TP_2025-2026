// Дан массив строк. Количество строк не больше 100000.
// Отсортировать массив методом поразрядной сортировки MSD по символам.
// Размер алфавита - 256 символов. Последний символ строки = ‘0’.

#include <iostream>
#include <string>
#include <assert.h>

template <typename T>
class MyVector {
    int capacity;
    int currentSize;
    T** myVector;

    void ResizeVector();
public:
    MyVector() : capacity(1), currentSize(0), myVector(new T*[capacity]){}

    ~MyVector();

    MyVector(const MyVector& other);

    MyVector& operator=(const MyVector& other);

    void AddElement(T&& element);

    int GetCurrentSize();

    void PrintVector();

    template<typename U>
    friend void MSD(MyVector<U>* myVector, U** buffer, std::size_t digitStr, std::size_t left, std::size_t right);

    template<typename U>
    friend void CalculateSort(MyVector<U>* myVector, int* counterDigits, int* borderDigits, U** buffer, std::size_t digitStr, std::size_t left, std::size_t right);
};

template <typename T>
MyVector<T>::~MyVector() {
    for (std::size_t i = 0; i < currentSize; ++i) {
        delete myVector[i];
    }

    delete[] myVector;
}

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& other) {
    T** newVector = new T*[other.capacity];

    for (std::size_t i = 0; i < other.currentSize; ++i) {
        newVector[i] = new T(*(other.myVector[i]));
    }

    myVector = newVector;
    capacity = other.capacity;
    currentSize = other.currentSize;
}

template <typename T>
void MyVector<T>::ResizeVector() {
    T** newVector = new T*[capacity * 2];
    T** oldVector = myVector;

    std::copy(myVector, myVector + currentSize, newVector);

    myVector = newVector;
    capacity *= 2;

    delete[] oldVector;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other) {
    if (this != &other) {
        for (std::size_t i = 0; i < currentSize; ++i) {
            delete myVector[i];
        }

        delete[] myVector;

        T** newVector = new T*[other.capacity];
        for (std::size_t i = 0; i < other.currentSize; ++i) {
            newVector[i] = new T(*other.myVector[i]);
        }

        myVector = newVector;
        capacity = other.capacity;
        currentSize = other.currentSize;
    }

    return *this;
}

template <typename T>
void MyVector<T>::AddElement(T&& element) {
    if (currentSize == capacity) {
        ResizeVector();
    }

    myVector[currentSize++] = new T(std::move(element));
}

template <typename T>
int MyVector<T>::GetCurrentSize() {
    return currentSize;
}

template <typename T>
void MyVector<T>::PrintVector() {
    for (std::size_t i = 0; i < currentSize; ++i) {
        std::cout << *(myVector[i]) << '\n';
    }
}

inline unsigned int GetChar(const std::string& str, int digitStr) {
    if (digitStr > str.size()) {
        return 0;
    }
    return str[digitStr];
}

template<typename T>
void CalculateSort(MyVector<T>* myVector, int* counterDigits, int* borderDigits, T** buffer, std::size_t digitStr, std::size_t left, std::size_t right) {
    for (std::size_t i = left; i < right; ++i) {
        unsigned int symbolCode = GetChar(*(myVector->myVector[i]), digitStr);

        ++counterDigits[symbolCode];
        ++borderDigits[symbolCode];
    }

    for (std::size_t i = 1; i < 256; ++i) {
        counterDigits[i] += counterDigits[i - 1];
    }

    for (std::size_t i = right; i > left; --i) {
        int symbolCode = GetChar(*(myVector->myVector[i - 1]), digitStr);
        buffer[left + (--counterDigits[symbolCode])] = myVector->myVector[i - 1];
    }

    for (std::size_t i = left; i < right; ++i) {
        myVector->myVector[i] = buffer[i];
    }
}

template <typename T>
void MSD(MyVector<T>* myVector, T** buffer, std::size_t digitStr, std::size_t left, std::size_t right) {
    if (right - left <= 1) {
        return;
    }

    int counterDigits[256]{0};
    int borderDigits[256]{0};

    CalculateSort(myVector, counterDigits, borderDigits, buffer, digitStr, left, right);

    for (std::size_t i = 0; i < 256; ++i) {
        if (borderDigits[i] != 0) {
            if (i != 0) {
                MSD(myVector, buffer, digitStr + 1, left, left + borderDigits[i]);
            }

            left += borderDigits[i];
        }
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::string str;

    MyVector<std::string> vectorStrs;

    while (std::cin >> str) {
        assert(!str.empty());
        vectorStrs.AddElement(std::move(str));
    }

    int sizeVector = vectorStrs.GetCurrentSize();
    assert(sizeVector <= 100000);

    std::string** buffer = new std::string*[sizeVector];

    MSD(&vectorStrs, buffer, 0, 0, sizeVector);

    vectorStrs.PrintVector();

    for (std::size_t i = 0; i < sizeVector; ++i) {
        buffer[i] = nullptr;
    }

    delete[] buffer;

    return 0;
}
