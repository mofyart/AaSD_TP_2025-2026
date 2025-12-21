// Реализовать дек с динамическим зацикленным буфером (на основе динамического массива).
// Требования: Дек должен быть реализован в виде класса.


#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>

class Deque {
    private:
        int capacity;
        int currentSize;
        int* database;
        int head;
        int tail;

        void resizeDB();
    public:
        Deque() : capacity(1), currentSize(0), database(new int[capacity]{}), head(0), tail(0){}

        ~Deque();

        Deque(const Deque& other);

        Deque& operator=(const Deque& other);

        void pushFront(int val);

        void pushBack(int val);

        int popFront();

        int popBack();
};

Deque::~Deque() {
    delete[] database;
}

Deque::Deque(const Deque& other) {
    assert(other.capacity > 0);

    int* newDatabase = new int[other.capacity];
    std::copy(other.database, other.database + other.capacity, newDatabase);
    database = newDatabase;

    capacity = other.capacity;
    currentSize = other.currentSize;
    head = other.head;
    tail = other.tail;
}

Deque& Deque::operator=(const Deque& other) {
    assert(other.capacity > 0);

    if (this != &other) {
        int* newDatabase = new int[other.capacity];
        int* oldDatabase = database;

        std::copy(other.database, other.database + other.capacity, newDatabase);
        database = newDatabase;

        capacity = other.capacity;
        currentSize = other.currentSize;
        head = other.head;
        tail = other.tail;

        delete[] oldDatabase;
    }

    return *this;
}

void Deque::resizeDB() {
    int* newDatabase = new int[capacity * 2];
    int* oldDatabase = database;

    for (int i = 0; i < capacity; ++i) {
        newDatabase[i] = database[(head + i) % capacity];
    }

    head = 0;
    tail = currentSize;

    database = newDatabase;
    capacity *= 2;

    delete[] oldDatabase;
}

void Deque::pushFront(int val) {
    capacity == currentSize ? resizeDB() : (void)0;

    head = (head - 1 + capacity) % capacity;
    database[head] = val;
    ++currentSize;
}

void Deque::pushBack(int val) {
    capacity == currentSize ? resizeDB() : (void)0;

    database[tail] = val;
    tail = (tail + 1) % capacity;
    ++currentSize;
}

int Deque::popFront() {
    if (currentSize == 0) {
        return -1;
    }

    int result = database[head];
    database[head] = 0;
    head = (head + 1) % capacity;
    --currentSize;

    return result;
}

int Deque::popBack() {
    if (currentSize == 0) {
        return -1;
    }

    tail = (tail - 1 + capacity) % capacity;
    int result = database[tail];
    database[tail] = 0;
    --currentSize;

    return result;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int quantityOperation = 0;
    std::cin >> quantityOperation;
    assert(!std::cin.fail());

    Deque deq;

    for (int i = 0; i < quantityOperation; ++i) {
        int numberComand = 0;
        int value = 0;

        std::cin >> numberComand >> value;
        assert(!std::cin.fail() && numberComand > 0 && numberComand < 5);

        switch (numberComand) {
            case 1:
                deq.pushFront(value);
                break;
            case 2: {
                int topElement = deq.popFront();

                if (topElement != value) {
                    std::cout << "NO" << '\n';
                    return 0;
                }
                break;
            }
            case 3:
                deq.pushBack(value);
                break;
            case 4: {
                int backElement = deq.popBack();

                if (backElement != value) {
                    std::cout << "NO" << '\n';
                    return 0;
                }
                break;
            }
            default:
                assert(numberComand > 0 && numberComand < 5);
                break;
        }
    }

    std::cout << "YES" << '\n';
}

  // for (int i = 0; i < minHeap.GetCurrentSizeDataBase(); ++i) {
    //     std::cout << minHeap.database[i].preority << ' ' << minHeap.database[i].timeWork << ' ';
    // }

    // std::cout << minHeap.GetCurrentSizeDataBase() << ' ';

    // minHeap.Add({1, 0, 4});
    // minHeap.ShiftUp(minHeap.GetCurrentSizeDataBase() - 1);

    // std::cout << std::endl;

    // for (int i = 0; i < minHeap.GetCurrentSizeDataBase(); ++i) {
    //     std::cout << minHeap.database[i].preority << ' ' << minHeap.database[i].timeWork << ' ';
    // }

    // std::cout << minHeap.GetCurrentSizeDataBase() << ' ';

    // std::cout << std::endl;

    // Process check = minHeap.Extract();

    // for (int i = 0; i < minHeap.GetCurrentSizeDataBase(); ++i) {
    //     std::cout << minHeap.database[i].preority << ' ' << minHeap.database[i].timeWork << ' ';
    // }

    // std::cout << minHeap.GetCurrentSizeDataBase() << '\n';

    //     std::cout << check.timeWork << ' ' << check.preority << '\n';
    // if (check.RunProcess()) {
    //     std::cout << check.timeWork << ' ' << check.preority << '\n';
    //     minHeap.Add(check);
    //     minHeap.ShiftUp(minHeap.GetCurrentSizeDataBase() - 1);
    // }

    // for (int i = 0; i < minHeap.GetCurrentSizeDataBase(); ++i) {
    //     std::cout << minHeap.database[i].preority << ' ' << minHeap.database[i].timeWork << ' ' << minHeap.database[i].timeEnd << ' ';
    // }

    // std::cout << std::endl;

    // std::cout << minHeap.GetCurrentSizeDataBase() << '\n';
