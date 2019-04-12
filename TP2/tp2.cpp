/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes

   AUTEUR:
    1) Matthieu PETIT : PETM09089908
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "arbremapavl.h"

using namespace std;

// Structure qui contient les 2 arbres nécessaires
struct Arbres
{
    ArbreMapAVL<string, vector<string>> arbreTypes;
    ArbreMapAVL<string, vector<vector<string>>> arbreFoncteurs;
};

string format(string &s, bool type);
void repartirInputs(Arbres &arbres, vector<string> inputs);
void insererType(Arbres &arbres, string type);
void insererFoncteur(Arbres &arbres, vector<string> foncteur);
void trouverFoncteur(const Arbres &arbres, const string &input);
void afficherValeurs(const vector<string> &elements);
void afficherType(const Arbres &arbres, const string &type);
void afficherFoncteur(const Arbres &arbres, const string &foncteur);

// DEBUG
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

// Trouve une valeur dans un vecteur
bool find_vector(vector<string> v, string element)
{
    if (find(v.begin(), v.end(), element) != v.end())
        return true;
    return false;
}

// Retire accolades parenthèses et virgules selon l'entrée (type ou foncteur)
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

// Répartit et sépare les entrées, sous forme de type ou sous forme de foncteur
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

// Insère le type dans l'arbre des types
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

// Insère le foncteur dans l'arbre des foncteurs
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
                exit(1);
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
                    exit(1);
                }
                j++;
                elements[i].push_back(format(element, false));
            }
        }

        arbres.arbreFoncteurs.inserer(identificateur, elements);
    }
}

// 1e commande : Cherche les combinaisons existantes dans l'arbre des foncteurs, selon la position du '?'
void chercherValeurPossible(const Arbres &arbres, const vector<string> &input, const string &identificateur, const size_t &position)
{
    vector<vector<string>> elements = arbres.arbreFoncteurs[identificateur];
    vector<string> valeurs_possibles;
    size_t longueur = input.size();
    size_t count = 0;

    for (vector<string> &element : elements)
    {
        if (count == 0)
        {
            count++;
            continue;
        }

        if (element.size() == longueur && !find_vector(valeurs_possibles, element[position]))
        {
            bool correspondant = true;
            for (size_t i = 0; i < longueur; i++)
            {
                if (i != position && element[i] != input[i])
                    correspondant = false;
            }
            if (correspondant)
                valeurs_possibles.push_back(element[position]);
        }
    }

    afficherValeurs(valeurs_possibles);
}

// 1ere commande : Cherche le foncteur spécifié dans la commande ainsi que la position du '?' 
void trouverFoncteur(const Arbres &arbres, const string &input)
{
    string identificateur = input.substr(0, input.find("("));

    if (!arbres.arbreFoncteurs.contient(identificateur))
    {
        cerr << "Foncteur inconnu" << endl;
        exit(1);
    }
    else
    {
        istringstream stream(input);
        string arg;
        size_t position = -1;

        while (stream >> arg)
        {
            position++;
            if (arg.find("?") != string::npos)
            {
                // Creer vecteurs avec arguments de la requete
                vector<string> input_vector;
                string arguments = input.substr(input.find("("), input.length());
                istringstream stream(arguments);
                string argument;

                while (stream >> argument)
                    input_vector.push_back(format(argument, false));

                return chercherValeurPossible(arbres, input_vector, identificateur, position);
            }
        }
    }
}

// Affiche toutes les valeurs d'un vecteur spécifié
void afficherValeurs(const vector<string> &elements)
{
    cout << "{";
    for (size_t i = 0; i < elements.size(); i++)
    {
        cout << elements[i];
        if (i < elements.size() - 1)
            cout << ", ";
    }
    cout << "}" << endl;
}

// 2e commande : Cherche et retourne le type tel quel
void afficherType(const Arbres &arbres, const string &type)
{
    vector<string> elements = arbres.arbreTypes[type];
    afficherValeurs(elements);
}

// 3e commande : Cherche et retourne le foncteur tel quel
void afficherFoncteur(const Arbres &arbres, const string &foncteur)
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

    // Lit le fichier en entrée
    while (getline(entree, input))
        inputs.push_back(input);

    // Insère dans la base de connaissances
    repartirInputs(arbres, inputs);

    // Lit l'entrée standard
    while (getline(cin, input))
    {
        if (input.find("(") != string::npos)
            trouverFoncteur(arbres, input);
        else if (input.find("?") != string::npos)
        {
            string identificateur = input.substr(0, input.find("?"));
            if (arbres.arbreTypes.contient(identificateur))
                afficherType(arbres, identificateur);
            else if (arbres.arbreFoncteurs.contient(identificateur))
                afficherFoncteur(arbres, identificateur);
            else
            {
                cerr << "Identificateur inconnu" << endl;
                exit(1);
            }
        }
        else
        {
            cerr << "Commande inconnue" << endl;
            exit(1);
        }
    }

    return 0;
}
