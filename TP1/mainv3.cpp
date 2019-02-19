#include <algorithm>
#include <ctime>
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

static const clock_t start = clock();
clock_t functionTime = 0;

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

    void definirChaineRepresentant()
    {
        Element* parent = this->suivant;
        while (parent != nullptr) {
            parent->definirRang(parent->getRang() + 1);
            parent = parent->suivant;
        }
    }

    Element* representant(const bool& optimisation)
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
        Element* el1_ptr = el2;
        Element* el2_ptr = this;
        Element* commun = nullptr;
        list<Element*> liste;

        clock_t localTime = clock();
        while (el1_ptr != nullptr) {
            liste.push_back(el1_ptr);
            el1_ptr = el1_ptr->suivant;
        }

        while (el2_ptr != nullptr && commun == nullptr) {
            if (find(liste.begin(), liste.end(), el2_ptr) != liste.end())
                commun = el2_ptr;
            el2_ptr = el2_ptr->suivant;
        }

        functionTime += clock() - localTime;
        if (commun != nullptr)
            return commun->getIdentificateur();
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
    if (requete)
        commande.erase(commande.find("?"));
    else
        commande.erase(commande.find("."));
    replace(commande.begin(), commande.end(), ',', ' ');
}

int count(string s, char c)
{
    int count = 0;
    for (size_t i = 0; i < s.size(); i++)
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
void execCommande(const vector<string> arguments, map<string, Element*>& identificateurs, const bool& optimisation)
{
    int taille = arguments.size();
    Element *id0, *idX;
    map<string, Element*>::iterator element_0 = identificateurs.find(arguments[0]);
    // Si le premier argument n'existe pas, on le créé et on l'ajoute dans la map
    if (element_0 == identificateurs.end()) {
        id0 = new Element(arguments[0]);
        identificateurs[arguments[0]] = id0;
    } else {
        id0 = element_0->second;
    }
    /* Cas de la 2e commande : on créé le deuxième argument dans la map
       puis on assigne les representants de chaque identifiant */
    if (taille > 1 && arguments[1] == "rep") {
        for (int i = 2; i < taille; i++) {
            element_0 = identificateurs.find(arguments[i]);
            if (element_0 == identificateurs.end()) {
                idX = new Element(arguments[i]);
                identificateurs[arguments[i]] = idX;
            } else {
                idX = element_0->second;
            }
            if (idX == id0) {
                cerr << "Entrees non conforme." << endl;
                exit(-1);
            }
            idX->definirRepresentant(id0);
            if (id0->getRang() <= idX->getRang()) {
                id0->definirRang(idX->getRang() + 1);
                idX->definirChaineRepresentant();
            }
        }
        /* 3e commande : on fusionne les 2 classes */
    } else if (taille > 1 && arguments[1] == "ce") {
        element_0 = identificateurs.find(arguments[2]);
        if (element_0 == identificateurs.end()) {
            idX = new Element(arguments[2]);
            identificateurs[arguments[2]] = idX;
            idX = idX->representant(optimisation);
        } else
            idX = element_0->second->representant(optimisation);
        if (id0->getRang() > idX->getRang() && id0 != idX)
            idX->definirRepresentant(id0);
        else if (id0->getRang() < idX->getRang() && id0 != idX)
            id0->definirRepresentant(idX);
        else if (id0->getRang() == idX->getRang() && id0 != idX) {
            id0->definirRang(id0->getRang() + 1);
            idX->definirRepresentant(id0);
        }
    } else if (taille > 1) {
        cerr << "Entrees non conforme." << endl;
        exit(-1);
    }
}

void execRequete(const vector<string> arguments, const map<string, Element*>& identificateurs, const bool& optimisation)
{
    int taille = arguments.size();
    if (taille == 2) {
        if (arguments[0] != "rep") {
            cerr << "Entrees non conforme." << endl;
            exit(-1);
        } else {
            auto identifiant = identificateurs.find(arguments[1]);
            if (identifiant == identificateurs.end())
                cerr << "Identificateur inexistant." << endl;
            else
                cout << identifiant->second->representant(optimisation)->getIdentificateur() << endl;
        }
    } else if (taille == 3) {
        if (arguments[1] == "ce") {
            auto id1 = identificateurs.find(arguments[0]);
            auto id2 = identificateurs.find(arguments[2]);
            // clock_t localTime = clock();
            if (id1 == identificateurs.end() || id2 == identificateurs.end())
                cerr << "Identificateur inexistant." << endl;
            else {
                if (id1->second->representant(optimisation) != id2->second->representant(optimisation))
                    cout << "non" << endl;
                else
                    cout << "oui" << endl;
            }
            // functionTime += clock() - localTime;
        } else if (arguments[0] == "rep") {
            auto id1 = identificateurs.find(arguments[1]);
            auto id2 = identificateurs.find(arguments[2]);
            if (id1 == identificateurs.end() || id2 == identificateurs.end())
                cerr << "Identificateur inexistant." << endl;
            else {
                cout << id1->second->representantCommun(id2->second) << endl;
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
    // int nombre_arguments = nombreMots(commande);
    // cout << nombre_arguments << endl;
    vector<string> arguments;
    istringstream stream(commande);
    string argument;
    commande = "";
    while (stream >> argument) {
        arguments.push_back(argument);
    }

    // displayTab(arguments, nombre_arguments);
    if (!requete) {
        execCommande(arguments, identificateurs, optimisation);
    } else {
        execRequete(arguments, identificateurs, optimisation);
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

        if (input.rfind(".") != string::npos && input.rfind("?") != string::npos) {
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

    auto total = clock() - start;
    cerr << "total time : " << (total - functionTime) / (double)total * 100.0 << "%" << endl;
    cerr << "time in function: " << functionTime / (double)total * 100.0 << "%" << endl;
}
