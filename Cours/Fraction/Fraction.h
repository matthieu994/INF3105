#ifndef FRACTION_H
#define FRACTION_H

class Fraction {
protected:
    int numerateur;
    int denominateur;

public:
    Fraction(int n = 0, int d = 0);
    Fraction operator+(const Fraction& F);
    friend ostream& operator<<(ostream& out, const Fraction& F);
};

#endif