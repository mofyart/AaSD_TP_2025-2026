// Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных
// чисел, а также запрос на получение k-ой порядковой статистики. Команда добавления числа
// A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
// Запрос на получение k-ой порядковой статистики задается числом k.
// Требуемая скорость выполнения запроса - O(log n).

#include <cassert>
#include <iostream>
#include <queue>

template <typename T, typename Comparator = std::less<T>>
class AVLTree {
    private:
        struct Node {
            T val;
            Node* leftNode;
            Node* rightNode;

            int height;
            int countNodes;

            Node(const T& element) : val(element), leftNode(nullptr), rightNode(nullptr), height(1), countNodes(1) {}
        };

        Node* root;
        Comparator comparator;

        Node* addInternal(Node* node, const T& element);

        Node* deleteInternal(Node* node, const T& element);

        Node* doBalance(Node* node);

        int getHeightNode(Node* node);

        int getCountNodes(Node* node);

        int getBalanceNode(Node* node);

        void fixHeight(Node* node);

        void fixCountNodes(Node* node);

        Node* smallLeftRotate(Node* node);

        Node* smallRightRotate(Node* node);

        Node* findAndRemoveMin(Node*& node);

        Node* findAndRemoveMax(Node*& node);

        int internalFindStatistic(Node* node, size_t indexKthStatistic);

        void destroyTree(Node* node);
    public:
        AVLTree(Comparator comparatorVal) : root(nullptr), comparator(comparatorVal) {}

        AVLTree(const AVLTree& otherTree);

        AVLTree& operator=(const AVLTree& otherTree);

        ~AVLTree();

        void AddElement(const T& element);

        void DeleteElement(const T& element);

        void FindKthStatistic(size_t indexKthStatistic);
};

template <typename T, typename Comparator>
typename AVLTree<T, Comparator>::Node* AVLTree<T, Comparator>::addInternal(Node* node, const T& element) {
    if (!node) {
        return new Node(element);
    }

    if (comparator(element, node->val)) {
        node->leftNode = addInternal(node->leftNode, element);
    } else {
        node->rightNode = addInternal(node->rightNode, element);
    }

    return doBalance(node);
}

template <typename T, typename Comparator>
typename AVLTree<T, Comparator>::Node* AVLTree<T, Comparator>::deleteInternal(Node* node, const T& element) {
    if (!node) {
        return nullptr;
    }

    if (element == node->val) {
        Node* left = node->leftNode;
        Node* right = node->rightNode;

        delete node;

        if (!left) {
            return right;
        }

        if (!right) {
            return left;
        }

        if (getHeightNode(left->rightNode) > getHeightNode(right->leftNode)) {
            Node* newNode = doBalance(findAndRemoveMin(right));

            newNode->leftNode = left;

            if (newNode != right) {
                newNode->rightNode = right;
            }

            return doBalance(newNode);
        } else {
            Node* newNode = doBalance(findAndRemoveMax(left));

            if (newNode != left) {
                newNode->leftNode = left;
            }

            newNode->rightNode = right;

            return doBalance(newNode);
        }
    } else if (comparator(element, node->val)) {
        node->leftNode = deleteInternal(node->leftNode, element);
    } else {
        node->rightNode = deleteInternal(node->rightNode, element);
    }

    return doBalance(node);
}

template <typename T, typename Comparator>
typename AVLTree<T, Comparator>::Node* AVLTree<T, Comparator>::doBalance(Node* node) {
    fixHeight(node);
    fixCountNodes(node);

    int ratioBlance = getBalanceNode(node);

    switch (ratioBlance) {
    case 2:
        if (getBalanceNode(node->leftNode) < 0) {
            node->leftNode = smallLeftRotate(node->leftNode);
        }
        return smallRightRotate(node);
    case -2:
        if (getBalanceNode(node->rightNode) > 0) {
            node->rightNode = smallRightRotate(node->rightNode);
        }
        return smallLeftRotate(node);
    default:
        return node;
    }
}

template <typename T, typename Comparator>
int AVLTree<T, Comparator>::getHeightNode(Node* node) {
    return node ? node->height : 0;
}

template <typename T, typename Comparator>
int AVLTree<T, Comparator>::getCountNodes(Node* node) {
    return node ? node->countNodes : 0;
}

template <typename T, typename Comparator>
int AVLTree<T, Comparator>::getBalanceNode(Node* node) {
    return getHeightNode(node->leftNode) - getHeightNode(node->rightNode);
}

template <typename T, typename Comparator>
void AVLTree<T, Comparator>::fixHeight(Node* node) {
    node->height = std::max(getHeightNode(node->leftNode), getHeightNode(node->rightNode)) + 1;
}

template <typename T, typename Comparator>
void AVLTree<T, Comparator>::fixCountNodes(Node* node) {
    node->countNodes = getCountNodes(node->leftNode) + getCountNodes(node->rightNode) + 1;
}

template <typename T, typename Comparator>
typename AVLTree<T, Comparator>::Node* AVLTree<T, Comparator>::smallLeftRotate(Node* node) {
    Node* tempNode = node->rightNode;
    node->rightNode = tempNode->leftNode;
    tempNode->leftNode = node;

    fixHeight(node);
    fixHeight(tempNode);

    fixCountNodes(node);
    fixCountNodes(tempNode);

    return tempNode;
}

template <typename T, typename Comparator>
typename AVLTree<T, Comparator>::Node* AVLTree<T, Comparator>::smallRightRotate(Node* node) {
    Node* tempNode = node->leftNode;
    node->leftNode = tempNode->rightNode;
    tempNode->rightNode = node;

    fixHeight(node);
    fixHeight(tempNode);

    fixCountNodes(node);
    fixCountNodes(tempNode);

    return tempNode;
}

template <typename T, typename Comparator>
typename AVLTree<T, Comparator>::Node* AVLTree<T, Comparator>::findAndRemoveMin(Node*& node) {
    if (!node->leftNode) {
        return node;
    }

    if (node->leftNode && !node->leftNode->leftNode) {
        Node* minNode = node->leftNode;
        node->leftNode = node->leftNode->rightNode;
        node = doBalance(node);
        return minNode;
    }

    Node* minNode = findAndRemoveMin(node->leftNode);

    node = doBalance(node);

    return minNode;
}

template <typename T, typename Comparator>
typename AVLTree<T, Comparator>::Node* AVLTree<T, Comparator>::findAndRemoveMax(Node*& node) {
    if (!node->rightNode) {
        return node;
    }

    if (node->rightNode && !node->rightNode->rightNode) {
        Node* maxNode = node->rightNode;
        node->rightNode = node->rightNode->leftNode;
        node = doBalance(node);

        return maxNode;
    }

    Node* maxNode = findAndRemoveMax(node->rightNode);

    node = doBalance(node);

    return maxNode;
}

template <typename T, typename Comparator>
int AVLTree<T, Comparator>::internalFindStatistic(Node* node, size_t indexKthStatistic) {
    if (!node) {
        return -1;
    }

    int leftCount = getCountNodes(node->leftNode);
    int rightCount = getCountNodes(node->rightNode);

    if (indexKthStatistic == leftCount) {
        return node->val;
    } else if (indexKthStatistic < leftCount) {
        return internalFindStatistic(node->leftNode, indexKthStatistic);
    } else {
        return internalFindStatistic(node->rightNode, indexKthStatistic - leftCount - 1);
    }
}

template <typename T, typename Comparator>
void AVLTree<T, Comparator>::destroyTree(Node* node) {
    if (!node) {
        return;
    }

    if (node->leftNode) {
        destroyTree(node->leftNode);
    }

    if (node->rightNode) {
        destroyTree(node->rightNode);
    }

    delete node;
}

template <typename T, typename Comparator>
AVLTree<T, Comparator>::AVLTree(const AVLTree<T, Comparator>& otherTree) {
    if (!otherTree.root) {
        return;
    }

    comparator = otherTree.comparator;

    std::queue<Node*> queueNodes;
    queueNodes.push(otherTree.root);

    while(!queueNodes.empty()) {
        Node* frontNode = queueNodes.front();
        queueNodes.pop();

        AddElement(frontNode->val);

        if (frontNode->leftNode) {
            queueNodes.push(frontNode->leftNode);
        }

        if (frontNode->rightNode) {
            queueNodes.push(frontNode->rightNode);
        }
    }

}

template <typename T, typename Comparator>
AVLTree<T, Comparator>& AVLTree<T, Comparator>::operator=(const AVLTree<T, Comparator>& otherTree) {
    if (this == &otherTree) {
        return *this;
    }

    destroyTree(root);
    root = nullptr;

    if (!otherTree.root) {
        return *this;
    }

    std::queue<Node*> queueNodes;
    queueNodes.push(otherTree.root);

    while(!queueNodes.empty()) {
        Node* frontNode = queueNodes.front();
        queueNodes.pop();

        AddElement(frontNode->val);

        if (frontNode->leftNode) {
            queueNodes.push(frontNode->leftNode);
        }

        if (frontNode->rightNode) {
            queueNodes.push(frontNode->rightNode);
        }
    }

    return *this;
}

template <typename T, typename Comparator>
AVLTree<T, Comparator>::~AVLTree() {
    destroyTree(root);
}

template <typename T, typename Comparator>
void AVLTree<T, Comparator>::AddElement(const T& element) {
    root = addInternal(root, element);
}

template <typename T, typename Comparator>
void AVLTree<T, Comparator>::DeleteElement(const T& element) {
    root = deleteInternal(root, element);
}

template <typename T, typename Comparator>
void AVLTree<T, Comparator>::FindKthStatistic(size_t indexKthStatistic) {
    int result = internalFindStatistic(root, indexKthStatistic);

    std::cout << result << '\n';
}

class IntComparator {
    public:
        bool operator()(int firstElement, int secondElement) const {
            return firstElement < secondElement;
        }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    int quantityOperations = 0;
    std::cin >> quantityOperations;

    IntComparator comparator;
    AVLTree<int, IntComparator> tree(comparator);

    for (int i = 0; i < quantityOperations; ++i) {
        int number = 0;
        size_t indexKthStatistic = 0;
        std::cin >> number >> indexKthStatistic;

        assert(!std::cin.fail());

        if (number > 0) {
            tree.AddElement(number);
        } else {
            tree.DeleteElement(std::abs(number));
        }

        tree.FindKthStatistic(indexKthStatistic);
    }

    return 0;
}
