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
