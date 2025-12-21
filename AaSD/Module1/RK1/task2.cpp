#include <iostream>

using namespace std;

class Stack {
    int capacity;
    int currentSize;
    char* database;

    void ResizeDatabase() {
        char* newDatabase = new char[capacity * 2];
        char* oldDatabase = database;

        std::copy(oldDatabase, oldDatabase + currentSize, newDatabase);
        database = newDatabase;
        capacity *= 2;

        delete[] oldDatabase;
    }

    public:
        Stack() : capacity(1), currentSize(0), database(new char[capacity]) {}

        void Add(char ele) {
            if (capacity == currentSize) {
                ResizeDatabase();
            }

            database[currentSize++] = ele;
        }

        void Delete() {
            --currentSize;
        }

        int GetSize() {
            return currentSize;
        }

        ~Stack() {
            delete[] database;
        }
};


int main() {
    string str;
    cin >> str;

    Stack stack;

    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '(') {
            stack.Add(str[i]);
        } else {
            if (stack.GetSize() > 0) {
                stack.Delete();
            } else {
                cout << "NO" << '\n';
                return 0;
            }
        }
    }

    if (stack.GetSize() == 0) {
        cout << "YES" << '\n';
    } else {
        cout << "NO" << '\n';
    }

    return 0;
}
