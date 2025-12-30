#include <algorithm>
#include <cassert>
#include <iostream>

#include <cmath>

#include <vector>
#include <set>
#include <unordered_map>



long long HeuristicFunc(size_t start, size_t end, size_t dimensionTable) {
    return std::abs(int(start / dimensionTable) - int((end) / dimensionTable)) + std::abs(int(start % dimensionTable) - int((end) % dimensionTable));
}

class GameState {
    private:
        static size_t countSymbols;
        static size_t dimensionTable;

        static std::vector<char> completedTable;

        std::vector<char> currentTable;
        size_t emptyPos;

        std::vector<char> getCompleteTable() const;

        size_t getCountInversions() const;

        size_t findPosEmpty() const;

        long long findBeginHeuristicValue() const;
    public:
        GameState(const std::vector<char>& table) : currentTable(table), emptyPos(findPosEmpty()) {}

        bool IsComplete() const;

        bool CanMoveLeft() const;

        bool CanMoveRight() const;

        bool CanMoveUp() const;

        bool CanMoveDown() const;

        GameState MoveLeft() const;

        GameState MoveRight() const;

        GameState MoveUp() const;

        GameState MoveDown() const;

        bool operator<(const GameState& other) const;

        bool operator==(const GameState& other) const;

        friend std::string FindPathCompleted(GameState& startTable);

        friend class GameStateHasher;

        friend void FillStaticFields(size_t size);
};

size_t GameState::countSymbols;
size_t GameState::dimensionTable;
std::vector<char> GameState::completedTable;

std::vector<char> GameState::getCompleteTable() const {
    std::vector<char> completedTable(countSymbols);

    for (size_t i = 0; i < countSymbols - 1; ++i) {
        completedTable[i] = i + 1;
    }

    completedTable[countSymbols - 1] = 0;

    return completedTable;
}

size_t GameState::getCountInversions() const {
    size_t countInversions = 0;

    for (size_t i = 0; i < countSymbols - 1; ++i) {
        for (size_t j = i + 1; j < countSymbols; ++j) {
            if (currentTable[i] && currentTable[j] && currentTable[i] > currentTable[j]) {
                ++countInversions;
            }
        }
    }

    return countInversions;
}

size_t GameState::findPosEmpty() const {
    for (size_t i = 0; i < countSymbols; ++i) {
        if (!currentTable[i]) {
            return i;
        }
    }

    return 0;
}

long long GameState::findBeginHeuristicValue() const {
    long long value = 0;

    for (size_t i = 0; i < countSymbols; ++i) {
        size_t element = size_t(currentTable[i]);

        if (element) {
            value += HeuristicFunc(i, element - 1, dimensionTable);
        }
    }

    return value;
}

bool GameState::IsComplete() const {
    return currentTable == completedTable;
}

bool GameState::CanMoveLeft() const {
    return emptyPos % dimensionTable != 0;
}

bool GameState::CanMoveRight() const {
    return emptyPos % dimensionTable < dimensionTable - 1;
}

bool GameState::CanMoveUp() const {
    return emptyPos >= dimensionTable;
}

bool GameState::CanMoveDown() const {
    return emptyPos < dimensionTable * (dimensionTable - 1);
}

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());

    GameState newTable(*this);

    std::swap(newTable.currentTable[emptyPos - 1], newTable.currentTable[emptyPos]);
    --newTable.emptyPos;

    return newTable;
}

GameState GameState::MoveRight() const {
    assert(CanMoveRight());

    GameState newTable(*this);

    std::swap(newTable.currentTable[emptyPos + 1], newTable.currentTable[emptyPos]);
    ++newTable.emptyPos;

    return newTable;
}

GameState GameState::MoveUp() const {
    assert(CanMoveUp());

    GameState newTable(*this);

    std::swap(newTable.currentTable[emptyPos - dimensionTable], newTable.currentTable[emptyPos]);
    newTable.emptyPos -= dimensionTable;

    return newTable;
}

GameState GameState::MoveDown() const {
    assert(CanMoveDown());

    GameState newTable(*this);

    std::swap(newTable.currentTable[emptyPos + dimensionTable], newTable.currentTable[emptyPos]);
    newTable.emptyPos += dimensionTable;

    return newTable;
}

bool GameState::operator<(const GameState& other) const {
    return currentTable < other.currentTable;
}

bool GameState::operator==(const GameState& other) const {
    return currentTable == other.currentTable;
}

class GameStateHasher {
    public:
        size_t operator()(const GameState& other) const {
            size_t hash = 1469598103934665603ULL;
            const size_t prime = 1099511628211ULL;

            for (size_t i = 0; i < other.countSymbols; ++i) {
                hash ^= other.currentTable[i];
                hash *= prime;
            }

            return hash;
        }
};
std::string FindPathCompleted(GameState& startTable) {
    size_t countInversions = startTable.getCountInversions();
    size_t posEmpty = startTable.findPosEmpty();

    size_t numberLineZeroElement = posEmpty / startTable.dimensionTable + 1;

    if ((startTable.dimensionTable % 2 != 0 && countInversions % 2 != 0) || (startTable.dimensionTable % 2 == 0 && (numberLineZeroElement + countInversions) % 2 != 0)) {
        return "-1";
    }

    std::set<std::pair<long long, GameState>> setGameState;
    std::unordered_map<GameState, char, GameStateHasher> usedTable;

    long long heuristicValue = startTable.findBeginHeuristicValue();

    usedTable[startTable] = 'S';
    setGameState.insert({heuristicValue, startTable});

    while (!setGameState.empty()) {
        GameState frontTable = (*setGameState.begin()).second;
        long long weight = (*setGameState.begin()).first;

        if (frontTable.IsComplete()) {
            break;
        }

        setGameState.erase({weight, frontTable});

        if (frontTable.CanMoveDown()) {
            GameState newTable = frontTable.MoveDown();

            if (!usedTable.count(newTable)) {
                size_t posSwapElement = frontTable.emptyPos + GameState::dimensionTable;
                size_t element = size_t(frontTable.currentTable[posSwapElement]);

                long long newWeight = weight - HeuristicFunc(posSwapElement, element - 1, GameState::dimensionTable);

                newWeight += HeuristicFunc(frontTable.emptyPos, element - 1, GameState::dimensionTable);

                setGameState.insert({newWeight, newTable});

                usedTable[newTable] = 'D';
            }
        }

        if (frontTable.CanMoveUp()) {
            GameState newTable = frontTable.MoveUp();

            if (!usedTable.count(newTable)) {
                size_t posSwapElement = frontTable.emptyPos - GameState::dimensionTable;
                size_t element = size_t(frontTable.currentTable[posSwapElement]);

                long long newWeight = weight - HeuristicFunc(posSwapElement, element - 1, GameState::dimensionTable);

                newWeight += HeuristicFunc(frontTable.emptyPos, element - 1, GameState::dimensionTable);

                setGameState.insert({newWeight, newTable});

                usedTable[newTable] = 'U';
            }
        }

        if (frontTable.CanMoveLeft()) {
            GameState newTable = frontTable.MoveLeft();

            if (!usedTable.count(newTable)) {
                size_t posSwapElement = frontTable.emptyPos - 1;
                size_t element = size_t(frontTable.currentTable[posSwapElement]);

                long long newWeight = weight - HeuristicFunc(posSwapElement, element - 1, GameState::dimensionTable);

                newWeight += HeuristicFunc(frontTable.emptyPos, element - 1, GameState::dimensionTable);

                setGameState.insert({newWeight, newTable});

                usedTable[newTable] = 'L';
            }
        }

        if (frontTable.CanMoveRight()) {
            GameState newTable = frontTable.MoveRight();

            if (!usedTable.count(newTable)) {
                size_t posSwapElement = frontTable.emptyPos + 1;
                size_t element = size_t(frontTable.currentTable[posSwapElement]);

                long long newWeight = weight - HeuristicFunc(posSwapElement, element - 1, GameState::dimensionTable);

                newWeight += HeuristicFunc(frontTable.emptyPos, element - 1, GameState::dimensionTable);

                setGameState.insert({newWeight, newTable});

                usedTable[newTable] = 'R';
            }
        }
    }

    std::string path;

    GameState curTable = (*setGameState.begin()).second;

    while (usedTable[curTable] != 'S') {
        char move = usedTable[curTable];

        switch (move) {
            case 'L':
                curTable = curTable.MoveRight();
                path += 'R';
                break;
            case 'R':
                curTable = curTable.MoveLeft();
                path += 'L';
                break;
            case 'D':
                curTable = curTable.MoveUp();
                path += 'U';
                break;
            case 'U':
                curTable = curTable.MoveDown();
                path += 'D';
                break;
        }
    }

    std::reverse(path.begin(), path.end());

    return path;
}

void FillStaticFields(size_t size) {
    for (size_t i = 0; i < size - 1; ++i) {
        GameState::completedTable.push_back(i + 1);
    }

    GameState::completedTable.push_back(0);

    GameState::countSymbols = size;
    GameState::dimensionTable = std::sqrt(size);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int digit;

    std::vector<char> array;

    while (std::cin >> digit) {
        assert(!std::cin.fail());

        array.push_back(static_cast<char>(digit));
    }

    FillStaticFields(array.size());

    GameState table(array);

    std::string resultPath = FindPathCompleted(table);

    if (resultPath == "-1") {
        std::cout << resultPath << '\n';

        return 0;
    }

    if (resultPath.empty()) {
        std::cout << 0 << '\n';

        return 0;
    }

    std::cout << resultPath.size() << '\n';
    for (const auto& digit : resultPath) {
        std::cout << digit;
    }

    return 0;
}
