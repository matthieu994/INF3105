
#include "DocumentXML.h"
#include "Histoire.h"
#include "Graphe.h"

#include <algorithm>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

vector<Histoire *> *lireDocuments(string a_nomFichier)
{
    vector<Histoire *> *histoires = new vector<Histoire *>();
    DocumentXML *listeFichiers = lireFichierXML(a_nomFichier);

    Element *courrant = listeFichiers->racine();
    vector<Contenu *>::const_iterator it = courrant->begin();

    // trouver <liste>
    while (!(*it)->estElement())
        ++it;
    courrant = (Element *)(*it);

    for (Contenu *contenu : *courrant)
    {
        if (contenu->estElement())
        {
            Element *element = (Element *)contenu;

            DocumentXML *doc = lireFichierXML(element->attribut(string("fichier")));

            vector<Histoire *> *h = extraireHistoires(*doc);

            histoires->insert(histoires->end(), h->begin(), h->end());
        }
    }

    return histoires;
}

bool estCaractereMot(char c);

int main(int argc, char const *argv[])
{
    vector<Histoire *> *histoires = lireDocuments(string("listeDocument.xml"));

    if (argv[1] == nullptr)
    {
        cerr << "Usage: " << argv[0] << " <titre>" << endl;
        exit(1);
    }

    string titre = argv[1];
    Histoire *histoire;

    histoire = findHistoire(*histoires, titre);

    if (histoire == nullptr)
    {
        cerr << "L'histoire '" << titre << "' n'existe pas." << endl;
        exit(1);
    }
    else
    {
        Graphe<Phrase> graphe;
        cout << "Histoire '" << titre << "' trouvée. Nombre phrases: " << histoire->end() - histoire->begin() << endl;
        graphe.construireGraphe(histoire);

        map<int, Phrase> path;
        cout << graphe.trouverChemin(5, histoire->phrases().front(), histoire->phrases().back(), path) << endl;

        for (size_t i = 1; i <= 5; i++)
        {
            cout << path[i].ordonnee() << endl;
        }
    }
}
