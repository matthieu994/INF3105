#include <utility>
#include <iostream>
#include <map>
#include <regex>
#include <cstdlib>
#include <algorithm>
#include <string.h>
#include <time.h>

using namespace std;

/*
 * Cette fonction retourne un type d'erreur identifié en paramètre
 */
void erreur(int i) {
    if( i == 1) {
        cerr << "Entrees non conforme." << endl;
        exit(-1);
    }else if(i == 2)
        cerr << "Identificateur inexistant." << endl;
}

// trim from start (in place)
static inline void ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

class Element {
protected:
    string identificateur;
    int rang;
    Element * suivant;
private:
    Element * representant_Opt() {

        if(suivant == nullptr)
            return this;

        suivant = suivant->representant_Opt();

        return suivant;
    }

    Element * representant_NoOpt() {
        Element * tmp = this;

        while (tmp->suivant != nullptr)
            tmp = tmp->suivant;

        return tmp;
    }
public:
    static bool optimisation;

    //il s'agit du constructeur
    explicit Element (const string & e): identificateur(e),rang(0),suivant(nullptr) {};

    /*
     * Cette méthode retourne l'élément qui represente l'élement courant. si la variable opt2 est mis à false
     * aucune optimisation n'est faite. Même si la variable globale optimisation est mis à true
    */
    Element * representant() {
        Element * res;
        if(optimisation)
            res = representant_Opt();
        else
            res = representant_NoOpt();
        return res;
    };

    //
    string parent_commun(Element * id) {
        string res = "Pas d'identificateur commun";
        Element * tmp = this;
        Element * tmp2 = id;
        int i1 = tmp->rang, i2 = tmp2->rang;


        while(tmp != nullptr && tmp2 != nullptr) {
            if(tmp == tmp2) {
                res = tmp->identificateur;
                break;
            }else if(i1 < i2) {
                tmp = tmp->suivant;
                i1 = tmp->rang;
            }else if(i2 < i1) {
                tmp2 = tmp2->suivant;
                i2 = tmp2->rang;
            }else{
                tmp = tmp->suivant;
                tmp2 = tmp2->suivant;
                i1 = tmp->rang;
                i2 = tmp2->rang;
            }

        }
        return res;
    }

    //cette méthode défini le représentant de l'élément courant
    void setSuivant(Element* s) {
        if(suivant != nullptr)
            erreur(1);
        suivant = s;
    }

    //setteur du rang
    void setRang(int rang_) {
        Element * tmp = suivant;
        rang = rang_;
        while(tmp != nullptr) {
            if(tmp->rang <= rang_)
                tmp->rang = rang_+1;
            else
                break;
            rang_ = tmp->rang;
            tmp = tmp->suivant;
        }
    }

    //getteur du rang
    int getRang() {
        return rang;
    };

    //getteur de l'identificateur
    string getIdentificateur() {
        return identificateur;
    }
};

//fonction de Debug. elle affiche la map
void afficher(map<string,Element * > m) {
    for (auto &it : m)
        cerr << it.second->getIdentificateur() << " - " << it.second->getRang() << "." << endl;
}

/*
 * Cette classe est utiliser pour traiter les lignes reçues en entrée et le stockage
 */
class Commande {
private:
    deque<string> elem;
    int mot_cle;
    int type;
    map<string, Element* > m;

    bool estAlphabetic(const string & s) {
        bool res = true;
        for (char it : s)
            if(!isalpha(it))
                res = false;
        return res;
    }

    /*Cette méthode a 2 fonctionnalités. test à false => elle permet de créer un nouvel élément dans la map si elle
 * n'existe pas sinon elle renvoie l'élement existant.
 * test à true => elle vérifie l'existence de l'élément dans la map et la renvoie sinon elle retour un nullptr
*/
    Element * ajoutElement(const string & entree,bool test = false) {
        // on cherche l'élément dans la map
        auto it = m.find(entree);
        Element * res;
        if(it == m.end()) {
            // s'il ne le trouve pas ...
            if(!test) {
                // et qu'on ne vérifie pas son existance alors on le créé
                res = new Element(entree);
                m[entree] = res;
            }else
                // sinon(on vérifie son existence) on retourne null
                res = nullptr;
        }else
            // sinon on retourne ce qu'il a trouvé
            res = it->second;

        return res;
    }

// cette méthode vérifie l'égalité de 2 représentants de 2 éléments retourne vrai si c'est le cas faux sinon
    bool ce_req() {
        string str1 = retirer(), str2 = retirer();
        Element *id0 = ajoutElement(str1,true), *id1 = ajoutElement(str2,true);
        if(id0 == nullptr || id1 == nullptr)
            erreur(2);

        return id0->representant() == id1->representant();
    }

/* cette fonction ajoute les éléments dans la map qui se trouve dans le tableau de string. elle identifie
 * les représentants des éléments 1 à taille-1 égale à 0 dans cmd
 */
    void rep_cmd() {
        string str = retirer();
        Element* representant = ajoutElement(str);
        unsigned long i, taille = this->taille();
        int j;
        for(i=0;i < taille; i++) {
            str = retirer();
            Element * tmp = ajoutElement(str);
            tmp->setSuivant(representant);
            if( representant->getRang() <= (j = tmp->getRang()) )
                representant->setRang(j+1);
        }
    }

/*
 * cette fonction effectue la recherche d'un parent commun dans la liaison de 2 éléments dans la map. Si ces éléments
 * ne sont pas présents une erreur est retournée
 */
    void rep_req2() {
        string str1 = retirer();
        string str2 = retirer();
        Element *id0 = ajoutElement(str1,true), *id1 = ajoutElement(str2,true);

        if(id0 == nullptr || id1 == nullptr)
            erreur(2);
        else
            cout << id0->parent_commun(id1) << endl;
    }

/*
 * cette méthode retourne le représentant de cmd dans la map m
 */
    void rep_req1() {
        Element *id = ajoutElement(retirer(),true);

        if(id == nullptr) {
            erreur(2);
            return;
        }

        cout << id->representant()->getIdentificateur() << endl;
    }

/*
 * en fonction des paramètres (taille) cette fonction en exécute une autre
 */
    void rep_req() {
        unsigned long taille = this->taille();
        if(taille == 1)
            rep_req1();
        else if(taille == 2)
            rep_req2();
        else
            erreur(1);
    }

/*
 * Cette fonction permet d'associer le representant d'un élément à un autre
 */
    void ce_cmd() {
        Element *id0 = ajoutElement(retirer()), *id1 = ajoutElement(retirer());
        if(id0->representant()->getRang() < id1->representant()->getRang())
            id0->representant()->setSuivant(id1->representant());
        else if (id0->representant()->getRang() > id1->representant()->getRang())
            id1->representant()->setSuivant(id0->representant());
        else {
            id1->setSuivant(id0->representant());
            id0->setRang(id0->getRang()+1);
        }
    }
public:
    const static int NO = 0;
    const static int REP = 1;
    const static int CE = 2;
    const static int CMD = 3;
    const static int REQ = 4;

    Commande() {
        effacer();
    }

    void ajouter(string & e) {
        if(e.empty() || e == "rep" || e == "ce" || !estAlphabetic(e))
            erreur(1);

        elem.push_back(e);
    }

    string retirer() {
        string res;
        try {
            res = string(elem.front());
        }catch(int e) {
            erreur(1);
        }
        elem.pop_front();
        return res;
    }

    void effacer() {
        elem.clear();
        type = NO;
        mot_cle = NO;
    }

    void setType(int _type) {
        type = _type;
    }

    void setKey_word(int _key_word) {
        mot_cle = _key_word;
    }

    unsigned long taille() {
        return elem.size();
    }

    void execution() {
        if( (mot_cle == REP || mot_cle == NO) && type == CMD)
            rep_cmd();
        else if(mot_cle == REP && type == REQ)
            rep_req();
        else if(mot_cle == CE && type == CMD)
            ce_cmd();
        else if(mot_cle == CE && type == REQ) {
            if(ce_req())
                cout << "oui" << endl;
            else
                cout << "non" << endl;
        }else
            erreur(1);

        effacer();
    }

    void effacer_map() {
        m.clear();
    }
};

string getWord(string & cmd) {
    ltrim(cmd);
    string res;
    if(cmd[0] == ',' || cmd[0] == '?' || cmd[0] == '.') {
        res = string(cmd,0,1);
        cmd.erase(0,1);
    }else {
        auto it = cmd.find_first_of(" ?.,");
        res = string(cmd,0,it);
        cmd.erase(0,it);
    }
    return res;
}

// je ne peux pas utiliser le regexp à cause de la version de g++ sur le serveur de l'uqam
/*
bool match(const string &entree) {
    regex pattern1(
            R"(^[\getword_\t\f ]*[a-z]+[\getword_\t\f ]+rep[\getword_\t\f ]+[a-z]+([\getword_\t\f ]*,[\t\getword_\f ]*[\n]?[a-z]+)*[\getword_\t\f ]*\.$)",
                regex_constants::icase);
    regex pattern2(R"(^[a-z]+[\t\getword_\f ]*\.$)", regex_constants::icase);
    regex pattern3(R"(^[a-z]+[\t\getword_\f ]+ce[\t\getword_\f ]+[a-z]+[\t\getword_\f ]*(\.|\?)$)", regex_constants::icase);
    regex pattern4(
            R"(^rep[\t\getword_\f ]+[a-z]+[\t\getword_\f ]*(,[\t\getword_\f ]*[\v]?[a-z]+[\t\getword_\f ]*)?\?$)", regex_constants::icase);

    return regex_match(entree,pattern1) || regex_match(entree,pattern2) || regex_match(entree,pattern3) ||
        regex_match(entree,pattern4);
}*/

//déclaration de la variable static de la classe Element
bool Element::optimisation = false;

int main(int argc, char* argv[]) {

    if(argc == 2 && strcmp(argv[1],"-h") == 0)
        Element::optimisation = true;

    Commande line;
    string cmd;
    while(cin && getline(cin,cmd)) {
        string word;
        if(!cmd.find_first_of("?."))
            while(cin && getline(cin,word)) {
                if(word.find_first_of("?."))
                    break;
                cmd += " " + word;
            }
        if(!cin)
            break;
        cmd += word;

        word = getWord(cmd);

        if(word == "rep") {
            line.setKey_word(Commande::REP);

            word = getWord(cmd);
            line.ajouter(word);

            word = getWord(cmd);

            if(word == "?")
                line.setType(Commande::REQ);
            else if(word == ",") {
                word = getWord(cmd);
                line.ajouter(word);

                word = getWord(cmd);
                if(word != "?")
                    erreur(1);

                line.setType(Commande::REQ);
            }else
                erreur(1);
        }else {
            line.ajouter(word);
            word = getWord(cmd);

            if(word == "rep") {
                line.setKey_word(Commande::REP);
                bool virgule = true;
                while(!(word = getWord(cmd)).empty()) {
                    if(word == "," && !virgule)
                        virgule = true;
                    else if(word != "," && virgule)
                        virgule = false;
                    else if(word == ".")
                        break;
                    else
                        erreur(1);

                    if(!virgule)
                        line.ajouter(word);
                }

                if(virgule || word != ".")
                    erreur(1);
                line.setType(Commande::CMD);
            }else if(word == "ce") {
                line.setKey_word(Commande::CE);

                word = getWord(cmd);

                line.ajouter(word);

                word = getWord(cmd);


                if(word == ".")
                    line.setType(Commande::CMD);
                else if(word == "?")
                    line.setType(Commande::REQ);
                else
                    erreur(1);

                if(!(word = getWord(cmd)).empty())
                    erreur(1);
            }else if(word == ".")
                line.setType(Commande::CMD);
            else
                erreur(1);
        }

        if(!getWord(cmd).empty())
            erreur(1);

        line.execution();
    }

    line.effacer_map();

/*
    cerr << "getword " << getword_/CLOCKS_PER_SEC << endl;
    cerr << "ajout element " << ajout_element_/CLOCKS_PER_SEC << endl;
    cerr << "rep cmd " << rep_cmd_/CLOCKS_PER_SEC << endl;
    cerr << "rep req 1 " << rep_req1_/CLOCKS_PER_SEC << endl;
    cerr << "rep req 2 " << rep_req2_/CLOCKS_PER_SEC << endl;
    cerr << "ce req " << ce_req_/CLOCKS_PER_SEC << endl;
    cerr << "ce cmd " << ce_cmd_/CLOCKS_PER_SEC << endl;
    cerr << "representant1 " <<  representant_/CLOCKS_PER_SEC << endl;
    cerr << "retirer " <<  retirer_/CLOCKS_PER_SEC << endl;
    cerr << "representant2 " <<  parnet_commun/CLOCKS_PER_SEC << endl;
    cerr << "estAlphabetic_" <<  estAlphabetic_/CLOCKS_PER_SEC << endl;
    cerr << "ajoutLigne" <<  ajout/CLOCKS_PER_SEC << endl;
    cerr << "traitement " <<  (decoup-getword_)/CLOCKS_PER_SEC << endl;
    cerr << "execution " <<  (executeur-ce_cmd_-ce_req_-rep_cmd_-rep_req1_-rep_req2_)/CLOCKS_PER_SEC << endl;
    cerr << t1/CLOCKS_PER_SEC << endl;*/
    return 0;
}