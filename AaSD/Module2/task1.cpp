#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include <string>

template <typename T, typename Hasher = std::hash<T>>
class HashTable {
    private:
        enum class CellVals {
            empty,
            deleted,
            withKey,
        };

        struct HashTableNode {
            T key;
            size_t hashKey;
            CellVals valCell = CellVals::empty;
        };

        size_t capacity;
        size_t realCells;
        size_t filledCells;

        std::vector<HashTableNode> dataBase;

        Hasher hasher;

        void ResizeHashTable();

    public:
        HashTable() : capacity(8), realCells(0), filledCells(0), dataBase(std::vector<T>(capacity)){}

        HashTable(Hasher hasherVal) : capacity(8), realCells(0), filledCells(0), dataBase(capacity), hasher(hasherVal){}

        bool CheckHasKey(const T& other, size_t& index) const;

        bool AddElement(const T& other);

        bool DeleteElement(const T& other);
};

template <typename T, typename Hasher>
void HashTable<T, Hasher>::ResizeHashTable() {
    double ratioRealFilled = static_cast<double>(realCells) / capacity;

    std::vector<HashTableNode> oldDatabase = std::move(dataBase);
    size_t oldCapacity = capacity;

    oldDatabase = dataBase;

    if (ratioRealFilled >= 0.25) {
        capacity *= 2;
    }

    filledCells = 0;

    dataBase.resize(capacity);

    for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldDatabase[i].valCell == CellVals::withKey) {
            size_t index = oldDatabase[i].hashKey % capacity;
            size_t countHasher = 0;

            while (dataBase[index].valCell == CellVals::withKey) {
                index = (index + countHasher + 1) % capacity;
                ++countHasher;
            }

            dataBase[index] = oldDatabase[i];

            ++filledCells;
        }
    }
}

template <typename T, typename Hasher>
bool HashTable<T, Hasher>::CheckHasKey(const T& other, size_t& index) const {
    index = hasher(other) % capacity;
    size_t countHasher = 0;

    while (dataBase[index].valCell == CellVals::deleted || dataBase[index].valCell == CellVals::withKey) {
        if (dataBase[index].valCell == CellVals::withKey && dataBase[index].key == other) {
            return true;
        }

        index = (index + countHasher + 1) % capacity;
        ++countHasher;
    }

    return false;
}

template <typename T, typename Hasher>
bool HashTable<T, Hasher>::AddElement(const T& other) {
    double ratioGeneralFilled = static_cast<double>(filledCells) / capacity;

    if (ratioGeneralFilled >= 0.75) {
        ResizeHashTable();
    }

    size_t hash = hasher(other);
    size_t index = hash % capacity;

    size_t countHasher = 0;

    bool firstDeletedCell = true;
    size_t firstDeletedIndex = 0;

    while (dataBase[index].valCell == CellVals::deleted || dataBase[index].valCell == CellVals::withKey) {
        if (dataBase[index].valCell == CellVals::withKey && dataBase[index].key == other) {
            return false;
        } else if (dataBase[index].valCell == CellVals::deleted && firstDeletedCell) {
            firstDeletedIndex = index;
            firstDeletedCell = false;
        }

        index = (index + countHasher + 1) % capacity;
        ++countHasher;
    }

    ++realCells;
    ++filledCells;

    if (!firstDeletedCell) {
        index = firstDeletedIndex;
    }

    dataBase[index] = {other, hash, CellVals::withKey};

    return true;
}

template <typename T, typename Hasher>
bool HashTable<T, Hasher>::DeleteElement(const T& other) {
    double ratioGeneralFilled = static_cast<double>(filledCells) / capacity;

    size_t index = 0;

    if (CheckHasKey(other, index)) {
        dataBase[index].valCell = CellVals::deleted;
        --realCells;

        return true;
    }

    return false;
}

class GornerHasher {
    private:
        size_t ratio;
    public:
        GornerHasher(size_t ratioVal) : ratio(ratioVal){}

        size_t operator()(const std::string& value) const {
            size_t hash = 0;

            for (const char& digit : value) {
                hash = hash * ratio + digit;
            }

            return hash;
        }
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    char val;

    HashTable<std::string, GornerHasher> hashTable(GornerHasher(6547));

    while (std::cin >> val) {
        std::string str;
        std::cin >> str;

        assert(!std::cin.fail());

        switch (val) {
            case '+':
                hashTable.AddElement(str) ? std::cout << "OK" << '\n' : std::cout << "FAIL" << '\n';
                break;
            case '-':
                hashTable.DeleteElement(str) ? std::cout << "OK" << '\n' : std::cout << "FAIL" << '\n';
                break;
            case '?':
                size_t index;
                hashTable.CheckHasKey(str, index) ? std::cout << "OK" << '\n' : std::cout << "FAIL" << '\n';
                break;
            default:
                std::cerr << "Такой команды нет" << '\n';
                break;
        }
    }

    return 0;
}
