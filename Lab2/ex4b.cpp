#include "Point.h"
using namespace std;
void test(Point p1, Point& p2, const Point& p3, Point* p4)
{
    cout << p1 << endl;
    p1 = p2;
    *p4 = p3;
    p4 = &p1;
}
int main()
{
    Point a, b(3, 4), c(0, 5);
    Point* d = new Point(5, 0);
    test(a, b, c, d);
    cout << a << '\t' << b << '\t' << c << '\t' << *d << endl;
    delete d;
    return 0;
}