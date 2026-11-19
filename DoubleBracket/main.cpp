/**
 *  @date : 22 octobre 2024
 *  @author : Alain Casali
 *  @Brief : une prmière réponse à la SAE 1.02 de 24/25
**/
#include <iostream>
#include <string>
#include <cstdlib>    // Requis pour rand()
#include <ctime>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

/**
 * @brief litUneString
 * @return la chaine lue sauf si :
 * (1) on une boulette sur l'entrée
 * (2) on trouve un commentaire sur l'entrée
 * le commentaire est matérialisé par la chaine "//"
 */


string litUneString (){
    string uneChaine;
    while (true){
        getline (cin, uneChaine);
        if (!cin || (uneChaine != "" && uneChaine[0] != '#')) break;
    }
    return uneChaine;
}

int litUnEntier (){
    string uneChaine = litUneString();
    if (uneChaine.size() >= 7 && uneChaine.substr(0, 7) == "Action:")
        return stoi(uneChaine.substr(7));
    return stoi(uneChaine);
}

struct participant {
    string nom;
    string prenom;
    unsigned score;
};

bool compare2part (const participant & p1, const participant & p2){
    return p1.prenom < p2.prenom;
}

void affichVectString (const vector<string> & v){
    for (const string & val : v)
        cout << val << '\t';
    cout << endl;
}

void affichVectParticipants (const vector<participant> & vPart){
    for (const participant & part : vPart){
        cout << part.nom << endl;
        cout << part.prenom << endl;
        cout << part.score << endl;
    }
}

// jeux 

// 0: pierre 1: feuille 2: ciseaux
participant pierreFeuilleCiseau (const participant & p1, const participant & p2) {
    int coup1 = litUnEntier();
    int coup2 = litUnEntier();

    if (coup1 == coup2){
        // match nul donc on rejoue
        return pierreFeuilleCiseau(p1, p2);
    }
    // coups
    if ((coup1 == 0 && coup2 == 2) ||
        (coup1 == 1 && coup2 == 0) ||
        (coup1 == 2 && coup2 == 1)){
        return p1;
    }
    return p2; // on retourne le gagnant
}
participant pileOuFace (const participant & p1, const participant & p2){
    int choix1 = litUnEntier();
    int choix2 = litUnEntier();

    if (choix1 == choix2){
        // on rejoue si meme choix
        return pileOuFace(p1, p2);
    }
    int resultat = rand() % 2;
    if (choix1 == resultat)
        return p1;
    return p2;
}

// Jeu 3: Nombre Secret (devine le nombre)
participant nombreSecret (const participant & p1, const participant & p2){
    int min, max, secret;
    min = 0;
    max = 20;
    secret = rand() % (max-min) + min;
    int guess1 = litUnEntier();
    int guess2= litUnEntier();
    int diff1 = abs(secret - guess1);
    int diff2 = abs(secret - guess2);

    if (diff1 == diff2)
        return (nombreSecret(p1, p2));

    else{
        if (diff1 < diff2)
            return p1;
        return p2;
    }
}

// Manche en BO3 (2 victoires pour gagner le match)
participant jouerMatch (const participant & p1, const participant & p2){
    unsigned v1 (0);
    unsigned v2 (0);

    // Jeu 1: PFC
    if (pierreFeuilleCiseau(p1, p2).nom == p1.nom)
        v1++;
    else
        v2++;

    // Jeu 2: Pile ou Face
    if (pileOuFace(p1, p2).nom == p1.nom)
        v1++;
    else
        v2++;

    // Jeu 3: seulement si egalite
    if (v1 == 1 && v2 == 1){
        if (nombreSecret(p1, p2).nom == p1.nom)
            v1++;
        else
            v2++;
    }

    if (v1 > v2) return p1;
    return p2;
}

void jouerMatchs (const vector<participant> & joueurs, vector<participant> & gagnants, vector<participant> & perdants){
    for (unsigned i = 0; i < joueurs.size(); i += 2){
        participant gagnant = jouerMatch(joueurs[i], joueurs[i+1]);
        if (gagnant.nom == joueurs[i].nom) {
            gagnants.push_back(joueurs[i]);
            perdants.push_back(joueurs[i+1]);
        }
        else {
            gagnants.push_back(joueurs[i+1]);
            perdants.push_back(joueurs[i]);
        }
    }
}

void jouerMatchsCroises (const vector<participant> & anciens, const vector<participant> & nouveaux, vector<participant> & gagnants, vector<participant> & elimines){
    if (anciens.size() != nouveaux.size()) {
        cout << "Erreur : les deux tableaux n'ont pas la meme taille." << endl;
        return;
    }
    for (unsigned i = 0; i < anciens.size(); ++i){
        participant gagnant = jouerMatch(anciens[i], nouveaux[i]);
        if (gagnant.nom == anciens[i].nom) {
            gagnants.push_back(anciens[i]);
            elimines.push_back(nouveaux[i]);
        }
        else {
            gagnants.push_back(nouveaux[i]);
            elimines.push_back(anciens[i]);
        }
    }
}

participant tournoiDoubleBracket(vector<participant> & joueurs){ 
    vector<participant> tableauGagnants = joueurs;
    vector<participant> tableauPerdants;

    while (tableauGagnants.size() > 1){
        vector<participant> nouveauxGagnants;
        vector<participant> nouveauxPerdants;
        // Matchs du winner bracket
        jouerMatchs(tableauGagnants, nouveauxGagnants, nouveauxPerdants);
        tableauGagnants = nouveauxGagnants;

        vector<participant> survivants;
        vector<participant> elimines;

        if (tableauPerdants.empty()){
            jouerMatchs(nouveauxPerdants, survivants, elimines);
        }
        else {
            while (tableauPerdants.size() > nouveauxPerdants.size()) {
                vector<participant> survivantsIntermediaires;
                vector<participant> eliminesIntermediaires;
                jouerMatchs(tableauPerdants, survivantsIntermediaires, eliminesIntermediaires);
                tableauPerdants = survivantsIntermediaires;
            }
            if(tableauPerdants.size() < nouveauxPerdants.size()){
                cout << "Erreur: effectifs incorrects dans le Loser Bracket" << endl;
                return joueurs[0];
            }
            if(tableauPerdants.size() == nouveauxPerdants.size()){
                jouerMatchsCroises(tableauPerdants, nouveauxPerdants, survivants, elimines);
            }
        }
        tableauPerdants = survivants;
    }
    // Grande finale
    participant restant1 = tableauGagnants[0];
    participant restant2 = tableauPerdants[0];
    participant gagnant = jouerMatch(tableauGagnants[0], tableauPerdants[0]);
    // Si le champion du looser bracket gagne on rejoue une finale
    if(gagnant.nom == tableauPerdants[0].nom)
        gagnant = jouerMatch(tableauGagnants[0], tableauPerdants[0]);
    return gagnant;
}

const unsigned KNbJoueurs (64); // On initialise le nombre de joueurs

vector<participant> lireParticipants (){
    vector<participant> v;
    for (unsigned i (0); i < KNbJoueurs; ++i){
        string ligne = litUneString();
        if (ligne.size() >= 7 && ligne.substr(0, 7) == "Joueur:")
            ligne = ligne.substr(7);
        v.push_back(participant{ligne, "", 0});
    }
    return v;
}

int main()
{
    srand(47);
    vector<participant> joueurs = lireParticipants();
    participant vainqueur = tournoiDoubleBracket(joueurs);
    cout << "1:" << vainqueur.nom << endl;
    return 0;
}
