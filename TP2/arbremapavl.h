/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreMapAVL<K, V> pour le Lab6 et le TP2.

   AUTEUR:
    1) Matthieu PETIT : PETM09089908
*/

#if !defined(__ARBREMapAVL_H__)
#define __ARBREMapAVL_H__
#include <iostream>
#include <cassert>
#include "pile.h"

template <class K, class V>
class ArbreMapAVL
{
  public:
    ArbreMapAVL();
    ArbreMapAVL(const ArbreMapAVL &);
    ~ArbreMapAVL();

    // Annonce l'existence d'une classe Iterateur.
    class Iterateur;

    void inserer(const K &cle, const V &valeur);
    bool contient(const K &cle) const;
    bool vide() const;
    void vider();
    void afficher() const;
    // void enlever(const K &); // non requis pour le TP2.
    int hauteur() const;

    // Fonctions pour obtenir un itérateur (position dans l'arbre)
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const K &) const;
    Iterateur rechercherEgalOuSuivant(const K &) const;
    Iterateur rechercherEgalOuPrecedent(const K &) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const V &operator[](const K &c) const;
    const K &operator[](const Iterateur &) const;
    K &operator[](const Iterateur &);

    // Copie d'un arbre AVL.
    ArbreMapAVL &operator=(const ArbreMapAVL &);

  private:
    struct Noeud
    {
        Noeud(const K &cle_, const V &valeur_);
        K cle;
        V valeur;
        int equilibre;
        Noeud *gauche, *droite;
        friend class ArbreMapAVL;
    };
    Noeud *racine;

    // Fonctions internes
    bool inserer(Noeud *&noeud, const K &cle, const V &valeur);
    void rotationGaucheDroite(Noeud *&);
    void rotationDroiteGauche(Noeud *&);
    void vider(Noeud *&);
    void copier(const Noeud *, Noeud *&) const;
    const K &max(Noeud *) const;
    void afficher(Noeud *courant, int profondeur = 0, int etat = 0) const;
    // bool enlever(Noeud *&, const K &e);

  public:
    // Sera présenté à la semaine #7
    class Iterateur
    {
      public:
        Iterateur(const ArbreMapAVL &a);
        Iterateur(const Iterateur &a);
        Iterateur(const ArbreMapAVL &a, Noeud *c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur &) const;
        bool operator!=(const Iterateur &) const;

        const K &operator*() const;

        Iterateur &operator++();
        Iterateur operator++(int);
        Iterateur &operator=(const Iterateur &);

      private:
        const ArbreMapAVL &arbre_associe;
        Noeud *courant;
        Pile<Noeud *> chemin;

        friend class ArbreMapAVL;
    };
};

//-----------------------------------------------------------------------------

template <class K, class V>
ArbreMapAVL<K, V>::Noeud::Noeud(const K &c, const V &v)
    : cle(c), valeur(v), equilibre(0), gauche(NULL), droite(NULL)
{
}

template <class K, class V>
ArbreMapAVL<K, V>::ArbreMapAVL()
    : racine(NULL)
{
}

template <class K, class V>
ArbreMapAVL<K, V>::ArbreMapAVL(const ArbreMapAVL<K, V> &autre)
    : racine(NULL)
{
    copier(autre.racine, racine);
}

template <class K, class V>
ArbreMapAVL<K, V>::~ArbreMapAVL()
{
    vider(racine);
}

template <class K, class V>
bool ArbreMapAVL<K, V>::contient(const K &cle) const
{
    Noeud *racineTemp = racine;

    while (racineTemp != nullptr && racineTemp->cle != cle)
    {
        if (racineTemp->cle > cle)
            racineTemp = racineTemp->gauche;
        else
            racineTemp = racineTemp->droite;
    }

    if (racineTemp == nullptr)
        return false;
    else
        return true;
}

template <class K, class V>
void ArbreMapAVL<K, V>::inserer(const K &cle, const V &valeur)
{
    inserer(racine, cle, valeur);
}

template <class K, class V>
bool ArbreMapAVL<K, V>::inserer(Noeud *&noeud, const K &cle, const V &valeur)
{
    if (noeud == NULL)
    {
        noeud = new Noeud(cle, valeur);
        return true;
    }
    if (cle < noeud->cle)
    {
        if (inserer(noeud->gauche, cle, valeur))
        {
            noeud->equilibre++;
            if (noeud->equilibre == 0)
                return false;
            if (noeud->equilibre == 1)
                return true;
            assert(noeud->equilibre == 2);
            if (noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else if (cle > noeud->cle)
    {
        if (inserer(noeud->droite, cle, valeur))
        {
            noeud->equilibre--;
            if (noeud->equilibre == 0)
                return false;
            if (noeud->equilibre == -1)
                return true;
            assert(noeud->equilibre == -2);
            if (noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }

    // cle == noeud->cle
    noeud->cle = cle;
    noeud->valeur = valeur;
    return false;
}

template <class K, class V>
void ArbreMapAVL<K, V>::rotationGaucheDroite(Noeud *&racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int ea = temp->equilibre;
    int eb = racinesousarbre->equilibre;
    int neb = -(ea > 0 ? ea : 0) - 1 + eb;
    int nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class K, class V>
void ArbreMapAVL<K, V>::rotationDroiteGauche(Noeud *&racinesousarbre)
{
    Noeud *temp = racinesousarbre->droite;
    int ea = temp->equilibre;
    int eb = racinesousarbre->equilibre;
    int neb = -(ea < 0 ? ea : 0) + 1 + eb;
    int nea = ea + (neb > 0 ? neb : 0) + 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

template <class K, class V>
void ArbreMapAVL<K, V>::afficher() const
{
    printf("\n");
    if (racine != nullptr)
        afficher(racine);
    else
        printf("Vide");
    printf("\n");
}

template <class K, class V>
void ArbreMapAVL<K, V>::afficher(Noeud *courant, int profondeur, int etat) const
{
    if (courant->droite)
        afficher(courant->droite, profondeur + 1, 1);

    for (int i = 0; i < profondeur; i++)
        printf("          ");

    if (etat == 1) // gauche
        std::cout << "┌───────";
    else if (etat == 2) // droite
        std::cout << "└───────";

    std::cout << courant->cle << std::endl;

    if (courant->gauche)
        afficher(courant->gauche, profondeur + 1, 2);
}

template <class K, class V>
bool ArbreMapAVL<K, V>::vide() const
{
    return racine == nullptr;
}

template <class K, class V>
void ArbreMapAVL<K, V>::vider()
{
    vider(racine);
}

template <class K, class V>
void ArbreMapAVL<K, V>::vider(Noeud *&noeud)
{
    if (noeud == nullptr)
        return;

    vider(noeud->gauche);
    vider(noeud->droite);

    delete noeud;
}

template <class K, class V>
void ArbreMapAVL<K, V>::copier(const Noeud *source, Noeud *&noeud) const
{
    if (source == nullptr)
    {
        noeud = nullptr;
        return;
    }

    noeud = new Noeud(source->cle, source->valeur);
    noeud->equilibre = source->equilibre;

    if (source->gauche != nullptr)
        copier(source->gauche, noeud->gauche);

    if (source->droite != nullptr)
        copier(source->droite, noeud->droite);
}

template <class K, class V>
int ArbreMapAVL<K, V>::hauteur() const
{
    // À compléter.
    return 0;
}

template <class K, class V>
const K &ArbreMapAVL<K, V>::max(Noeud *n) const
{
    Noeud *max = n;
    while (max->droite != nullptr)
        max = max->droite;
    return max->cle;
}

//----------------------- OPERATEURS ----------------------------//
template <class K, class V>
const V &ArbreMapAVL<K, V>::operator[](const K &c) const
{
    return rechercher(c).courant->valeur;
}

template <class K, class V>
const K &ArbreMapAVL<K, V>::operator[](const Iterateur &iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->cle;
}

template <class K, class V>
K &ArbreMapAVL<K, V>::operator[](const Iterateur &iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->cle;
}

template <class K, class V>
ArbreMapAVL<K, V> &ArbreMapAVL<K, V>::operator=(const ArbreMapAVL &autre)
{
    if (this == &autre)
        return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

//----------------------- ITERATEURS ----------------------------//
template <class K, class V>
ArbreMapAVL<K, V>::Iterateur::Iterateur(const ArbreMapAVL &a)
    : arbre_associe(a), courant(NULL)
{
}

template <class K, class V>
ArbreMapAVL<K, V>::Iterateur::Iterateur(const ArbreMapAVL<K, V>::Iterateur &a)
    : arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur &ArbreMapAVL<K, V>::Iterateur::operator++()
{
    // À compléter.
    return *this;
}

// Post-incrément
template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur ArbreMapAVL<K, V>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class K, class V>
ArbreMapAVL<K, V>::Iterateur::operator bool() const
{
    return courant != NULL;
}

template <class K, class V>
bool ArbreMapAVL<K, V>::Iterateur::operator!() const
{
    return courant == NULL;
}

template <class K, class V>
bool ArbreMapAVL<K, V>::Iterateur::operator==(const Iterateur &o) const
{
    assert(&arbre_associe == &o.arbre_associe);
    return courant == o.courant;
}

template <class K, class V>
bool ArbreMapAVL<K, V>::Iterateur::operator!=(const Iterateur &o) const
{
    assert(&arbre_associe == &o.arbre_associe);
    return courant != o.courant;
}

template <class K, class V>
const K &ArbreMapAVL<K, V>::Iterateur::operator*() const
{
    assert(courant != NULL);
    return courant->cle;
}

template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur &ArbreMapAVL<K, V>::Iterateur::operator=(const Iterateur &autre)
{
    assert(&arbre_associe == &autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

//----------------------- ITERATEURS ----------------------------//
template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur ArbreMapAVL<K, V>::debut() const
{
    Iterateur iter(*this);
    iter.courant = racine;
    if (iter.courant != NULL)
        while (iter.courant->gauche != NULL)
        {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        }
    return iter;
}

template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur ArbreMapAVL<K, V>::fin() const
{
    return Iterateur(*this);
}

template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur ArbreMapAVL<K, V>::rechercher(const K &e) const
{
    Iterateur iter(*this);
    Noeud *n = racine;
    while (n)
    {
        if (e < n->cle)
        {
            iter.chemin.empiler(n);
            n = n->gauche;
        }
        else if (n->cle < e)
            n = n->droite;
        else
        {
            iter.courant = n;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}

template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur ArbreMapAVL<K, V>::rechercherEgalOuSuivant(const K &e) const
{
    Iterateur iter(*this);
    // À compléter.
    return iter;
}

template <class K, class V>
typename ArbreMapAVL<K, V>::Iterateur ArbreMapAVL<K, V>::rechercherEgalOuPrecedent(const K &e) const
{
    // À compléter.
    return Iterateur(*this);
}

#endif