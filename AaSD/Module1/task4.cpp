// 4.3 Планировщик процессов
// В операционной системе Technux есть планировщик процессов.
// Каждый процесс характеризуется:
// приоритетом P
// временем, которое он уже отработал t
// временем, которое необходимо для завершения работы процесса T
// Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
// выполняет его время P и кладет обратно в очередь процессов.
// Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
// Требуется посчитать кол-во переключений процессора.
// Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
// Формат выходных данных: Кол-во переключений процессора.


#include <cassert>
#include <iostream>
#include <functional>

template <typename T, typename Comparator = std::less<T>>
class MyVector {
private:
    int capacity;
    int currentSize;
    T* database;
    Comparator comparator;

    void ResizeDatabase();

public:
    MyVector() : capacity(1), currentSize(0), database(new T[capacity]) {}

    MyVector(Comparator comparatorVal) : capacity(1), currentSize(0), database(new T[capacity]), comparator(comparatorVal) {}

    ~MyVector();

    MyVector(const MyVector& other);

    MyVector& operator=(const MyVector& other);

    void Add(T val);

    T Extract();

    void ShiftDown(size_t id);

    void ShiftUp(size_t id);

    int CountSwitches();

    void Heapify();

    int GetCurrentSizeDataBase();
};

template <typename T, typename Comparator>
MyVector<T, Comparator>::~MyVector() {
    delete[] database;
}

template <typename T, typename Comparator>
MyVector<T, Comparator>::MyVector(const MyVector& other) {
    T* newDatabase = new T[other.capacity];

    std::copy(other.database, other.database + other.capacity, newDatabase);

    database = newDatabase;
    capacity = other.capacity;
    currentSize = other.currentSize;
    comparator = other.comparator;
}

template <typename T, typename Comparator>
MyVector<T, Comparator>& MyVector<T, Comparator>::operator=(const MyVector& other) {
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
void MyVector<T, Comparator>::ResizeDatabase() {
    T* newDatabase = new T[capacity * 2];
    T* oldDatabase = database;

    std::copy(database, database + currentSize, newDatabase);

    database = newDatabase;
    capacity *= 2;

    delete[] oldDatabase;
}

template <typename T, typename Comparator>
void MyVector<T, Comparator>::Add(T val) {
    if (capacity == currentSize) {
        ResizeDatabase();
    }

    database[currentSize++] = val;
}

template <typename T, typename Comparator>
void MyVector<T, Comparator>::Heapify() {
    for (int i = currentSize / 2 - 1; i >= 0; --i) {
        ShiftDown(i);
    }
}

template <typename T, typename Comparator>
void MyVector<T, Comparator>::ShiftDown(size_t id) {
    if (2 * id + 1 >= currentSize) {
        return;
    }

    if (2 * id + 2 >= currentSize) {
        if (comparator(database[2 * id + 1], database[id])) {
            std::swap(database[2 * id + 1], database[id]);
            ShiftDown(2 * id + 1);
        }

        return;
    }

    if (comparator(database[2 * id + 1], database[id]) && comparator(database[2 * id + 1], database[2 * id + 2])) {
        std::swap(database[2 * id + 1], database[id]);
        ShiftDown(2 * id + 1);
    } else if (comparator(database[2 * id + 2], database[id]) && comparator(database[2 * id + 2], database[2 * id + 1])) {
        std::swap(database[2 * id + 2], database[id]);
        ShiftDown(2 * id + 2);
    }
}

template <typename T, typename Comparator>
void MyVector<T, Comparator>::ShiftUp(size_t id) {
    if (id == 0) {
        return;
    }

    if (comparator(database[id], database[(id - 1) / 2])) {
        std::swap(database[id], database[(id - 1) / 2]);
        ShiftUp((id - 1) / 2);
    }
}

template <typename T, typename Comparator>
T MyVector<T, Comparator>::Extract() {
    T checkingElement = database[0];

    std::swap(database[--currentSize], database[0]);
    ShiftDown(0);

    return checkingElement;
}

template <typename T, typename Comparator>
int MyVector<T, Comparator>::GetCurrentSizeDataBase() {
    return currentSize;
}


struct Process {
    int priority;
    int timeWork;
    int timeEnd;

    Process() : priority(0), timeWork(0), timeEnd(0) {}

    Process(int preorityVal, int timeWorkVal, int timeEndVal) : priority(preorityVal), timeWork(timeWorkVal), timeEnd(timeEndVal) {}

    int FindNewPreority() const;

    bool RunProcess();
};

int Process::FindNewPreority() const {
    return priority * (timeWork + 1);
}

bool Process::RunProcess() {
    timeWork += priority;

    return timeEnd > timeWork;
}

template <typename Comparator>
int FindQuantityOperations(MyVector<Process, Comparator>& minHeap, int quantityProcesses) {
    int result = 0;

    while (minHeap.GetCurrentSizeDataBase() > 0) {
        Process checkingProcess = minHeap.Extract();

        if (checkingProcess.RunProcess()) {
            ++result;

            minHeap.Add(checkingProcess);
            minHeap.ShiftUp(minHeap.GetCurrentSizeDataBase() - 1);
        }
    }

    return result + quantityProcesses;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int quantityProceses = 0;
    std::cin >> quantityProceses;

    assert(!std::cin.fail() && quantityProceses >= 0);

    auto comparatorPlanner = [](const Process& other1, const Process& other2) {return other1.FindNewPreority() < other2.FindNewPreority();};

    MyVector<Process, decltype(comparatorPlanner)> minHeap(comparatorPlanner);

    for (int i = 0; i < quantityProceses; ++i) {
        int priority = 0;
        int timeEnd = 0;
        int timeWork = 0;

        std::cin >> priority >> timeEnd;

        assert(!std::cin.fail() && priority > 0 && timeEnd >= 0);

        minHeap.Add({priority, timeWork, timeEnd});
    }

    minHeap.Heapify();

    int result = FindQuantityOperations<decltype(comparatorPlanner)>(minHeap, quantityProceses);

    std::cout << result << '\n';

    return 0;
}
