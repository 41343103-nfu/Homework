# 41343103

## 解題說明

### 問題描述:
- 實作Polynomial類別
- 使用C++寫出函式,做出以下功能:
1.Input()：輸入多項式
2.Print()：輸出多項式
3.Add()：兩個多項式相加
4.Mult()：兩個多項式相乘
5.Eval()：代入某個x值計算結果

### 解題策略:
- 先設計一個Term類別,用來儲存多項式的係數與指數
- 再用Polynomial類別把Term組合起來
- 主要是把數學運算轉換成程式邏輯
- 例如加法的兩項次方相同,則係數相加,如果不同,則將指數大的項直接加入結果

## 程式實作
```cpp
#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend Polynomial;
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term term[100];
    int terms;
public:
    Polynomial() { terms = 0; }

    void Input() {
        cout << "請輸入項數:\n";
        cin >> terms;
        cout << "請輸入每一項的係數與次方:\n";
        for (int i = 0; i < terms; i++) {
            cout << "第 " << i + 1 << " 項: ";
            cin >> term[i].coef >> term[i].exp;
        }
    }

    void Print() {
        cout << "多項式: ";
        for (int i = 0; i < terms; i++) {
            if (i > 0 && term[i].coef > 0) cout << " + ";
            if (i > 0 && term[i].coef < 0) {
                cout << " - "; term[i].coef = -term[i].coef;
            }
            cout << term[i].coef << "x^" << term[i].exp;
        }
        cout << endl;
    }

    Polynomial Add(Polynomial p) {
        Polynomial result;
        int i = 0, j = 0;
        while (i < terms && j < p.terms) {
            if (term[i].exp == p.term[j].exp) {
                float sum = term[i].coef + p.term[j].coef;
                if (sum != 0) {
                    result.term[result.terms].coef = sum;
                    result.term[result.terms].exp = term[i].exp;
                    result.terms++;
                }
                i++; j++;
            } else if (term[i].exp > p.term[j].exp) {
                result.term[result.terms++] = term[i++];
            } else {
                result.term[result.terms++] = p.term[j++];
            }
        }
        while (i < terms) result.term[result.terms++] = term[i++];
        while (j < p.terms) result.term[result.terms++] = p.term[j++];
        return result;
    }

    Polynomial Mult(Polynomial p) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < p.terms; j++) {
                float newCoef = term[i].coef * p.term[j].coef;
                int newExp = term[i].exp + p.term[j].exp;
                bool found = false;
                for (int k = 0; k < result.terms; k++) {
                    if (result.term[k].exp == newExp) {
                        result.term[k].coef += newCoef;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    result.term[result.terms].coef = newCoef;
                    result.term[result.terms].exp = newExp;
                    result.terms++;
                }
            }
        }
        return result;
    }

    float Eval(float x) {
        float sum = 0;
        for (int i = 0; i < terms; i++) {
            float value = 1;
            for (int j = 0; j < term[i].exp; j++)
                value *= x;
            sum += term[i].coef * value;
        }
        return sum;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式:\n";
    p1.Input();
    cout << "輸入第二個多項式:\n";
    p2.Input();

    cout << "\n第一個: "; p1.Print();
    cout << "第二個: "; p2.Print();

    Polynomial sum = p1.Add(p2);
    cout << "\n相加結果: "; sum.Print();

    Polynomial product = p1.Mult(p2);
    cout << "相乘結果: "; product.Print();

    float x;
    cout << "\n請輸入要代入的 x 值: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;
    return 0;
}
```

## 效能分析

### 時間複雜度:
- Add():O(n + m)
- Mult():O(n × m)
- Eval():O(n × e)
   
### 空間複雜度:
- Term term[100]: O(n)
  
## 測試與驗證
### 輸入:
```shell
$ g++ problems.cpp -o problems.exe
$ ./problems.exe
輸入第一個多項式：
3
3 2
2 1
1 0
輸入第二個多項式：
2
1 1
1 0
請輸入要代入的 x 值: 2
```
### 輸出:
```shell
$ g++ problems.cpp -o problems.exe
$ ./problems.exe
第一個: 3x^2 + 2x^1 + 1x^0
第二個: 1x^1 + 1x^0
相加結果: 3x^2 + 3x^1 + 2x^0
相乘結果: 3x^3 + 5x^2 + 3x^1 + 1x^0

請輸入要代入的 x 值: 2
p1(2) = 17
```
## 申論及開發報告
這份作業讓我更清楚如何把數學運算的邏輯轉換成程式的邏輯,像這次是多項式的計算,在數學中只需要看指數去相加係數,而程式還要考慮如何儲存每一項係數與指數,學會分析問題,一步步用程式模擬手算的過程
