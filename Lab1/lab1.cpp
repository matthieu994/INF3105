/*
  UQAM / Département d'informatique
  INF3105
  Laboratoire 1
*/

#include "point.h"
#include <iostream>
#include <limits>
using namespace std;

/* 
   Allez-y étape par étape. C'est la seule façon de ne pas introduire de bogue!

   Étape 1 : Compiler ce sans modification.
   Étape 2 : Enlever la fonction main() et renommer main2() ==> main()
   Étape 2 : Enlever la fonction main2() et renommer main3() ==> main()
   ...

 */

// int main(int argc, const char** args)
// {
//   std::cout << "Bienvenue au cours INF3105 en C++ !" << std::endl;
//   return 0;
// }
// int main2(int argc, const char** args)
// {
//   Point point(3, 105);
//   std::cout << point;
//   std::cout << std::endl; // fin de ligne
//   return 0;
// }

// int main(int argc, const char** args) // 3
// {
//   Point point(1.2,5.7);
//   std::cout << "point.x=" << point.x << "\tpoint.y=" << point.y << std::endl;
//   return 0;
// }

// int main(int argc, const char** args) // 4
// {
//   Point point;
//   std::cout << "Entrez un point:" << std::endl;
//   std::cin >> point;
//   std::cout << "Revoici votre point:" << std::endl;
//   std::cout << point << std::endl;
//   return 0;
// }

// int main(int argc, const char** args) // 5
// {
//   Point a(0,0);
//   Point b(4,3);
//   double d = a.distance(b);
//   std::cout << "Distance("<< a << "," << b << ")=" << d << std::endl;
//   return 0;
// }

int main(int argc, const char** args) // 6
{
    int nb;
    std::cin >> nb;

    Point points[nb];
    for (int i = 0; i < nb; i++)
        std::cin >> points[i];

    double min = numeric_limits<double>::max(), value;
    for (int i = 0; i < nb; ++i) {
        for (int j = i + 1; j < nb; ++j) {
            value = points[i].distance(points[j]);
            if (value < min) {
                min = value;
            }
        }
    }
    cout << "Min: " << min << endl;

    return 0;
}
