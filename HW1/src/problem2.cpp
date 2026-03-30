#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) {
        key = k;
        left = NULL;
        right = NULL;
    }
};

Node* Insert(Node* root, int x) {
    if (root == NULL) {
        return new Node(x);
    }

    if (x < root->key) {
        root->left = Insert(root->left, x);
    }
    else if (x > root->key) {
        root->right = Insert(root->right, x);
    }

    return root;
}

int Height(Node* root) {
    if (root == NULL) return 0;

    int a = Height(root->left);
    int b = Height(root->right);

    return (a > b ? a : b) + 1;
}

Node* FindMin(Node* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

Node* Delete(Node* root, int k) {
    if (root == NULL) return NULL;

    if (k < root->key) {
        root->left = Delete(root->left, k);
    }
    else if (k > root->key) {
        root->right = Delete(root->right, k);
    }
    else {
        // 找到要刪的節點

        // case 1: 沒有左子樹
        if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        }

        // case 2: 沒有右子樹
        if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // case 3: 左右子樹都有
        Node* temp = FindMin(root->right);
        root->key = temp->key;
        root->right = Delete(root->right, temp->key);
    }

    return root;
}

void Inorder(Node* root) {
    if (root == NULL) return;
    Inorder(root->left);
    cout << root->key << " ";
    Inorder(root->right);
}

void Destroy(Node* root) {
    if (root == NULL) return;
    Destroy(root->left);
    Destroy(root->right);
    delete root;
}

int main() {
    srand((unsigned)time(0));

    cout << "Part (a): height / log2(n)" << endl;

    for (int n = 100; n <= 10000; n += 500) {
        Node* root = NULL;

        for (int i = 0; i < n; i++) {
            int x = rand();
            root = Insert(root, x);
        }

        int h = Height(root);
        double ratio = h / log2((double)n);

        cout << "n = " << n
            << ", height = " << h
            << ", height/log2(n) = " << ratio << endl;

        Destroy(root);
    }

    cout << endl;
    cout << "Part (b): delete key from BST" << endl;

    Node* root = NULL;
    root = Insert(root, 50);
    root = Insert(root, 30);
    root = Insert(root, 70);
    root = Insert(root, 20);
    root = Insert(root, 40);
    root = Insert(root, 60);
    root = Insert(root, 80);

    cout << "Original BST (inorder): ";
    Inorder(root);
    cout << endl;

    root = Delete(root, 50);

    cout << "After deleting 50: ";
    Inorder(root);
    cout << endl;

    Destroy(root);

    return 0;
}