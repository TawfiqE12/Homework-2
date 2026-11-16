// Implement only this file. See avl.h and the assignment for contracts.
#include "avl.h"
#include <algorithm>

int nodeHeight(const Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}
void updateHeight(Node* node) {
    node->height = 1 + std::max(nodeHeight(node->left), nodeHeight(node->right));
}

int balanceFactor(const Node* node) {
    if (node == nullptr) {
        return 0;
    }

    return nodeHeight(node->left) - nodeHeight(node->right);
}

Node* rotateLeft(Node* root) {
    Node* newRoot = root->right;
    Node* temp = newRoot->left;

    newRoot->left = root;
    root->right = temp;

    updateHeight(root);
    updateHeight(newRoot);

    return newRoot;
}

Node* rotateRight(Node* root) {
    Node* newRoot = root->left;
    Node* temp = newRoot->right;

    newRoot->right = root;
    root->left = temp;

    updateHeight(root);
    updateHeight(newRoot);

    return newRoot;
}
Node* rebalance(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }

    updateHeight(root);

    int balance = balanceFactor(root);

    if (balance > 1) {
        if (balanceFactor(root->left) < 0) {
            root->left = rotateLeft(root->left);
        }
        return rotateRight(root);
    }

    if (balance < -1) {
        if (balanceFactor(root->right) > 0) {
            root->right = rotateRight(root->right);
        }
        return rotateLeft(root);
    }

    return root;
}
Node* insertNode(Node* root, Node* newNode) {
    if (root == nullptr) {
        return newNode;
    }

    if (newNode->data.departure < root->data.departure) {
        root->left = insertNode(root->left, newNode);
    } else {
        root->right = insertNode(root->right, newNode);
    }

    return rebalance(root);
}


Node* deleteNode(Node* root, int departure) {
    if (root == nullptr) {
        return nullptr;
    }

    if (departure < root->data.departure) {
        root->left = deleteNode(root->left, departure);
    }
    else if (departure > root->data.departure) {
        root->right = deleteNode(root->right, departure);
    }
    else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* child = (root->left != nullptr)
                ? root->left
                : root->right;

            delete root;
            return child;
        }
        else {
            Node* successor = root->right;

            while (successor->left != nullptr) {
                successor = successor->left;
            }

            root->data = successor->data;

            root->right = deleteNode(
                root->right,
                successor->data.departure
            );
        }
    }

    return rebalance(root);
}

const Node* findNode(const Node* root, int departure) {
    if (root == nullptr) {
        return nullptr;
    }

    if (departure == root->data.departure) {
        return root;
    }

    if (departure < root->data.departure) {
        return findNode(root->left, departure);
    }

    return findNode(root->right, departure);
}

const Node* earliestNode(const Node* root, int departure) {
    const Node* result = nullptr;

    while (root != nullptr) {
        if (root->data.departure >= departure) {
            result = root;
            root = root->left;
        } else {
            root = root->right;
        }
    }

    return result;
}

void findRange(const Node* root, int start, int end,
               const Node* results[], int& count) {
    if (root == nullptr || start > end) {
        return;
    }

    if (root->data.departure > start) {
        findRange(root->left, start, end, results, count);
    }

    if (root->data.departure >= start &&
        root->data.departure <= end) {
        results[count++] = root;
    }

    if (root->data.departure < end) {
        findRange(root->right, start, end, results, count);
    }
}
