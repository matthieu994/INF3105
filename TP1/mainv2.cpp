#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <sstream>
#include <string>

// Que faire dans a ce b si representants egaux
// Que retourner pour rep a,b? si pas de commun
// Que retourner pour rep a,b? si -h

using namespace std;

class Element {
protected:
    string identificateur;
    int rang;
    Element* suivant;

public:
    Element(string _identificateur)
        : identificateur(_identificateur)
        , rang(0)
        , suivant(nullptr)
    {
    }

    operator std::string() const
    {
        return string("'" + identificateur + "'") + ", rang: " + to_string(rang) + (suivant == nullptr ? "" : "   ->" + string(*suivant));
    }

    int getRang()
    {
        return rang;
    }

    string getIdentificateur()
    {
        return identificateur;
    }

    void definirRang(int _rang)
    {
        rang = _rang;
    }

    void definirRepresentant(Element* E)
    {
        suivant = E;
    }

    Element* representant(bool optimisation)
    {
        Element* dernier = this;
        list<Element*> liste;
        while (dernier->suivant != nullptr) {
            if (optimisation)
                liste.push_back(dernier);
            dernier = dernier->suivant;
        }

        if (optimisation && liste.size() > 0) {
            if (liste.front()->suivant != dernier) {
                for (Element* el : liste)
                    el->suivant = dernier;
            }
        }

        return dernier;
    }

    string representantCommun(Element* el2)
    {
        Element* el1_ptr = this;
        Element* el2_ptr = el2;
        int counter = 0;
        pair<Element*, int> commun(nullptr, 0);
        map<Element*, int> liste;
        map<Element*, int>::iterator iterateur;

        while (el1_ptr != nullptr) {
            liste[el1_ptr] = counter++;
            el1_ptr = el1_ptr->suivant;
        }

        counter = 0;
        while (el2_ptr != nullptr) {
            if ((iterateur = liste.find(el2_ptr)) != liste.end()) {
                if (commun.second == 0 || counter + iterateur->second < commun.second) {
                    commun.first = el2_ptr;
                    commun.second = counter + iterateur->second;
                }
            }
            counter++;
            el2_ptr = el2_ptr->suivant;
        }
        if (commun.first)
            return commun.first->getIdentificateur();
        else
            return "Pas d'identificateur en commun.";
    }

    ~Element() {}
};

/*
    commande: string à formater
    La fonction enlève les espaces à la fin,
    retirer les virgules et '.' et '?'
    retour: ne retourne rien et modifie l'entrée
*/
void formater(string& commande, const bool& requete)
{
    // cout << "'" << commande << "'" << endl;
    // cout << commande.length() << endl;
    // if (!regex_match(commande, regex(R"( *[a-z, ]+[\?\.] *)", regex_constants::icase))) {
    //     cerr << "Entrees non conforme." << endl;
    //     exit(-1);
    // }

    // if (commande.find("rep") != string::npos && !regex_match(commande, regex(R"( *([a-z]*)? *rep *( *[a-z]+ *, *)*[a-z]+ *[\?\.] *)", regex_constants::icase))) {
    //     cerr << "Entrees non conforme." << endl;
    //     exit(-1);
    // }
    if (requete)
        commande.erase(commande.find("?"));
    else
        commande.erase(commande.find("."));
    replace(commande.begin(), commande.end(), ',', ' ');

    // commande.pop_back();
    // cout << "'" << commande << "'" << endl;
    // cout << commande.length() << endl;
}

/*
    commande: string à calculer
    La fonction compte le nombre de mots dans la commande
    retour: retourne nombre de mots
*/
int nombreMots(string& commande)
{
    istringstream stream(commande);
    string word;
    commande = "";
    int counter = 0;

    while (stream >> word) {
        commande += word + " ";
        counter++;
    }
    return counter;
}

int count(string s, char c)
{
    int count = 0;
    for (int i = 0; i < s.size(); i++)
        if (s[i] == c)
            count++;
    return count;
}

void displayTab(string array[], int size)
{
    for (int i = 0; i < size; i++)
        cout << '"' << array[i] << "\" ";
    cout << endl;
}

void displayMap(map<string, Element*> myMap)
{
    for (pair<string, Element*> elem : myMap)
        cout << string(*elem.second) << endl;
    cout << endl;
}

// Gère les 3 premières commandes
void execCommande(const string arguments[], const int& taille, map<string, Element*>& identificateurs, const bool& optimisation)
{
    Element *rep, *e;
    // Si le premier argument n'existe pas, on le créé et on l'ajoute dans la map
    if (identificateurs.count(arguments[0]) == 0) {
        rep = new Element(arguments[0]);
        identificateurs[arguments[0]] = rep;
    } else {
        rep = identificateurs.at(arguments[0]);
    }
    /* Cas de la 3e commande : on créé le deuxième argument dans la map
       puis on assigne les representants de chaque identifiant */
    if (taille > 1 && arguments[1] == "rep") {
        for (int i = 2; i < taille; i++) {
            if (identificateurs.count(arguments[i]) == 0) {
                e = new Element(arguments[i]);
                identificateurs[arguments[i]] = e;
            } else {
                e = identificateurs.at(arguments[i]);
            }
            if (e == rep) {
                cerr << "Entrees non conforme." << endl;
                exit(-1);
            }
            e->definirRepresentant(rep);
            if (rep->getRang() <= e->getRang())
                rep->definirRang(e->getRang() + 1);
        }
        /* 2e commande : on fusionne les 2 classes */
    } else if (taille > 1 && arguments[1] == "ce") {
        if (identificateurs.count(arguments[2]) == 0) {
            e = new Element(arguments[2]);
            identificateurs[arguments[2]] = e;
            e = e->representant(optimisation);
        } else
            e = identificateurs.at(arguments[2])->representant(optimisation);
        if (rep->getRang() > e->getRang() && rep != e)
            e->definirRepresentant(rep);
        else if (rep->getRang() < e->getRang() && rep != e)
            rep->definirRepresentant(e);
        else if (rep->getRang() == e->getRang() && rep != e) {
            rep->definirRang(rep->getRang() + 1);
            e->definirRepresentant(rep);
        }
    } else if (taille > 1) {
        cerr << "Entrees non conforme." << endl;
        exit(-1);
    }
}

void execRequete(const string arguments[], const int& taille, const map<string, Element*>& identificateurs, const bool& optimisation)
{
    if (taille == 2) {
        if (arguments[0] != "rep") {
            cerr << "Entrees non conforme." << endl;
            exit(-1);
        } else {
            if (identificateurs.count(arguments[1]) == 0)
                cerr << "Identificateur inexistant." << endl;
            else
                cout << identificateurs.at(arguments[1])->representant(optimisation)->getIdentificateur() << endl;
        }
    } else if (taille == 3) {
        if (arguments[1] == "ce") {
            if (identificateurs.count(arguments[0]) == 0 || identificateurs.count(arguments[2]) == 0)
                cerr << "Identificateur inexistant." << endl;
            else {
                if (identificateurs.at(arguments[0])->representant(optimisation) != identificateurs.at(arguments[2])->representant(optimisation))
                    cout << "non" << endl;
                else
                    cout << "oui" << endl;
            }
        } else if (arguments[0] == "rep") {
            if (identificateurs.count(arguments[1]) == 0 || identificateurs.count(arguments[2]) == 0)
                cerr << "Identificateur inexistant." << endl;
            else {
                cout << identificateurs.at(arguments[1])->representantCommun(identificateurs.at(arguments[2])) << endl;
            }
        } else {
            cerr << "Entrees non conforme." << endl;
            exit(-1);
        }
    } else {
        cerr << "Entrees non conforme." << endl;
        exit(-1);
    }
}

void gererArguments(string& commande, bool requete, map<string, Element*>& identificateurs, bool& optimisation)
{
    formater(commande, requete);
    int nombre_arguments = nombreMots(commande);
    // cout << nombre_arguments << endl;
    int i = 0;
    string arguments[nombre_arguments];
    istringstream stream(commande);
    string argument;
    commande = "";
    while (stream >> argument) {
        arguments[i++] = argument;
    }

    // displayTab(arguments, nombre_arguments);
    if (!requete) {
        execCommande(arguments, nombre_arguments, identificateurs, optimisation);
    } else {
        execRequete(arguments, nombre_arguments, identificateurs, optimisation);
    }
    // displayMap(identificateurs);
}

int main(int argc, char const* argv[])
{
    string input;
    string second_input;
    map<string, Element*> identificateurs;
    bool optimisation = false;
    if (argc > 1 && string(argv[1]) == "-h")
        optimisation = true;

    while (getline(cin, input)) {
        while (input.find(".") == string::npos && input.find("?") == string::npos) {
            if (getline(cin, second_input)) {
                input += second_input;
            } else {
                cerr << "Entrees non conforme." << endl;
                exit(-1);
            }
        }

        if (input.find(".") != string::npos && input.find("?") != string::npos) {
            cerr << "Entrees non conforme." << endl;
            exit(-1);
        }

        if (count(input, '.') > 1 || count(input, '?') > 1) {
            cerr << "Entrees non conforme." << endl;
            exit(-1);
        }

        if (input.find(".") != string::npos) {
            gererArguments(input, false, identificateurs, optimisation);
        } else {
            gererArguments(input, true, identificateurs, optimisation);
        }
    }
}
