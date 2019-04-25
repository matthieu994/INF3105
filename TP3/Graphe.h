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
  void trouverChemin(Phrase depart, Phrase arrivee);
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
  sommets[s].voisins = voisins;
}

// Construit le graphe
// histoire: histoire choisie dans le main
template <class S>
void Graphe<S>::construireGraphe(Histoire *histoire)
{
  int count = 0;
  vector<Phrase> phrases(histoire->begin(), histoire->end());

  // Itère sur toutes les phrases, ajoute le sommet et définit les voisins
  for (Phrase phrase : phrases)
  {
    ajouterSommet(phrase);
    count++;
    // Crée un set en partant de la phrase courante, jusqu'à la fin
    setVoisins(phrase, set<Phrase>(histoire->begin() + count, histoire->end()));
  }

  // cout << "Nombre sommets: " << sommets.size() << endl;
  return;
}

// Comparateur utilisé pour la priority queue
bool phraseComparator(const pair<Phrase, double> p1, const pair<Phrase, double> p2)
{
  return p1.second > p2.second;
}

// Retourne la longueur d'un chemin en parcourant 'listeChemins' depuis 'phrase' jusqu'à la phrase 'depart'
// listeChemins: map contenant les phrases et leur parent
// phrase: phrase courante (fin du chemin)
// depart: 1ère phrase de l'histoire
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

// Affiche le meilleur chemin trouvé
// listeChemins: map contenant les phrases et leur parent
// depart: phrase courante (debut du chemin)
// arrivee: (avant) dernière phrase de l'histoire
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

  // DEBUG - Affiche la taille du chemin trouvée
  // cout << "Taille: " << chemin.size() << endl;
  for (size_t i = 0; i < chemin.size(); i++)
  {
    cout << chemin.at(i).original() << endl;
    if (i < chemin.size() - 1)
    {
      total += chemin.at(i).distance(chemin.at(i + 1));
      // DEBUG - Affiche la distance entre x et x+1
      // cout << "-> " << chemin.at(i).distance(chemin.at(i + 1)) << endl;
    }
  }
  cout << "Total: " << total << endl;
}

// Trouve le meilleur chemin dans le graphe
// depart : 1ere phrase
// arrivee : (avant) dernière phrase
template <class S>
void Graphe<S>::trouverChemin(Phrase depart, Phrase arrivee)
{
  // Queue utilisée pour Dijkstra
  priority_queue<pair<Phrase, double>, vector<pair<Phrase, double>>, function<bool(pair<Phrase, double>, pair<Phrase, double>)>> queue(phraseComparator);
  // Contient le meilleur chemin pour chaque phrase
  map<Phrase, pair<Phrase, double>> listeChemins;
  // Liste des noeuds visités
  map<Phrase, bool> listeVisite;
  // Meilleur chemin courant
  double meilleurChemin = 99999;

  for (const auto &sommet : sommets)
  {
    listeChemins[sommet.first] = make_pair(depart, 99999);
  }

  listeChemins[depart].second = 0;
  queue.push(listeChemins[depart]);

  // Dijkstra
  while (!queue.empty())
  {
    Phrase courant = queue.top().first;
    double distanceTotal = queue.top().second;
    int nombreSommets = longueurChemin(listeChemins, courant, depart);
    queue.pop();

    // Si le chemin courant est trop grand ou le noeud est déjà visité
    if (listeVisite.find(courant) != listeVisite.end() || nombreSommets >= 4)
      continue;
    else
      listeVisite[courant] = true;

    // Itère sur tous les voisins du noeud courant
    for (Phrase voisin : sommets[courant].voisins)
    {
      double distance = courant.distance(voisin);

      if (distance == 0)
        continue;

      if (voisin == arrivee)
      {
        if (distanceTotal + courant.distance(voisin) < meilleurChemin && nombreSommets >= 3)
        {
          meilleurChemin = distanceTotal + courant.distance(arrivee);
          listeChemins[arrivee] = make_pair(courant, courant.distance(arrivee));
        }
        else
          continue;
      }

      if (listeVisite.find(voisin) != listeVisite.end())
        continue;

      // Si la distance courant -> voisin est plus petite que la distance déjà trouvée
      if (distance <= listeChemins[voisin].second)
      {
        // Si le chemin est assez long et plus court que le chemin déjà trouvé
        if (distanceTotal + distance + voisin.distance(arrivee) < meilleurChemin && nombreSommets >= 3 && voisin.distance(arrivee) > 0)
        {
          meilleurChemin = distanceTotal + distance + voisin.distance(arrivee);
          listeChemins[arrivee] = make_pair(voisin, voisin.distance(arrivee));
        }
        listeChemins[voisin] = make_pair(courant, distance);
        queue.push(make_pair(voisin, distance + distanceTotal));
      }
    }
  }

  afficherChemin(listeChemins, depart, arrivee);
}

#endif
