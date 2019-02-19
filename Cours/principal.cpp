
#include <iomanip>
#include <iostream>

using namespace std;

class Forme2D {
private:
protected:
    double _centre_x;
    double _centre_y;

public:
    Forme2D(double centre_x = 0.0,
        double centre_y = 0.0)
        : _centre_x(centre_x)
        , _centre_y(centre_y)
    {
    }
    Forme2D(const Forme2D& copie)
        : _centre_x(copie._centre_x)
        , _centre_y(copie._centre_y)
    {
    }
    virtual ~Forme2D()
    {
    }
    virtual double aire()
    {
        return 0.0;
    }
    // virtuelle pure == abstract
    virtual double perimetre() = 0;
};

class Cercle : public Forme2D {
private:
protected:
    double _rayon;

public:
    Cercle(double rayon = 3.0,
        double centre_x = 0.0,
        double centre_y = 0.0)
        : Forme2D(centre_x, centre_y)
        , _rayon(rayon)
    {
    }
    Cercle(const Cercle& copie)
        : Forme2D(copie)
        , _rayon(copie._rayon)
    {
    }
    virtual ~Cercle()
    {
    }
    double aire()
    {
        return 3.1415926 * _rayon * _rayon;
    }
    double perimetre()
    {
        return 2.0 * 3.1415926 * _rayon;
    }
};

int main(int argc, char** argv)
{
    Cercle c1;
    Cercle c2(4.0, 2.0);
    Cercle c3(c1);

    Forme2D* f = new Cercle(4.5);
    f->aire();

    Forme2D* f2;
    Cercle c(3.0);
    f2 = &c;
    cout << f2->perimetre() << endl;
}
