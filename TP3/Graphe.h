/*  INF3105 - Structures de données et algorithmes
    Lab 11 -- Graphes / Représentations et algorithmes de base
    http://ericbeaudry.uqam.ca/INF3105/lab11/
*/

#ifndef GRAPHE_H_
#define GRAPHE_H_

#include "Histoire.h"

#include <queue>
#include <map>
#include <set>
#include <iostream>

using namespace std;

// Le type S est le type utilisé pour identifier les sommets
template <class S>
class Graphe
{
public:
  // Interface public pour créer le graphe.
  void ajouterSommet(const S &s);
  void ajouterAreteOrientee(const S &s1, const S &s2);
  void ajouterAreteNonOrientee(const S &s1, const S &s2);
  void construireGraphe(Histoire *histoire);
  double trouverChemin(int profondeur, Phrase depart, Phrase arrivee, map<int, Phrase> &chemin);

  // Interface public pour les requêtes de base.
  void parcoursRechercheProfondeur(const S &s) const;
  void parcoursProfondeurRec(const S &s) const;
  void parcoursRechercheLargeur(const S &s) const;
  void parcoursLargeurRec(const S &s) const;
  void extraireComposantesConnexes() const;
  void resetVisite() const;
  bool estVoisin(const Phrase &sommet, const Phrase &voisin) const;

private:
  struct Sommet
  {
    mutable bool estVisite;
    set<S> voisins; // ensemble des sommets accessibles via les arêtes sortantes du sommet.
  };
  map<S, Sommet> sommets; // identification --> sommet
};

template <class S>
void Graphe<S>::ajouterSommet(const S &s)
{
  sommets[s] = Sommet();
}

// Doit ajouter les 2 arêtes orientées : s1->s2 et s2->s1;
template <class S>
void Graphe<S>::ajouterAreteNonOrientee(const S &s1, const S &s2)
{
  ajouterAreteOrientee(s1, s2);
  ajouterAreteOrientee(s2, s1);
}

// Doit uniquement ajouter s1->s2.
template <class S>
void Graphe<S>::ajouterAreteOrientee(const S &s1, const S &s2)
{
  sommets[s1].voisins.insert(s2);
}

template <class S>
void Graphe<S>::parcoursRechercheProfondeur(const S &s) const
{
  resetVisite();
  parcoursProfondeurRec(s);
  cout << endl;
}

template <class S>
void Graphe<S>::parcoursProfondeurRec(const S &s) const
{
  sommets.at(s).estVisite = true;
  cout << s << " ";

  for (const S &voisin : sommets.at(s).voisins)
  {
    if (!sommets.at(voisin).estVisite)
      parcoursProfondeurRec(voisin);
  }
}

template <class S>
void Graphe<S>::parcoursRechercheLargeur(const S &s) const
{
  resetVisite();

  queue<S> file;
  sommets.at(s).estVisite = true;
  file.push(s);

  while (!file.empty())
  {
    S elem = file.front();
    file.pop();
    cout << elem << " ";

    for (const S &voisin : sommets.at(elem).voisins)
    {
      if (!sommets.at(voisin).estVisite)
      {
        sommets.at(voisin).estVisite = true;
        file.push(voisin);
      }
    }
  }

  cout << endl;
}

template <class S>
void Graphe<S>::extraireComposantesConnexes() const
{
  resetVisite();
  std::cout << "{ ";
  for (const auto &pair : sommets)
  {
    if (pair.second.estVisite)
      continue;
    std::cout << "{ ";
    std::queue<S> file;
    pair.second.estVisite = true;
    file.push(pair.first);
    while (!file.empty())
    {
      S elem = file.front();
      file.pop();
      std::cout << elem << " ";
      const Sommet &sommet = sommets.at(elem);

      for (const S &voisin : sommet.voisins)
        if (!sommets.at(voisin).estVisite)
        {
          sommets.at(voisin).estVisite = true;
          file.push(voisin);
        }
    }
    std::cout << "}";
  }
  std::cout << " }" << std::endl;
}

template <class S>
void Graphe<S>::resetVisite() const
{
  for (auto &pair : sommets)
    pair.second.estVisite = false;
}

template <class S>
void Graphe<S>::construireGraphe(Histoire *histoire)
{
  int count = 0;
  vector<Phrase> phrases(histoire->begin(), histoire->end() - 1);

  for (Phrase phrase : phrases)
  {
    ajouterSommet(phrase);
    count++;
    // cout << phrase.ordonnee() << "   LIEN: " << endl;

    for (Phrase phrase2 : vector<Phrase>(phrases.begin() + count, phrases.end()))
    {

      // cout << phrase2.ordonnee() << "   distance: " << phrase.distance(phrase2) << endl;
      if (phrase.distance(phrase2) > 0)
        ajouterAreteOrientee(phrase, phrase2);
    }
    // cout << endl;
  }

  cout << "Nombre sommets: " << sommets.size() << endl;
  return;
}

template <class S>
bool Graphe<S>::estVoisin(const Phrase &sommet, const Phrase &voisin) const
{
  return sommets[sommet].voisins.find(voisin) != sommets[sommet].voisins.end();
}

template <class S>
double Graphe<S>::trouverChemin(int profondeur, Phrase depart, Phrase arrivee, map<int, Phrase> &chemin)
{
  if (profondeur == 0 && depart == arrivee)
    return 0;
  if (profondeur == 1)
    return depart.distance(arrivee);
  if (profondeur <= 0)
    return -1;

  double shortPath = 999999999;

  for (Phrase voisin : sommets[depart].voisins)
  {
    if (depart != voisin && arrivee != voisin)
    {
      double tempRes = trouverChemin(profondeur - 1, voisin, arrivee, chemin);
      if (tempRes != -1 && tempRes < shortPath)
      {
        chemin[profondeur] = voisin;
        shortPath = min(shortPath, depart.distance(voisin) + tempRes);
      }
    }

    return shortPath;
  }
}

#endif
