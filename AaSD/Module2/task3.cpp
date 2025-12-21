#include <cassert>
#include <iostream>
#include <vector>
#include <queue>

template <typename T, typename Comparator = std::less<T>>
class BTree {
    private:
        struct Node {
            std::vector<T> keys;
            std::vector<Node*> descendants;
            bool leaf;

            Node(bool valLeaf): leaf(valLeaf) {}

            ~Node() {
                for (size_t i = 0; i < descendants.size(); ++i) {
                    delete descendants[i];
                }
            }
        };

        Node* root;
        Comparator comparator;
        size_t degreeTree;

        bool IsFullNode(Node* node);

        void SplitNode(Node* fullNode, int indexDescendes);

        void InsertElementNotFull(Node* node, const T& element);

    public:
        BTree(size_t degreeTreeVal, Comparator comparatorVal) : root(nullptr), degreeTree(degreeTreeVal), comparator(comparatorVal) {}

        BTree(const BTree& other);

        ~BTree();

        BTree& operator=(const BTree& other);

        void InsertElement(const T& element);

        void PrintTree();
};

template <typename T, typename Comparator>
BTree<T, Comparator>::~BTree() {
    if (root) {
        delete root;
    }
}

template <typename T, typename Comparator>
BTree<T, Comparator>& BTree<T, Comparator>::operator=(const BTree& other) {
    if (this != &other || !other.root) {
        return *this;
    }

    delete root;

    std::queue<Node*> queueNodes;

    queueNodes.push(other.root);

    while (!queueNodes.empty()) {
        Node* frontNode = queueNodes.front();
        queueNodes.pop();

        for (size_t i = 0; i < frontNode->keys.size(); ++i) {
            if (!frontNode->leaf) {
                queueNodes.push(frontNode->descendants[i]);
            }

            InsertElement(frontNode->keys[i]);
        }

        if (!frontNode->leaf) {
            queueNodes.push(frontNode->descendants[frontNode->keys.size()]);
        }
    }

    return *this;
}

template <typename T, typename Comparator>
BTree<T, Comparator>::BTree(const BTree& other) {
    if (!other.root) {
        return;
    }

    std::queue<Node*> queueNodes;

    queueNodes.push(other.root);

    while (!queueNodes.empty()) {
        Node* frontNode = queueNodes.front();
        queueNodes.pop();

        for (size_t i = 0; i < frontNode->keys.size(); ++i) {
            if (!frontNode->leaf) {
                queueNodes.push(frontNode->descendants[i]);
            }

            InsertElement(frontNode->keys[i]);
        }

        if (!frontNode->leaf) {
            queueNodes.push(frontNode->descendants[frontNode->keys.size()]);
        }
    }
}

template <typename T, typename Comparator>
bool BTree<T, Comparator>::IsFullNode(Node* node) {
    return node->keys.size() >= 2 * degreeTree - 1;
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::SplitNode(Node* node, int indexDescend) {
    int sizeKeys = node->descendants[indexDescend]->keys.size();

    int indexMedianKey = sizeKeys / 2;
    T medianKey = node->descendants[indexDescend]->keys[indexMedianKey];

    bool isLeafSplitedNodes = node->descendants[indexDescend]->leaf;

    // создаём два новых узла
    Node* leftSubtree = new Node(isLeafSplitedNodes);
    Node* rightSubtree = new Node(isLeafSplitedNodes);

    leftSubtree->keys.resize(indexMedianKey);
    rightSubtree->keys.resize(sizeKeys - 1 - indexMedianKey);

    leftSubtree->descendants.resize(indexMedianKey + 1);
    rightSubtree->descendants.resize(sizeKeys - indexMedianKey);

    // зполнение этих двух узлов отноcительно медианы первоначального узла
    for (size_t i = 0; i < indexMedianKey; ++i) {
        leftSubtree->keys[i] = node->descendants[indexDescend]->keys[i];

        if (node->descendants[indexDescend]->descendants.size() > i) {
            leftSubtree->descendants[i] = node->descendants[indexDescend]->descendants[i];
        }
    }

    if (node->descendants[indexDescend]->descendants.size() > indexMedianKey) {
        leftSubtree->descendants[indexMedianKey] = node->descendants[indexDescend]->descendants[indexMedianKey];
    }

    for (size_t i = indexMedianKey + 1; i < sizeKeys; ++i) {
        rightSubtree->keys[i - 1 - indexMedianKey] = node->descendants[indexDescend]->keys[i];

        if (node->descendants[indexDescend]->descendants.size() > i) {
            rightSubtree->descendants[i - 1 - indexMedianKey] = node->descendants[indexDescend]->descendants[i];
        }
    }

    if (node->descendants[indexDescend]->descendants.size() > sizeKeys) {
        rightSubtree->descendants[sizeKeys - 1 - indexMedianKey] = node->descendants[indexDescend]->descendants[sizeKeys];
    }

    node->descendants.resize(node->descendants.size() + 1);

    node->descendants[indexDescend]->descendants.clear();
    delete node->descendants[indexDescend];
    node->descendants[indexDescend] = nullptr;

    for (int i = node->descendants.size() - 2; i > indexDescend; --i) {
        node->descendants[i + 1] = node->descendants[i];
    }

    node->descendants[indexDescend] = leftSubtree;
    node->descendants[indexDescend + 1] = rightSubtree;

    // добавляем значение key в иcходный узел
    node->keys.resize(node->keys.size() + 1);

    for (int i = node->keys.size() - 2; i >= indexDescend; --i) {
        node->keys[i + 1] = node->keys[i];
    }

    node->keys[indexDescend] = medianKey;
}

template <typename T, typename Comparator>
void  BTree<T, Comparator>::InsertElementNotFull(Node* node, const T& element) {
    if (node->leaf) {
        node->keys.resize(node->keys.size() + 1);

        int pos = 0;
        for (pos = node->keys.size() - 2; pos >= 0 && comparator(element, node->keys[pos]); --pos) {
            node->keys[pos + 1] = node->keys[pos];
        }
        node->keys[pos + 1] = element;

    } else {
        int pos = 0;
        for (pos = node->keys.size() - 1; pos >= 0; --pos) {
            if (!comparator(element, node->keys[pos])) {
                break;
            }
        }

        if (IsFullNode(node->descendants[pos + 1])) {
            SplitNode(node, pos + 1);

            if (!comparator(element, node->keys[pos + 1])) {
                ++pos;
            }
        }

        InsertElementNotFull(node->descendants[pos + 1], element);
    }
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::InsertElement(const T& element) {
    if (!root) {
        root = new Node(true);
    }

    if (IsFullNode(root)) {
        Node* newRoot = new Node(false);
        newRoot->descendants.push_back(root);
        root = newRoot;
        SplitNode(root, 0);
    }

    InsertElementNotFull(root, element);
}

template <typename T, typename Comparator>
void BTree<T, Comparator>::PrintTree() {
    std::queue<Node*> queueNodes;

    queueNodes.push(root);

    while (!queueNodes.empty()) {
        int numberNodePerSlice = queueNodes.size();

        while (numberNodePerSlice > 0) {
            Node* frontNode = queueNodes.front();
            queueNodes.pop();

            for (size_t i = 0; i < frontNode->keys.size(); ++i) {
                if (!frontNode->leaf) {
                    queueNodes.push(frontNode->descendants[i]);
                }
                std::cout << frontNode->keys[i] << ' ';
            }

            if (!frontNode->leaf) {
                queueNodes.push(frontNode->descendants[frontNode->keys.size()]);
            }

            --numberNodePerSlice;
        }

        std::cout << std::endl;
    }

}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int minTreeOrder = 0;
    std::cin >> minTreeOrder;

    auto comparator = [](int firstElement, int secondElement){return firstElement < secondElement;};
    BTree<int, decltype(comparator)> tree(minTreeOrder, comparator);

    int number = 0;

    while (std::cin >> number) {
        assert(!std::cin.fail());

        tree.InsertElement(number);
    }

    tree.PrintTree();

    return 0;
}
