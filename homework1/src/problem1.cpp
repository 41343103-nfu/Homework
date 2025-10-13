#include<iostream>
using namespace std;

int A(int m, int n)
{
	if (m == 0)
		return n + 1;
	else if (n == 0)
		return A(m - 1, 1);
	else
		return A(m - 1, A(m, n - 1));
	}

int Ac(int m, int n)
{
    const int MAX = 100000;
    int stack[MAX];
    int top = -1;
    stack[++top] = m;

    while (top >= 0)
    {
        m = stack[top--];
        if (m == 0)
        {
            n = n + 1;
        }
        else if (n == 0)
        {
            n = 1;
            stack[++top] = m - 1;
        }
        else
        {
            stack[++top] = m - 1;
            stack[++top] = m;
            n = n - 1;
        }
    }
    return n;
}
int main()
{
	int m, n;

	cout << "¿é¤J m,n :" << endl;
	cin >> m >> n;
	cout << "A(" << m << "," << n << ")=" << A(m, n) << endl;

    cout << "A(" << m << "," << n << ")=" << Ac(m, n) << endl;

	return 0;
}
