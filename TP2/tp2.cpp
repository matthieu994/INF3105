#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "arbremapavl.h"

using namespace std;

// exit on error ?
// file / cin ?

struct Arbres
{
    ArbreMapAVL<string, vector<string>> arbreTypes;
    ArbreMapAVL<string, vector<vector<string>>> arbreFoncteurs;
};

string format(string &s);
void repartirInputs(Arbres &arbres, vector<string> inputs);
void insererType(Arbres &arbres, string type);
void insererFoncteur(Arbres &arbres, vector<string> foncteur);
void afficherValeurPossible(Arbres arbres, string input);
void afficherType(Arbres arbres, string type);
void afficherFoncteur(Arbres arbres, string foncteur);

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

bool find_vector(vector<string> v, string element)
{
    if (find(v.begin(), v.end(), element) != v.end())
        return true;
    return false;
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
        stream.str(inputs[i]);
        stream >> arg;

        if (arg == "type")
            insererType(arbres, inputs[i]);

        if (arg == "foncteur")
        {
            vector<string> foncteur;
            size_t j = i;
            foncteur.push_back(inputs[j++]);
            stream.clear();
            stream.str(inputs[j]);
            stream >> arg;
            while (arg != "type" && arg != "foncteur" && j < inputs.size())
            {
                foncteur.push_back(inputs[j++]);
                if (j < inputs.size())
                {
                    stream.clear();
                    stream.str(inputs[j]);
                    stream >> arg;
                }
            }
            i = j - 1;
            insererFoncteur(arbres, foncteur);
        }
    }
}

void insererType(Arbres &arbres, string type)
{
    istringstream stream(type);
    string identificateur;
    stream >> identificateur;
    stream >> identificateur;

    if (identificateur != "type" && identificateur != "foncteur" && !arbres.arbreTypes.contient(identificateur) && !arbres.arbreFoncteurs.contient(identificateur))
    {
        vector<string> elements;
        string element;
        // enlever '='
        stream >> element;

        while (stream >> element)
            elements.push_back(format(element, true));

        arbres.arbreTypes.inserer(identificateur, elements);
    }
}

void insererFoncteur(Arbres &arbres, vector<string> foncteur)
{
    istringstream stream(foncteur[0]);
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
        size_t j;
        for (size_t i = 1; i < foncteur.size(); i++)
        {
            stream.clear();
            stream.str(foncteur[i]);
            j = 0;

            while (stream >> element)
            {
                element = format(element, false);
                string local_identificateur = elements[0][j];
                // verifier que le type existe dans l'arbre des types
                if (!find_vector(arbres.arbreTypes[local_identificateur], element))
                {
                    cerr << "Erreur de type pour le foncteur <" << identificateur << "> : " << element << endl;
                    return;
                }
                j++;
                elements[i].push_back(format(element, false));
            }
        }

        arbres.arbreFoncteurs.inserer(identificateur, elements);
    }
}

void afficherValeurPossible(Arbres arbres, string input)
{
    string identificateur = input.substr(0, input.find("("));

    if (!arbres.arbreFoncteurs.contient(identificateur))
    {
        cerr << "Foncteur inconnu" << endl;
    }
    else
    {
        vector<vector<string>> elements = arbres.arbreFoncteurs[identificateur];
        istringstream stream(input);
        string arg;
        int position = -1;

        while (stream >> arg)
        {
            position++;
            if (arg.find("?") != string::npos)
            {
                afficherType(arbres, elements[0][position]);
                return;
            }
        }
    }
}

void afficherType(Arbres arbres, string type)
{
    vector<string> elements = arbres.arbreTypes[type];
    cout << "{";
    for (size_t i = 0; i < elements.size(); i++)
    {
        cout << elements[i];
        if (i < elements.size() - 1)
            cout << ", ";
    }
    cout << "}" << endl;
}

void afficherFoncteur(Arbres arbres, string foncteur)
{
    vector<vector<string>> elements = arbres.arbreFoncteurs[foncteur];
    for (size_t i = 1; i < elements.size(); i++)
    {
        cout << "(";
        for (size_t j = 0; j < elements[i].size(); j++)
        {
            cout << elements[i][j];
            if (j < elements[i].size() - 1)
                cout << ", ";
        }
        cout << ")" << endl;
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

    while (getline(cin, input))
    {
        if (input.find("(") != string::npos)
            afficherValeurPossible(arbres, input);
        else if (input.find("?") != string::npos)
        {
            string identificateur = input.substr(0, input.find("?"));
            if (arbres.arbreTypes.contient(identificateur))
                afficherType(arbres, identificateur);
            else if (arbres.arbreFoncteurs.contient(identificateur))
                afficherFoncteur(arbres, identificateur);
            else
                cerr << "Identificateur inconnu" << endl;
        }
        else
            cerr << "Commande inconnue" << endl;
    }

    return 0;
}
