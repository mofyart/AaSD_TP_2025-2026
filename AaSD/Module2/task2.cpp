#include <cassert>
#include <iostream>
#include <stack>
#include <queue>

template <typename T, typename Comparator>
class BinarySearchTree {
    private:
        struct Node {
            T val;
            Node* left = nullptr;
            Node* right = nullptr;
        };

        Node* root;
        Comparator comparator;
    public:
        BinarySearchTree(Comparator comparatorVal) : root(nullptr), comparator(comparatorVal) {}

        BinarySearchTree(const T& value, Comparator comparatorVal) : root(new Node{value, nullptr, nullptr}), comparator(comparatorVal) {}

        BinarySearchTree(const BinarySearchTree& other);

        ~BinarySearchTree();

        BinarySearchTree& operator=(const BinarySearchTree& other);

        void AddElement(const T& element);

        void CleanTree();

        void PrintNodesPostOrdered() const;

        void PreOrderedRun(Node* curNode);
};

template <typename T, typename Comparator>
BinarySearchTree<T, Comparator>::BinarySearchTree(const BinarySearchTree& other) : root(nullptr), comparator(other.comparator) {
    PreOrderedRun(other.root);
}

template <typename T, typename Comparator>
BinarySearchTree<T, Comparator>::~BinarySearchTree() {
    if (!root) return;

    CleanTree();

    root = nullptr;
}

template <typename T, typename Comparator>
BinarySearchTree<T, Comparator>& BinarySearchTree<T, Comparator>::operator=(const BinarySearchTree& other) {
    if (this == &other) {
        return *this;
    }

    CleanTree();

    root = nullptr;

    PreOrderedRun(other.root);

    return *this;
}


template <typename T, typename Comparator>
void BinarySearchTree<T, Comparator>::AddElement(const T& element) {
    if (!root) {
        root = new Node{element};
        return;
    }

    Node* curNode = root;

    while (curNode) {
        if (comparator(element, curNode->val)) {
            if (!curNode->left) {
                curNode->left = new Node{element};
                break;
            } else {
                curNode = curNode->left;
            }
        } else {
            if (!curNode->right) {
                curNode->right = new Node{element};
                break;
            } else {
                curNode = curNode->right;
            }
        }
    }
}

template <typename T, typename Comparator>
void BinarySearchTree<T, Comparator>::CleanTree() {
    if (!root) return;

    std::queue<Node*> nodesQueue;
    nodesQueue.push(root);

    while (!nodesQueue.empty()) {
        Node* curNode = nodesQueue.front();

        nodesQueue.pop();

        if (curNode->left) {
            nodesQueue.push(curNode->left);
        }

        if (curNode->right) {
            nodesQueue.push(curNode->right);
        }

        delete curNode;
    }
}

template <typename T, typename Comparator>
void BinarySearchTree<T, Comparator>::PrintNodesPostOrdered() const {
    Node* curNode = root;
    Node* lastVisited = nullptr;

    std::stack<Node*> nodesStack;

    while (curNode || !nodesStack.empty()) {
        if (curNode) {
            nodesStack.push(curNode);
            curNode = curNode->left;
        } else {
            Node* topElement = nodesStack.top();

            if (topElement->right && topElement->right != lastVisited) {
                curNode = topElement->right;
            } else {
                std::cout << topElement->val << ' ';
                lastVisited = topElement;
                nodesStack.pop();
            }
        }
    }
}

template <typename T, typename Comparator>
void BinarySearchTree<T, Comparator>::PreOrderedRun(Node* curNode) {
    std::stack<Node*> nodesStack;

    while (!nodesStack.empty || curNode) {
        if (curNode) {
            AddElement(curNode->val);
            nodesStack.push(curNode);

            curNode = curNode->left;
        } else {
            Node* topElement = nodesStack.top();

            curNode = topElement->right;

            nodesStack.pop();
        }
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int n = 0;
    std::cin >> n;

    auto comparator = [](int firstElement, int secondElement){return firstElement < secondElement;};
    BinarySearchTree<int, decltype(comparator)> bts(comparator);

    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;

        assert(!std::cin.fail());

        bts.AddElement(value);
    }

    bts.PrintNodesPostOrdered();

    return 0;
}
