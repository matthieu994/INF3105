/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Lab12
    Fichier de départ lab12.cpp © Éric Beaudry (beaudry.eric@uqam.ca).
    http://ericbeaudry.ca/INF3105/lab12/ */

#include <iostream>
#include <fstream>
#include "carte.h"

using namespace std;

int lab12(std::istream& is){
    // Lecture de la carte.
    Carte carte;
    is >> carte;
    
    set<long> noeuds_inaccessibles = carte.trouver_noeuds_inaccessibles();
    for(set<long>::iterator iter=noeuds_inaccessibles.begin();iter!=noeuds_inaccessibles.end();++iter){
        cout << 'n' << *iter << ' ';
    }
    cout << endl;
    cout << "Nombre de noeuds exclus : " << noeuds_inaccessibles.size() << endl;
    return 0;
}

int main(int argc, const char** argv)
{
    if(argc<=1)
        return lab12(cin);
    else{
        ifstream fichiercarte(argv[1]);
        if(fichiercarte.fail()){
            cout << "Erreur ouverture du fichier : " << argv[1] << endl;    
            return 1;
        }
        return lab12(fichiercarte);
    }
}
