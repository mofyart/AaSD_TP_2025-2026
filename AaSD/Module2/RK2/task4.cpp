#include <climits>
#include <iostream>

using namespace std;

class AVLTree {
    private:
        struct Node {
            int value;
            Node* left;
            Node* right;

            int height;

            Node(int element) : value(element) , left(nullptr), right(nullptr), height(1) {}
        };

        Node* root;

        bool FindElement(Node* node, int element) {
            if (!node) {
                return false;
            }

            if (element == node->value) {
                return true;
            }

            if (element < node->value) {
                return FindElement(node->left, element);
            } else {
                return FindElement(node->right, element);
            }
        }

        Node* doBalance(Node* node) {
            fixHeight(node);

            int ratio = findDifference(node);

            switch(ratio) {
                case 2:
                    if (findDifference(node->left) < 0) {
                        node->left = smallLeftRotate(node->left);
                    }

                    return smallRightRotate(node);
                case -2:
                    if (findDifference(node->right) > 0) {
                        node->right = smallRightRotate(node->right);
                    }

                    return smallLeftRotate(node);
            }

            return node;
        }

        Node* smallLeftRotate(Node* node) {
            Node* tempNode = node->right;
            node->right = tempNode->left;
            tempNode->left = node;

            fixHeight(node);
            fixHeight(tempNode);

            return tempNode;
        }

        Node* smallRightRotate(Node* node) {
            Node* tempNode = node->left;
            node->left = tempNode->right;
            tempNode->right = node;

            fixHeight(node);
            fixHeight(tempNode);

            return tempNode;
        }

        int findDifference(Node* node) {
            return getHeight(node->left) - getHeight(node->right);
        }

        void fixHeight(Node* node) {
            node->value = max(getHeight(node->left), getHeight(node->right)) + 1;
        }

        size_t getHeight(Node* node) {
            return node ?  node->height : 0;
        }

        Node* addInternal(Node* node, int element) {
            if (!root) {
                return new Node{element};
            }

            if (node->value < element) {
                node->left = addInternal(node->left, element);
            } else {
                node->right = addInternal(node->right, element);
            }

            return doBalance(node);
        }

        Node* deleteInternal(Node* node, int element) {
            if (!node) {
                return nullptr;
            }

            if (element == node->value) {
                Node* left = node->left;
                Node* right = node->right;

                delete node;

                if (!left) {
                    return right;
                }

                if (!right) {
                    return left;
                }

                if (getHeight(left->right) > getHeight(right->left)) {
                    Node* newNode = doBalance(findAndRemoveMin(right));

                    newNode->left = left;

                    if (newNode != right) {
                        newNode->right = right;
                    }

                    return doBalance(newNode);
                } else {
                    Node* newNode = doBalance(findAndRemoveMax(left));

                    if (newNode != left) {
                        newNode->left = left;
                    }

                    newNode->right = right;

                    return doBalance(newNode);
                }
            } else if (element < node->value) {
                node->left = deleteInternal(node->left, element);
            } else {
                node->right = deleteInternal(node->right, element);
            }

            return doBalance(node);
        }

        Node* findAndRemoveMin(Node*& node) {
            if (!node->left) {
                return node;
            }

            if (node->left && !node->left->left) {
                Node* minNode = node->left;
                node->left = node->left->right;
                node = doBalance(node);
                return minNode;
            }

            Node* minNode = findAndRemoveMin(node->left);

            node = doBalance(node);

            return minNode;
        }

        Node* findAndRemoveMax(Node*& node) {
            if (!node->right) {
                return node;
            }

            if (node->right && !node->right->right) {
                Node* maxNode = node->right;
                node->right = node->right->left;
                node = doBalance(node);

                return maxNode;
            }

            Node* maxNode = findAndRemoveMax(node->right);

            node = doBalance(node);

            return maxNode;
        }

        Node* findNextInternal(Node* node, int element) {
            if (!node) {
                return nullptr;
            }
        }

    public:

        void CleanTree(Node* node) {
            if (!node) {
                return;
            }

            CleanTree(node->left);
            CleanTree(node->right);

            delete node;
        }

        AVLTree() : root(nullptr) {}

        ~AVLTree() {
            CleanTree(root);
        }

        void Insert(int element) {
            if (Has(element)) {
                return;
            }

            root = addInternal(root, element);
        }

        bool Has(int element) {
            return FindElement(root, element);
        }

        void Delete(int element) {
            if (!Has(element)) {
                return;
            }

            root = deleteInternal(root, element);
        }

        int findNext(int element) {
            if (!Has(element)) {
                return INT_MIN;
            }
        }

        int findPrev(int element) {
            if (!Has(element)) {
                return INT_MAX;
            }
        }

};

int main() {
    AVLTree tree;

    std::string cmd;
    int x;

    while (std::cin >> cmd >> x) {
        if (cmd == "insert") {
            tree.Insert(x);
        } else if (cmd == "delete") {
            tree.Delete(x);
        } else if (cmd == "exists") {
            std::cout << (tree.Has(x) ? "true" : "false") << "\n";
        } else if (cmd == "next") {
            auto it = tree.findNext(x);
            if (it != INT_MIN) {
                std::cout << it << "\n";
            } else {
                std::cout << "none" << "\n";
            }
        } else if (cmd == "prev") {
            auto it = tree.findPrev(x);
            if (it != INT_MAX) {
                std::cout << it << "\n";
            } else {
                std::cout << "none" << "\n";
            }
        }
    }

    return 0;
}
