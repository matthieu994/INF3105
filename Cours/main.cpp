#include "Forme2D.cpp"

int main()
{
    Forme2D* f1 = new Forme2D(30.0);
    Forme2D* f2 = new Forme2D(*f1);
    // cout << f2->aire() << endl;

    Cercle* c1 = new Cercle(2.0);
    Cercle* c2 = new Cercle(*c1);
    // cout << c2->aire() << endl;

    Forme2D f3(2.0, 3.0);
    Cercle c3(2.0);
    f3 = c3;
    cout << f3.aire() << endl;
}