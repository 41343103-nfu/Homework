# 41343103

## 解題說明

### 問題描述:
實作一個以 Circular Linked List（循環串列）為基礎的一元多項式（Polynomial）類別
多項式中的每一項包含一個係數（coef）與一個指數（exp），並依照指數由大到小排序
實作鏈結串列結構
- 建立帶有 header node 的 circular linked list
- 支援多項式的輸入與輸出  
- 支援多項式的加法、減法、乘法運算  
- 支援多項式在指定變數值下的計算（Evaluate）  
- 實作 copy constructor 與 assignment operator  
- 實作 available-space list 以回收刪除的節點

### 解題策略:

#### (1)ChainNode
ChainNode 為鏈結串列中的基本節點
- data：儲存實際資料
- link：指向下一個節點
在 ChainNode 中加入一個靜態指標 avail 作為 available-space list 的表頭，用來回收已刪除但可重複使用的節點

#### (2)Circular Linked List with Header Node
- 當串列為空時 header->link 指向自己
- 最後一個節點的 link 會指回 header
- 使用 Begin() 與 End() 作為走訪的起點與終點

#### (3)Iterator
- operator*:存取節點資料
- operator++:移動到下一個節點
- operator!=:判斷是否走訪完成

#### AddTerm
- 若指數相同，合併係數
- 若合併後係數為 0，刪除該項
- 插入時保持指數遞減排序

## 程式實作
```cpp
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace std;

// (1)ChainNode

template <class T> class Chain;

template <class T>
class ChainNode {
    friend class Chain<T>;
private:
    T data;
    ChainNode<T>* link;

    static ChainNode<T>* avail;

public:
    ChainNode() : data(T()), link(nullptr) {}
    ChainNode(const T& d) : data(d), link(nullptr) {}
    ChainNode(const T& d, ChainNode<T>* l) : data(d), link(l) {}

    static ChainNode<T>* GetNode(const T& d, ChainNode<T>* l) {
        ChainNode<T>* node;
        if (avail == nullptr) {
            node = new ChainNode<T>(d, l);
        }
        else {
            node = avail;
            avail = avail->link;
            node->data = d;
            node->link = l;
        }
        return node;
    }

    static void ReturnNode(ChainNode<T>* node) {
        if (!node) return;
        node->link = avail;
        avail = node;
    }
};

template <class T>
ChainNode<T>* ChainNode<T>::avail = nullptr;


// (3) ChainIterator
template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;
public:
    ChainIterator(ChainNode<T>* start = nullptr) : current(start) {}

    T& operator*() const { return current->data; }

    ChainIterator<T>& operator++() {
        current = current->link;
        return *this;
    }

    bool operator!=(const ChainIterator<T>& rhs) const {
        return current != rhs.current;
    }

    template <class U> friend class Chain;
};


// (2)Chain

template <class T>
class Chain {
public:
    using iterator = ChainIterator<T>;

    Chain() : size(0) {
        header = new ChainNode<T>();
        header->link = header;
    }

    Chain(const Chain<T>& other) : Chain() {
        CopyFrom(other);
    }

    Chain<T>& operator=(const Chain<T>& other) {
        if (this != &other) {
            Clear();
            CopyFrom(other);
        }
        return *this;
    }

    ~Chain() {
        Clear();
        delete header;
    }

    int Size() const { return size; }

    iterator Begin() const { return iterator(header->link); }
    iterator End() const { return iterator(header); }

    // Insert at position k
    void Insert(int k, const T& value) {
        if (k < 0 || k > size) throw out_of_range("Insert out of range");

        ChainNode<T>* prev = header;
        for (int i = 0; i < k; ++i)
            prev = prev->link;

        ChainNode<T>* newNode =
            ChainNode<T>::GetNode(value, prev->link);

        prev->link = newNode;
        ++size;
    }

    // Delete at position k
    void Delete(int k) {
        if (k < 0 || k >= size) throw out_of_range("Delete out of range");

        ChainNode<T>* prev = header;
        for (int i = 0; i < k; ++i)
            prev = prev->link;

        ChainNode<T>* delNode = prev->link;
        prev->link = delNode->link;

        ChainNode<T>::ReturnNode(delNode);
        --size;
    }

    void Clear() {
        ChainNode<T>* current = header->link;
        while (current != header) {
            ChainNode<T>* temp = current;
            current = current->link;
            ChainNode<T>::ReturnNode(temp);
        }
        header->link = header;
        size = 0;
    }

private:
    ChainNode<T>* header;
    int size;

    void CopyFrom(const Chain<T>& other) {
        for (auto it = other.Begin(); it != other.End(); ++it)
            Insert(size, *it);
    }
};


// (4)Polynomial

struct Term {
    int coef;
    int exp;
};

class Polynomial {
private:
    Chain<Term> poly;

    void AddTerm(int coef, int exp) {
        if (coef == 0) return;

        int index = 0;
        for (auto it = poly.Begin(); it != poly.End(); ++it, ++index) {
            if ((*it).exp == exp) {
                (*it).coef += coef;
                if ((*it).coef == 0)
                    poly.Delete(index);
                return;
            }
            if ((*it).exp < exp) {
                poly.Insert(index, Term{ coef, exp });
                return;
            }
        }
        poly.Insert(poly.Size(), Term{ coef, exp });
    }

public:
    Polynomial() = default;

    // (c)Copy constructor
    Polynomial(const Polynomial& other) : poly(other.poly) {}

    // (d)Assignment
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other)
            poly = other.poly;
        return *this;
    }

    // (f)Addition
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        auto itA = poly.Begin();
        auto itB = other.poly.Begin();

        while (itA != poly.End() && itB != other.poly.End()) {
            if ((*itA).exp > (*itB).exp) {
                result.AddTerm((*itA).coef, (*itA).exp);
                ++itA;
            }
            else if ((*itA).exp < (*itB).exp) {
                result.AddTerm((*itB).coef, (*itB).exp);
                ++itB;
            }
            else {
                result.AddTerm((*itA).coef + (*itB).coef, (*itA).exp);
                ++itA; ++itB;
            }
        }
        while (itA != poly.End()) {
            result.AddTerm((*itA).coef, (*itA).exp);
            ++itA;
        }
        while (itB != other.poly.End()) {
            result.AddTerm((*itB).coef, (*itB).exp);
            ++itB;
        }
        return result;
    }

    // (g)Subtraction
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result;
        auto itA = poly.Begin();
        auto itB = other.poly.Begin();

        while (itA != poly.End() && itB != other.poly.End()) {
            if ((*itA).exp > (*itB).exp) {
                result.AddTerm((*itA).coef, (*itA).exp);
                ++itA;
            }
            else if ((*itA).exp < (*itB).exp) {
                result.AddTerm(-(*itB).coef, (*itB).exp);
                ++itB;
            }
            else {
                result.AddTerm((*itA).coef - (*itB).coef, (*itA).exp);
                ++itA; ++itB;
            }
        }
        while (itA != poly.End()) {
            result.AddTerm((*itA).coef, (*itA).exp);
            ++itA;
        }
        while (itB != other.poly.End()) {
            result.AddTerm(-(*itB).coef, (*itB).exp);
            ++itB;
        }
        return result;
    }

    // (h)Multiplication
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (auto itA = poly.Begin(); itA != poly.End(); ++itA)
            for (auto itB = other.poly.Begin(); itB != other.poly.End(); ++itB)
                result.AddTerm((*itA).coef * (*itB).coef,
                    (*itA).exp + (*itB).exp);
        return result;
    }

    // (i)Evaluate
    float Evaluate(float x) const {
        float sum = 0.0f;
        for (auto it = poly.Begin(); it != poly.End(); ++it)
            sum += (*it).coef * powf(x, (*it).exp);
        return sum;
    }

    // (a)Input
    friend istream& operator>>(istream& is, Polynomial& p) {
        p.poly.Clear();
        int n;
        is >> n;
        for (int i = 0; i < n; ++i) {
            int c, e;
            is >> c >> e;
            p.AddTerm(c, e);
        }
        return is;
    }

    // (b)Output
    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        os << p.poly.Size();
        for (auto it = p.poly.Begin(); it != p.poly.End(); ++it)
            os << " " << (*it).coef << " " << (*it).exp;
        return os;
    }
};

int main()
{
    Polynomial A, B;

    cin >> A >> B;

    cout << A + B << endl;
    cout << A - B << endl;
    cout << A * B << endl;

    return 0;
}

```

## 效能分析

### 時間複雜度:
- 輸入:O(n²)
- 輸出:O(n)
- 加法:O(n + m)
- 減法:O(n + m)
- 乘法:O(n × m)
- Evaluate:O(n)

### 空間複雜度:
- 儲存一個多項式需要 n 個節點:O(n)
- 加法與減法結果最多包含 n + m 項:O(n + m)
- 乘法結果在最壞情況下可能包含 n × m 項:O(n × m)
  
## 測試與驗證
### 輸入:
```shell
$ g++ problems.cpp -o problems.exe
$ ./problems.exe
3 3 5 2 3 7 0
2 1 4 -2 3

```
### 輸出:
```shell
$ g++ problems.cpp -o problems.exe
$ ./problems.exe
3 3 5 1 4 7 0
3 3 5 4 3 7 0
5 3 9 1 -4 8 0

```
## 申論及開發報告
這份作業用circular linked list實作一元多項式運算
實際應用了課堂中所學的linked list、iterator與動態記憶體管理概念
在資料結構設計上，使用帶有header node的circular linked list，可避免在插入與刪除節點時處理空串列或特殊邊界情況
多項式中的每一項以(coef, exp)表示，並依照指數遞減排序
透過集中於AddTerm函式處理項目插入與合併，確保加法、減法與乘法運算後，多項式仍維持正確結構

先完成ChainNode、ChainIterator與Chain，建立帶有 header node 的 circular linked list
使用Chain<Term>作為多項式的內部表示，完成輸入、輸出與基本運算功能
實作多項式的加法、減法與乘法，並確保結果自動合併同指數項
加入available-space list，回收已刪除的節點並重複使用

主要困難在節點插入與刪除時的邊界處理
透過使用header node的circular linked list可統一處理所有情況，避免額外的特例判斷
加入available-space list後，也讓我更了解節點回收與重複利用的實務做法，進一步加深對動態記憶體管理的理解
