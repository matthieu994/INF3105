template <class C1, class C2>
class Pair {
public:
    C1 c1;
    C2 c2;
    Pair(C1 c1, C2 c2)
    {
        this->c1 = c1;
        this->c2 = c2;
    }
};

int main()
{
    Pair<int, double> p(2, 1.0);
    return 0;
}