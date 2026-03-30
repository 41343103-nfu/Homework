# 41343103

## 解題說明

### 問題描述:
MinPQ 是抽象類別
MinHeap 繼承 MinPQ
實作這4個函式：
- IsEmpty()
- Top()
- Push()
- Pop()

### 解題策略:


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

### 複雜度:
- IsEmpty() → O(1)
- Top() → O(1)
- Push() → O(log n)
- Pop() → O(log n)
  
## 測試與驗證



## 申論及開發報告




## 解題說明

### 問題描述:

  
### 解題策略:

  
## 程式實作

```cpp

```

## 效能分析

### 時間複雜度:


### 空間複雜度:


## 測試與驗證


## 申論及開發報告
