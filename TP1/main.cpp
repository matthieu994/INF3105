#include <iostream>
#include <list>
#include <map>
#include <sstream>

// Que faire dans a ce b si representants egaux
// Commande fausse dans exempls
// Que retourner pour rep a,b? si pas de commun
// Que retourner pour rep a,b? si -h

// TODO : getline() et retirer virgules pour éviter trim()

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
        int counter = 0, temp;
        pair<Element*, int> commun(nullptr, 0);
        map<Element*, int> liste;

        while (el1_ptr != nullptr) {
            liste[el1_ptr] = counter++;
            el1_ptr = el1_ptr->suivant;
        }

        counter = 0;
        while (el2_ptr != nullptr) {
            if (liste.count(el2_ptr) > 0) {
                temp = liste.at(el2_ptr);
                if (commun.second == 0 || counter + temp < commun.second) {
                    commun.first = el2_ptr;
                    commun.second = counter + temp;
                }
            }
            counter++;
            el2_ptr = el2_ptr->suivant;
        }
        if (commun.first)
            return commun.first->getIdentificateur();
        else
            return "false";
    }

    ~Element() {}
};

string trim(const string& str)
{
    string newString;
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        newString = str;
    } else {
        size_t last = str.find_last_not_of(' ');
        newString = str.substr(first, (last - first + 1));
    }
    return newString;
}

void enleverEspaces(string& commande)
{
    istringstream stream(commande);
    string word;
    while (stream >> word) {
        // cout << word << endl;
    }
}

/*
    input: string à évaluer
    La fonction insère un espace avant chaque point,
    et avant+après chaque virgule, elle fait aussi
    appel à la fonction trim().
*/
void evaluer(string& input)
{
    if (input.find('.') != string::npos) {
        input.insert(input.find('.'), 1, ' ');
        input.insert(input.find('.') + 1, 1, ' ');
    }
    if (input.find('?') != string::npos) {
        input.insert(input.find('?'), 1, ' ');
        input.insert(input.find('?') + 1, 1, ' ');
    }
    if (input.find(',') != string::npos) {
        int pos = input.find(',', 0);
        while (pos != string::npos) {
            input.insert(pos, 1, ' ');
            input.insert(pos + 2, 1, ' ');
            pos = input.find(',', pos + 3);
        }
    }
    input = trim(input);
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

void execCommande(const string arguments[], const int taille, map<string, Element*>& identificateurs, const bool optimisation)
{
    Element *rep, *e;
    if (identificateurs.count(arguments[0]) == 0) {
        rep = new Element(arguments[0]);
        identificateurs[arguments[0]] = rep;
    } else {
        rep = identificateurs.at(arguments[0]);
    }
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

void execRequete(const string arguments[], const int taille, const map<string, Element*>& identificateurs, const bool optimisation)
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

void gererArguments(string commande, bool requete, map<string, Element*>& identificateurs, bool optimisation)
{
    cout << commande << endl;
    // enleverEspaces(commande);
    cout << commande << endl;
    int nombre_arguments = count(commande, ' ') - count(commande, ',');
    int i = 0;
    int last = 0;
    int next = 0;
    string argument;
    string arguments[nombre_arguments] = {};
    while ((next = commande.find(' ', last)) != string::npos) {
        argument = commande.substr(last, next - last);
        if (argument != "," && argument != "." && argument != "?")
            arguments[i++] = argument;
        last = next + 1;
    }
    if (nombre_arguments > 1 && i < nombre_arguments)
        arguments[i] = commande.substr(last);

    if (!requete) {
        // execCommande(arguments, nombre_arguments, identificateurs, optimisation);
    } else {
        // execRequete(arguments, nombre_arguments, identificateurs, optimisation);
    }
    // displayTab(arguments, nombre_arguments);
    // displayMap(identificateurs);
}

int main(int argc, char const* argv[])
{
    string input;
    string backup_input;
    map<string, Element*> identificateurs;
    bool optimisation = false;
    if (argc > 1 && string(argv[1]) == "-h")
        optimisation = true;

    while (cin >> input) {
        evaluer(input);
        while (input.find(".") == string::npos && input.find("?") == string::npos) {
            if (cin >> backup_input) {
                evaluer(backup_input);
                input = input + " " + backup_input;
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
        } else if (input.find("?") != string::npos) {
            gererArguments(input, true, identificateurs, optimisation);
        }
    }
}
