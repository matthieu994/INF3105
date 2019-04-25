/*
 * Histoire.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: Bruno Malenfant
 */

#include "Histoire.h"
#include "Phrase.h"

#include <algorithm>
#include <sstream>

Histoire::Histoire(void)
{
}

Histoire::Histoire(string a_titre) : _titre(a_titre)
{
}

Histoire::Histoire(const Histoire &a_histoire) : _titre(a_histoire._titre),
                                                 _phrases(a_histoire._phrases)
{
}

Histoire::~Histoire(void)
{
}

string Histoire::titre(void) const
{
    return _titre;
}

void Histoire::ajouterPhrase(Phrase a_phrase)
{
    _phrases.push_back(a_phrase);
}

vector<Phrase>::const_iterator Histoire::begin(void) const
{
    return _phrases.begin();
}

vector<Phrase>::const_iterator Histoire::end(void) const
{
    return _phrases.end();
}

bool estCaracterePhrase(char c)
{
    return '.' != c;
}

vector<Histoire *> *extraireHistoires(const DocumentXML &a_document)
{
    vector<Histoire *> *resultat = new vector<Histoire *>();

    Element *courrant = a_document.racine();
    vector<Contenu *>::const_iterator it = courrant->begin();

    // trouver <magasine>
    while (!(*it)->estElement())
        ++it;
    courrant = (Element *)(*it);

    it = courrant->begin();

    // trouver <contenus>
    while (!(*it)->estElement())
        ++it;
    courrant = (Element *)(*it);

    it = courrant->begin();
    vector<Contenu *>::const_iterator fin = courrant->end();

    for (; it < fin; ++it)
    {
        if ((*it)->estElement())
        {
            Element *histoire = (Element *)(*it);

            string titre = histoire->attribut(string("titre"));

            try
            {
                titre += " (partie " + histoire->attribut(string("partie")) + ")";
            }
            catch (AttributNonDefinie &a_e)
            {
                // rien
            }

            Histoire *nouvelle = new Histoire(titre);

            stringstream texte(histoire->texte());
            stringstream accumulateur;
            char c;

            while (!texte.eof())
            {
                do
                {
                    texte.get(c);
                } while (!estCaracterePhrase(c) && !texte.eof());

                while (estCaracterePhrase(c) && !texte.eof())
                {
                    accumulateur << c;
                    texte.get(c);
                }

                string phrase = accumulateur.str();
                accumulateur.str("");

                if (phrase.length() != 0)
                {
                    nouvelle->ajouterPhrase(Phrase(phrase));
                }
            }

            resultat->push_back(nouvelle);
        }
    }

    return resultat;
}

// Trouve l'histoire sélectionnée en argument
// histoires: liste des histoires lues
// titre: titre de l'histoire choisie
Histoire *findHistoire(const vector<Histoire *> histoires, const string titre)
{
    Histoire *match = nullptr;
    string titreCourant;

    for (Histoire *histoire : histoires)
    {
        titreCourant = histoire->titre();

        if (titre == titreCourant)
        {
            match = histoire;
            break;
        }

        size_t position = titreCourant.find(" (partie ");
        if (position != string::npos && titreCourant.substr(0, position) == titre)
        {
            // Partie 1 trouvée
            if (match == nullptr)
            {
                match = new Histoire(*histoire);
            }
            // Insérer Partie X dans Partie 1 et retirer derniere phrase de Partie 1
            else
            {
                match->phrases()->pop_back();
                match->phrases()->insert(match->phrases()->end(), histoire->phrases()->begin(), histoire->phrases()->end());
            }
        }
    }

    // Retire la dernière phrase (vide)
    if (match != nullptr)
        match->phrases()->pop_back();

    return match;
}
