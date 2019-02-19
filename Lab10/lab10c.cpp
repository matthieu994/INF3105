#include <algorithm>
#include <iostream>
#include <set>
#include <string>
using namespace std;

int main()
{
    set<string> ensemble;
    ensemble.insert("un");
    ensemble.insert("deux");
    ensemble.insert("trois");
    ensemble.insert("quatre");
    ensemble.insert("cinq");

    for (set<string>::iterator iter = ensemble.begin(); iter != ensemble.end(); ++iter)
        std::cout << *iter << std::endl;

    return 0;
}