#include <iostream>
using namespace std;

int main()
{
    int a = 1, b = 2, c = 3, d = 4;
    int* pa = &a;
    int* pb = &b;
    int* pc = &c;
    int* pd = &d;
    cout << "a=" << a << "\tb=" << b << "\tc=" << c << "\td=" << d << endl;
    cout << "pa=" << pa << "\tpb=" << pb << "\tpc=" << pc << "\tpd=" << pd << endl;
    *pa = 4;
    cout << "a=" << a << "\tb=" << b << "\tc=" << c << "\td=" << d << endl;
    cout << "pa=" << pa << "\tpb=" << pb << "\tpc=" << pc << "\tpd=" << pd << endl;
    pa = pb;
    *pa = 8;
    cout << "a=" << a << "\tb=" << b << "\tc=" << c << "\td=" << d << endl;
    cout << "pa=" << pa << "\tpb=" << pb << "\tpc=" << pc << "\tpd=" << pd << endl;
    c = 10;
    d += *pd;
    cout << "a=" << a << "\tb=" << b << "\tc=" << c << "\td=" << d << endl;
    cout << "pa=" << pa << "\tpb=" << pb << "\tpc=" << pc << "\tpd=" << pd << endl;
    return 0;
}