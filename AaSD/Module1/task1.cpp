#include <iostream>
#include <cassert>

unsigned int CalcualteBitsOne(unsigned int number) {
    unsigned int countOne = 0;

    while (number > 0) {
        if (number & 1) {
            ++countOne;
        }

        number >>= 2;
    }

    return countOne;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[]) {
    unsigned int number = 0;
    std::cin >> number;

    assert(!std::cin.fail());

    unsigned int result = CalcualteBitsOne(number);

    std::cout << result << '\n';
}
