#include <iostream>

class Allo {
public:
    Allo(int x_ = 0)
        : x(x_)
    {
        std::cout << "A" << x << " ";
    }
    Allo(const Allo& autre)
        : x(autre.x)
    {
        std::cout << "B" << x << " ";
    }
    ~Allo()
    {
        std::cout << "C" << x << " ";
    }

    int x;
};

void f1(Allo a1, Allo& a2, Allo* a3)
{
    a1.x++;
    a2.x--;
    a3 += 1;
    (a3->x) += 2000;
}

int main()
{
    Allo tab[3];
    tab[0].x = 1;
    Allo a(20);
    Allo* b = new Allo(5);
    Allo* c = tab + 1;
    f1(a, *b, c);
    std::cout << std::endl
              << "-------" << std::endl;
    Allo* t = new Allo[4];
    t[2] = Allo(9);
    std::cout << std::endl;
}