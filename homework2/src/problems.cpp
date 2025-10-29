#include <iostream>
using namespace std;

class Polynomial;

class Term
{
    friend Polynomial;
    private:
        float coef;
        int exp;
};

class Polynomial
{
    private:
        Term term[100];
        int terms;
    public:
        Polynomial()
        {
            terms = 0;
        }

    void Input()
    {
        cout <<"�п�J����:\n";
        cin >> terms;
        cout <<"�п�J�C�@�����Y�ƻP����:\n";
        for (int i = 0; i < terms; i++)
        {
            cout <<"�� "<< i + 1 <<" ��:";
            cin>>term[i].coef>>term[i].exp;
        }
    }

    void Print()
    {
        cout <<"�h����:";
        for (int i = 0; i < terms; i++)
        {
            if (i > 0 && term[i].coef > 0)
                cout <<" + ";
            if (i > 0 && term[i].coef < 0)
            {
                cout <<" - ", term[i].coef = -term[i].coef;
            }
            cout << term[i].coef <<"x^"<< term[i].exp;
        }
        cout << endl;
    }

    Polynomial Add(Polynomial p)
    {
        Polynomial result;

        int i = 0, j = 0;

        while (i < terms && j < p.terms)
        {
            if (term[i].exp == p.term[j].exp)
            {
                float sum = term[i].coef + p.term[j].coef;
                if (sum != 0)
                {
                    result.term[result.terms].coef = sum;
                    result.term[result.terms].exp = term[i].exp;
                    result.terms++;
                }
                i++;
                j++;
            }
            else if (term[i].exp > p.term[j].exp)
            {
                result.term[result.terms] = term[i];
                result.terms++;
                i++;
            }
            else
            {
                result.term[result.terms] = p.term[j];
                result.terms++;
                j++;
            }
        }
        while (i < terms) result.term[result.terms++] = term[i++];
        while (j < p.terms) result.term[result.terms++] = p.term[j++];

        return result;
    }

    Polynomial Mult(Polynomial p)
    {
        Polynomial result;
        for (int i = 0; i < terms; i++)
        {
            for (int j = 0; j < p.terms; j++)
            {
                float newCoef = term[i].coef * p.term[j].coef;
                int newExp = term[i].exp + p.term[j].exp;
                bool found = false;

                for (int k = 0; k < result.terms; k++)
                {
                    if (result.term[k].exp == newExp)
                    {
                        result.term[k].coef += newCoef;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    result.term[result.terms].coef = newCoef;
                    result.term[result.terms].exp = newExp;
                    result.terms++;
                }
            }
        }
        return result;
    }

    float Eval(float x)
    {
        float sum = 0;
        for (int i = 0; i < terms; i++)
        {
            float value = 1;
            for (int j = 0; j < term[i].exp; j++)
            {
                value *= x;
            }
            sum += term[i].coef * value;
        }
        return sum;
    }
};

int main()
{
    Polynomial p1, p2;

    cout <<"��J�Ĥ@�Ӧh����:\n";
    p1.Input();
    cout <<"��J�ĤG�Ӧh����:\n";
    p2.Input();

    cout <<"\n�Ĥ@��:";
    p1.Print();
    cout <<"�ĤG��:";
    p2.Print();

    Polynomial sum = p1.Add(p2);
    cout <<"\n�ۥ[���G:";
    sum.Print();

    Polynomial product = p1.Mult(p2);
    cout <<"�ۭ����G:";
    product.Print();

    float x;
    cout <<"\n�п�J�n�N�J�� x ��:";
    cin >> x;
    cout <<"p1( "<< x <<" ) = "<< p1.Eval(x) << endl;

    return 0;
}