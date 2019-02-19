#include <iostream>

using namespace std;

class Forme2D {
private:
protected:
    double centreX;
    double centreY;
    int* p;

public:
    Forme2D(double x = 0.0, double y = 0.0)
        : centreX(x)
        , centreY(y)
    {
        p = new int();
    }

    virtual double aire()
    {
        return 0.0;
    }

    Forme2D(const Forme2D& autre_forme)
        : centreX(autre_forme.centreX)
        , centreY(autre_forme.centreY)
    {
    }

    ~Forme2D()
    {
        delete p;
    }
};

class Cercle : public Forme2D {
private:
protected:
    double _rayon;

public:
    Cercle(double rayon, double centre_x = 0.0, double centre_y = 0.0)
        : Forme2D(centre_x, centre_y)
        , _rayon(rayon)
    {
    }

    Cercle(const Cercle& autre_cercle)
        : Forme2D(autre_cercle.centreX, autre_cercle.centreY)
        , _rayon(autre_cercle._rayon)
    {
    }

    double aire()
    {
        return 3.1415926 * _rayon * _rayon;
    }

    ~Cercle()
    {
        delete p;
    }
};