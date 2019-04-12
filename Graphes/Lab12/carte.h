/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Lab12
    Fichier de départ carte.h © Éric Beaudry (beaudry.eric@uqam.ca).
    http://ericbeaudry.ca/INF3105/lab12/
*/
#if !defined(_CARTE__H_)
#define _CARTE__H_
#include <istream>
#include <list>
#include <set>
#include <map>
#include <string>
#include "pointst.h"

using namespace std;

class Carte
{
public:
  void ajouter_noeud(long id, const PointST &p);
  void ajouter_route(const std::string &nom, const std::list<long> &noeuds);

  set<long> trouver_noeuds_inaccessibles() const;

  // À compléter (vous pouvez tout changer).

private:
  struct Noeud
  {                    // Un noeud est un sommet dans le graphe
    mutable int num;
    mutable int min;
    mutable bool dansPile;
    PointST point;     // variable inutile pour le Lab12, mais utile pour le TP3.
    set<long> voisins; // ensemble des noeuds liés par un segment (tronçon) de route (arête).
  };
  map<long, Noeud> noeuds; // dictionnaire identifiant de noeud --> objet Noeud
};

// Déclaration opérateur (fonction) global
std::istream &operator>>(std::istream &is, Carte &carte);

#endif
