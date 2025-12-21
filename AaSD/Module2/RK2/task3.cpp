#include <iostream>
#include <unordered_set>

using namespace std;


struct Node {
    std::string product_name;
    int color;
    int size;

    Node(std::string valName, int valColor, int valSize) : product_name(valName), color(valColor), size(valSize) {}

    bool operator==(const Node& other) const {
        return (product_name == other.product_name && color == other.color && size == other.size);
    }
};

namespace std {
    template<>
        struct hash<Node> {
                std::size_t operator()(const Node& node) const noexcept {
                    size_t hString = hash<string>{}(node.product_name);
                    size_t hColor = hash<int>{}(node.color);
                    size_t hSize = hash<int>{}(node.size);

                    return hString ^ (hColor << 1) ^ (hSize << 2);
                }
        };
}


int main() {
    unordered_set<Node> setNode;

    char operation;

    while(cin >> operation) {
        string nameNode;
        int color;
        int size;

        cin >> nameNode;
        cin >> color;
        cin >> size;

        Node newNode(nameNode, color, size);

        switch (operation) {
        case '?':
            setNode.count(newNode) ? cout << "OK" << '\n' : cout << "FAIL" << '\n';
            break;

        case '+':
            if (setNode.count(newNode)) {
                cout << "FAIL" << '\n';
            } else {
                setNode.insert(newNode);
                cout << "OK" << '\n';
            }
            break;

        case '-':
            if (setNode.count(newNode)) {
                setNode.erase(newNode);
                cout << "OK" << '\n';
            } else {
                cout << "FAIL" << '\n';
            }
            break;
        default:
            break;
        }
    }

    return 0;
}
