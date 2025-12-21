#include <iostream>

using namespace std;

class BinaryTree {
    struct Node {
        int val;
        Node* left;
        Node* right;

        Node() : val(0), left(nullptr), right(nullptr) {}

        Node(int value) : val(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    public:

        BinaryTree() : root(nullptr) {}

        void CleanTree(Node* root) {
            if (!root) {
                return;
            }

            CleanTree(root->left);
            CleanTree(root->right);

            delete root;
        }


        void AddElement(int value)  {
            if (!root) {
                root = new Node{value};
                return;
            }

            Node* curNode = root;

            while (curNode) {
                if (value < curNode->val) {
                    if (!curNode->left) {
                        curNode->left = new Node{value};
                        break;
                    } else {
                        curNode = curNode->left;
                    }
                } else {
                    if (!curNode->right) {
                        curNode->right = new Node{value};
                        break;
                    } else {
                        curNode = curNode->right;
                    }
                }
            }
        }

        bool Check() {
            return Dfs(root, root->val);
        }

        bool Dfs(Node* root, int val) {
            if (root && root->val != val) {
                return false;
            }

            if (!root) {
                return true;
            }

            bool left = Dfs(root->left, val);
            bool right = Dfs(root->right, val);

            return left && right;
        }

        ~BinaryTree() {
            CleanTree(root);
        }
};

int main() {
    BinaryTree tree;

    int number;

    while (std::cin >> number) {
        tree.AddElement(number);
    }

    std::cout << tree.Check() << std::endl;

    return 0;
}
