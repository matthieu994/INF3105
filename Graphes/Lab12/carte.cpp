/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Lab12
    Fichier de départ carte.cpp © Éric Beaudry (beaudry.eric@uqam.ca).
    http://ericbeaudry.ca/INF3105/lab12/ */

#include "carte.h"

void Carte::ajouter_noeud(long id, const PointST &p)
{
    Noeud noeud;
    noeud.point = p;
    noeuds[id] = noeud;
}

void Carte::ajouter_route(const std::string &nomroute, const std::list<long> &listeNoeuds)
{
    auto it2 = listeNoeuds.begin();
    ++it2;

    for (auto it = listeNoeuds.begin(); it2 != listeNoeuds.end(); ++it, ++it2)
    {
        noeuds.at(*it).voisins.insert(*it2);
    }
}

std::set<long> Carte::trouver_noeuds_inaccessibles() const
{
    // À compléter.
    std::set<long> result;
    return result;
}
