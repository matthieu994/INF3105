/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
*/

#if !defined(__PILE_H__)
#define __PILE_H__

template <class T>
class Pile {
public:
    Pile();
    Pile(const Pile&);
    ~Pile();

    Pile<T>& operator=(const Pile<T>&);

    void empiler(const T&);

    void depiler(T& out);
    T depiler();

    bool vide() const { return sommet == NULL; }
    void vider();

private:
    class Cellule {
    public:
        Cellule(const T& e, Cellule* c);
        Cellule(const T& e);
        Cellule(Cellule*);
        T contenu;
        Cellule* suivante;
    };

    Cellule* sommet;
};

/*
 Puisque Pile<T> est une classe générique, toutes ses définitions doivent être
 inclues après les déclarations. On peut quand même séparer les déclarations et
 les définitions en deux fichiers (pile.h et pile.hcc), à condition d'inclure le
 deuxième (pile.hcc) à la fin du premier (pile.h). Ainsi, un fichier source
 (.cc, .cpp, c++) qui inclut pile.h inclura indirectement aussi pile.hcc.

*/

//#include "pile.hcc"
/**** Début des définitions pouvant être mises dans pile.hcc. ****/

// Les deux directives suivante sont utiles lorsqu'un fichier pile.hcc est utilisé.
#if !defined(__PILE_CPP__)
#define __PILE_CPP__

#include "assert.h"
#include "pile.h"

template <class T>
Pile<T>::Pile()
{
    sommet = NULL;
}

template <class T>
Pile<T>::Pile(const Pile& autre)
{
    Cellule* sommet_iterator;
    sommet_iterator = autre.sommet;
    // vider();
    sommet = NULL;
    while (sommet_iterator != NULL) {
        empiler(sommet_iterator->contenu);
        sommet_iterator = sommet_iterator->suivante;
    }
}

template <class T>
Pile<T>::~Pile()
{
    vider();
}

template <class T>
void Pile<T>::vider()
{
    while (!vide())
        depiler();
}

template <class T>
Pile<T>::Cellule::Cellule(const T& e, Cellule* c)
    : suivante(c)
{
    contenu = e;
}

template <class T>
Pile<T>::Cellule::Cellule(Cellule* c)
{
    suivante = c->suivante;
    contenu = c->contenu;
}

template <class T>
Pile<T>::Cellule::Cellule(const T& e)
{
    suivante = NULL;
    contenu = e;
}

template <class T>
void Pile<T>::empiler(const T& e)
{
    sommet = new Cellule(e, sommet);
    // std::cerr << "xxxxx" << std::endl;
    assert(sommet);
}

template <class T>
T Pile<T>::depiler()
{
    assert(sommet);
    Cellule c(*sommet);
    delete sommet;
    sommet = c.suivante;
    return c.contenu;
}

template <class T>
void Pile<T>::depiler(T& e)
{
    assert(sommet != NULL);
    e = sommet->contenu;
    Cellule* c = sommet;
    sommet = sommet->suivante;
    delete c;
}

template <class T>
Pile<T>& Pile<T>::operator=(const Pile<T>& autre)
{
    vider();
    if (autre.sommet != NULL) {
        sommet = new Cellule(autre.sommet->contenu);
        Cellule* cp = autre.sommet;
        Cellule* c = sommet;
        while (cp->suivante != NULL) {
            c->suivante = new Cellule(cp->suivante->contenu);
            c = c->suivante;
            cp = cp->suivante;
        }
    }
    return *this;
}

#endif
// Les deux directives suivante sont utiles lorsqu'un fichier pile.hcc est utilisé.

#endif
