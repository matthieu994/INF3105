#include <algorithm>
#include <iostream>
#include <list>
#include <set>

using namespace std;

class Combinaison {
public:
private:
    set<int> numeros;
    friend istream& operator>>(istream& is, Combinaison& c);
    friend bool operator<(const Combinaison& c1, const Combinaison& c2);
};

bool operator<(const Combinaison& c1, const Combinaison& c2)
{
    // auto n1 = c1.numeros.cbegin();
    // auto n2 = c2.numeros.cbegin();

    // for (; n1 != c1.numeros.end() && n2 != c2.numeros.end(); ++n1, ++n2) {
    //     if (*n1 < *n2)
    //         return false;
    // }
    // return true;

    return c1.numeros < c2.numeros;
}

istream& operator>>(istream& is, Combinaison& c)
{
    int nombre;
    for (int i = 0; i < 6; i++) {
        is >> nombre;
        c.numeros.insert(nombre);
    }
    return is;
}

int main()
{
    set<Combinaison> mises;
    while (cin) {
        Combinaison combinaison_choisie;
        cin >> combinaison_choisie;
        if (mises.find(combinaison_choisie) != mises.end())
            cout << "La combinaison est refusee!" << endl;
        else {
            mises.insert(combinaison_choisie);
            cout << "Combinaison insérée !" << endl;
        }
    }
}