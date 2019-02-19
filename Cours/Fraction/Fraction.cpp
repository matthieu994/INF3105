#include "Fraction.h"
#include <iostream>

using namespace std;

Fraction::Fraction(int n, int d)
{
    numerateur = n;
    this->denominateur = d;
}

Fraction::Fraction operator+(const Fraction& F)
    {
        int n = numerateur * F.denominateur + denominateur * F.numerateur;
        int d = denominateur * F.denominateur;
        return Fraction(n, d);
    }

ostream& operator<<(ostream& out, const Fraction& F)
{
    cout << F.numerateur << "/" << F.denominateur;
    return out;
}

int main()
{
    Fraction F1(2, 5);
    Fraction F2(2, 5);
    cout << (F1 + F2) << endl;
    return 0;
}