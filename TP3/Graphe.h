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
#include <algorithm>
#include <list>

using namespace std;

// Le type S est le type utilisé pour identifier les sommets
template <class S>
class Graphe
{
public:
  // Interface public pour créer le graphe.
  void ajouterSommet(const S &s);
  void construireGraphe(Histoire *histoire);
  double trouverChemin(Phrase depart, Phrase arrivee);
  void setVoisins(const S &s, set<S> voisins);

  // Interface public pour les requêtes de base.

private:
  struct Sommet
  {
    set<S> voisins; // ensemble des sommets accessibles via les arêtes sortantes du sommet.
  public:
  };
  map<S, Sommet> sommets; // identification --> sommet
};

template <class S>
void Graphe<S>::ajouterSommet(const S &s)
{
  sommets[s] = Sommet();
}

template <class S>
void Graphe<S>::setVoisins(const S &s, set<S> voisins)
{
  // cout << s.ordonnee() << " | voisins: " << endl;
  // for (auto it = voisins.begin(); it != voisins.end(); ++it)
  // {
  //   if (s.distance(*it) == 0)
  //     voisins.erase(it);
  //   else
  //   {
  //     // cout << it->ordonnee() << endl;
  //   }
  // }
  // cout << endl;
  sommets[s].voisins = voisins;
}

template <class S>
void Graphe<S>::construireGraphe(Histoire *histoire)
{
  int count = 0;
  vector<Phrase> phrases(histoire->begin(), histoire->end());

  for (Phrase phrase : phrases)
  {
    ajouterSommet(phrase);
    count++;
    setVoisins(phrase, set<Phrase>(histoire->begin() + count, histoire->end()));
  }

  cout << "Nombre sommets: " << sommets.size() << endl;
  return;
}

bool phraseComparator(const pair<Phrase, double> p1, const pair<Phrase, double> p2)
{
  return p1.second > p2.second;
}

int longueurChemin(const map<Phrase, pair<Phrase, double>> listeChemins, const Phrase phrase, const Phrase depart)
{
  int length = 0;

  auto parent = listeChemins.find(phrase);
  while (parent->first != depart)
  {
    // cout << parent->first.ordonnee() << endl;
    parent = listeChemins.find(parent->second.first);
    length++;
  }

  return length;
}

void afficherChemin(map<Phrase, pair<Phrase, double>> listeChemins, Phrase depart, Phrase arrivee)
{
  vector<Phrase> chemin;
  auto parent = listeChemins.find(arrivee);
  double total;

  while (parent != listeChemins.end() && parent->first != depart)
  {
    chemin.push_back(parent->first);
    parent = listeChemins.find(parent->second.first);
  }

  chemin.push_back(depart);
  reverse(chemin.begin(), chemin.end());

  cout << "Taille originale: " << chemin.size() << endl;

  while (chemin.size() > 5)
    chemin.erase(chemin.end() - 2);

  cout << "Taille: " << chemin.size() << endl;
  for (size_t i = 0; i < chemin.size(); i++)
  {
    cout << chemin.at(i).original() << endl;
    if (i < chemin.size() - 1)
    {
      total += chemin.at(i).distance(chemin.at(i + 1));
      cout << "-> " << chemin.at(i).distance(chemin.at(i + 1)) << endl;
    }
  }
  cout << "Total: " << total << endl;
}

template <class S>
double Graphe<S>::trouverChemin(Phrase depart, Phrase arrivee)
{
  priority_queue<pair<Phrase, double>, vector<pair<Phrase, double>>, function<bool(pair<Phrase, double>, pair<Phrase, double>)>> queue(phraseComparator);
  map<Phrase, pair<Phrase, double>> listeChemins;
  map<Phrase, bool> listeVisite;
  double meilleurChemin = 99999;

  for (const auto &sommet : sommets)
  {
    listeChemins[sommet.first] = make_pair(depart, 99999);
  }

  listeChemins[depart].second = 0;
  queue.push(listeChemins[depart]);

  while (!queue.empty())
  {
    Phrase courant = queue.top().first;
    double distanceTotal = queue.top().second;
    queue.pop();

    if (listeVisite.find(courant) != listeVisite.end())
      continue;
    else
      listeVisite[courant] = true;

    // cout << "* courant: " << courant.original() << endl;
    // cout << "distance totale: " << distanceTotal << endl;
    // cout << "voisins: " << sommets[courant].voisins.size() << endl;
    if (distanceTotal + courant.distance(arrivee) < meilleurChemin && longueurChemin(listeChemins, courant, depart) >= 3 && courant.distance(arrivee) > 0)
    {
      // afficherChemin(listeChemins, courant, arrivee);
      meilleurChemin = distanceTotal + courant.distance(arrivee);
      listeChemins[arrivee] = make_pair(courant, courant.distance(arrivee));
      // cout << endl << "Chemin taille 5 !   " << longueurChemin(listeChemins, courant, depart) << endl;
      // cout << "-> " << distanceTotal << endl << endl;
      // cout << meilleurChemin << endl;
      continue;
    }

    for (Phrase voisin : sommets[courant].voisins)
    {
      double distance = courant.distance(voisin);

      if (voisin == arrivee || distance == 0)
        continue;
      if (listeVisite.find(voisin) != listeVisite.end())
        continue;
      // cout << distance << " < " << listeChemins[voisin].second << " ?  " << voisin.original() << endl;
      if (distance <= listeChemins[voisin].second)
      {
        // cout << distance << " -> " << voisin.original();
        // cout << "REMPLACÉ" << endl;
        // cout << " <-- PREV: " << listeChemins[voisin].second << endl;
        listeChemins[voisin] = make_pair(courant, distance);
        queue.push(make_pair(voisin, distance + distanceTotal));
      }
    }
    // cout << endl << endl;
  }

  afficherChemin(listeChemins, depart, arrivee);

  return 0.0;
}

#endif
