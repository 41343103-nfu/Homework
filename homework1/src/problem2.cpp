#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int n;
    char S[20];

    cout << "¿é¤J¶°¦X¤¸¯À­Ó¼Æ n: ";
    cin >> n;

    cout << "¿é¤J " << n << " ­Ó¤¸¯À (¦r¤¸): ";
    for (int i = 0; i < n; i++)
        cin >> S[i];

    cout << "Powerset(S):" << endl;

    int total = pow(2, n);

    for (int i = 0; i < total; i++)
    {
        cout << "{";

        bool first = true;
        for (int j = 0; j < n; j++)
        {
            if (i & (1 << j))
            {
                if (!first) cout << ", ";
                cout << S[j];
                first = false;
            }
        }

        cout << "}" << endl;
    }

    return 0;
}
