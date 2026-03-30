# 41343103

## 解題說明

### 問題描述:
- MinPQ 是抽象類別 
- MinHeap 繼承 MinPQ 
- 實作： 
- IsEmpty()
- Top()
- Push()
- Pop()

### 解題策略:
Binary Heap:
- 父節點：`(i-1)/2`
- 左子節點：`2*i+1`
- 右子節點：`2*i+2`

Push:
步驟：
- 將新元素加到最後
- 向上比較（heapify up）
- 若比父節點小 → 交換

Pop:
- 將最後一個元素放到 root
- 向下比較（heapify down）
- 與較小的子節點交換

Top:
- 直接回傳 root

## 程式實作

```cpp
#include <iostream>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int size;

    void Resize() {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = heap[i];
        }
        delete[] heap;
        heap = temp;
    }

public:
    MinHeap(int c = 10) {
        capacity = c;
        size = 0;
        heap = new T[capacity];
    }

    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    const T& Top() const {
        if (IsEmpty()) {
            cout << "Heap is empty!" << endl;
            exit(1);
        }
        return heap[0];
    }

    void Push(const T& x) {
        if (size == capacity) {
            Resize();
        }

        int i = size;
        size++;
        heap[i] = x;

        while (i > 0) {
            int p = (i - 1) / 2;
            if (heap[p] <= heap[i]) break;

            T temp = heap[p];
            heap[p] = heap[i];
            heap[i] = temp;

            i = p;
        }
    }

    void Pop() {
        if (IsEmpty()) {
            cout << "Heap is empty!" << endl;
            return;
        }

        heap[0] = heap[size - 1];
        size--;

        int i = 0;
        while (2 * i + 1 < size) {
            int child = 2 * i + 1;

            if (child + 1 < size && heap[child + 1] < heap[child]) {
                child++;
            }

            if (heap[i] <= heap[child]) break;

            T temp = heap[i];
            heap[i] = heap[child];
            heap[child] = temp;

            i = child;
        }
    }

    void Print() const {
        for (int i = 0; i < size; i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    MinHeap<int> h;

    h.Push(30);
    h.Push(10);
    h.Push(20);
    h.Push(5);
    h.Push(15);

    cout << "Heap: ";
    h.Print();

    cout << "Top = " << h.Top() << endl;

    h.Pop();
    cout << "After Pop: ";
    h.Print();

    cout << "Top = " << h.Top() << endl;

    return 0;
}
```

## 效能分析

### 時間複雜度:
- IsEmpty() → O(1)
- Top() → O(1)
- Push() → O(log n)
- Pop() → O(log n)

### 空間複雜度:
- O(n)
  
## 測試與驗證
```shell
Heap: 5 10 20 30 15
Top = 5
After Pop: 10 15 20 30
Top = 10
```

## 申論及開發報告
本題透過陣列模擬完全二元樹，使插入與刪除皆維持對數時間。
相比線性結構（如 array 或 linked list），效率大幅提升。

## 解題說明

### 問題描述:
1. 
- 建立空的 BST
- 插入很多隨機數
- 算樹高 height
- 算 height / log2(n)
- 印出結果
2. 
- 從 BST 中刪除指定 key，需維持 BST 性質
  
### 解題策略:
1. 
- 建立空 BST
- 使用 rand() 產生隨機數
- 插入 n 個節點
- 使用遞迴計算樹高
- 計算 ratio
- 重複不同 n 值
2. 
- 無子節點 → 直接刪除
- 一個子節點 → 用該子節點取代
- 兩個子節點 → 找右子樹最小值作為替代 → 再刪除該節點  

## 程式實作
```cpp
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
    } else if (x > root->key) {
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
    } else if (k > root->key) {
        root->right = Delete(root->right, k);
    } else {
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
```

## 效能分析

### 時間複雜度:
1.
- Insert → O(log n) 
- Height → O(n) 
2.
- Delete → O(log n)
### 空間複雜度:
- O(n)

## 測試與驗證
```shell
Part (a): height / log2(n)

n = 100, height = 13, ratio = 1.95
n = 600, height = 18, ratio = 2.01
n = 1100, height = 20, ratio = 2.05
```

## 申論及開發報告
- Binary Search Tree（BST）是一種常見的樹狀資料結構
- 左子樹所有節點小於根節點，右子樹所有節點大於根節點
- 透過隨機插入結果顯示：height ≈ 2 log2(n) 表示隨機 BST 在平均情況下具有良好的平衡性
- 實作刪除操作，透過處理三種情況維持 BST 結構，確保資料仍符合排序特性
