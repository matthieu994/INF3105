#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "arbreavl.h"

using namespace std;

// exit on error ?
// file / cin ?

struct Arbres
{
    ArbreAVL<pair<string, vector<string>>> arbreTypes;
    ArbreAVL<pair<string, vector<vector<string>>>> arbreFoncteurs;
};

string format(string &s);
void repartirInputs(Arbres &arbres, vector<string> inputs);
void insererType(Arbres &arbres, string type);
void insererFoncteur(Arbres &arbres, vector<string> foncteur);

void afficher_vector(vector<string> &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << v[i];
        i != v.size() - 1 && cout << ",";
    }
    cout << endl
         << "----" << endl;
}

string format(string &s, bool type)
{
    if (type)
    {
        s.erase(remove(s.begin(), s.end(), '{'), s.end());
        s.erase(remove(s.begin(), s.end(), '}'), s.end());
    }
    else
    {
        s.erase(remove(s.begin(), s.end(), ')'), s.end());
        s.erase(remove(s.begin(), s.end(), '('), s.end());
    }
    s.erase(remove(s.begin(), s.end(), ','), s.end());
    return s;
}

void repartirInputs(Arbres &arbres, vector<string> inputs)
{
    istringstream stream;
    string arg;
    for (size_t i = 0; i < inputs.size(); i++)
    {
        stream = istringstream(inputs[i]);
        stream >> arg;

        if (arg == "type")
            insererType(arbres, inputs[i]);

        if (arg == "foncteur")
        {
            vector<string> foncteur;
            size_t j = i;
            foncteur.push_back(inputs[j++]);
            stream = istringstream(inputs[j]);
            stream >> arg;
            while (arg != "type" && arg != "foncteur" && j < inputs.size() - 1)
            {
                foncteur.push_back(inputs[j++]);
                stream = istringstream(inputs[j]);
                stream >> arg;
            }
            i = j - 1;
            insererFoncteur(arbres, foncteur);
        }
    }
}

void insererType(Arbres &arbres, string type)
{
    istringstream stream = istringstream(type);
    string identificateur;
    stream >> identificateur;
    stream >> identificateur;

    if (identificateur != "type" && identificateur != "foncteur" && !arbres.arbreTypes.contient(identificateur) && !arbres.arbreFoncteurs.contient(identificateur))
    {
        pair<string, vector<string>> contenu;
        vector<string> elements;
        string element;
        // enlever '='
        stream >> element;

        while (stream >> element)
            elements.push_back(format(element, true));

        contenu.first = identificateur;
        contenu.second = elements;
        arbres.arbreTypes.inserer(contenu);
    }
}

void insererFoncteur(Arbres &arbres, vector<string> foncteur)
{
    istringstream stream = istringstream(foncteur[0]);
    string identificateur;
    stream >> identificateur;
    stream >> identificateur;

    if (identificateur != "type" && identificateur != "foncteur" && !arbres.arbreTypes.contient(identificateur) && !arbres.arbreFoncteurs.contient(identificateur))
    {
        pair<string, vector<vector<string>>> contenu;
        vector<vector<string>> elements(foncteur.size());
        string element;
        // enlever '::'
        stream >> element;

        // remplissage des n types
        while (stream >> element)
        {
            element = format(element, false);
            if (!arbres.arbreTypes.contient(element))
            {
                cerr << "Erreur de type pour le foncteur <" << identificateur << "> : " << element << endl;
                return;
            }
            elements[0].push_back(element);
        }

        // remplissage des m clauses
        for (size_t i = 1; i < foncteur.size(); i++)
        {
            stream = istringstream(foncteur[i]);

            while (stream >> element)
            {
                element = format(element, false);
                // verifier que le type existe dans l'arbre des types
                // if (!arbres.arbreTypes.get(elements[0][j]).)
                // {
                //     cerr << "Erreur de type pour le foncteur <" << identificateur << "> : " << element << endl;
                //     return;
                // }
                elements[i].push_back(format(element, false));
            }
        }

        contenu.first = identificateur;
        contenu.second = elements;
        arbres.arbreFoncteurs.inserer(contenu);
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <fichier>" << endl;
        exit(1);
    }

    ifstream entree;
    Arbres arbres;
    vector<string> inputs;
    string input;
    entree.open(argv[1]);

    while (getline(entree, input))
        inputs.push_back(input);

    repartirInputs(arbres, inputs);

    arbres.arbreTypes.afficher();
    arbres.arbreFoncteurs.afficher();

    while (cin >> input)
    {
    }
    return 0;
}
