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

// 0: pierre, 1: feuille, 2: ciseaux
participant pierreFeuilleCiseau (const participant & p1, const participant & p2) {
    int coup1 = litUnEntier();
    int coup2 = litUnEntier();

    if (coup1 == coup2){
        // match nul donc on rejoue
        return pierreFeuilleCiseau(p1, p2);
    }
    // 0 bat 2, 1 bat 0, 2 bat 1
    if ((coup1 == 0 && coup2 == 2) ||
        (coup1 == 1 && coup2 == 0) ||
        (coup1 == 2 && coup2 == 1)){
        return p1;
    }
    return p2;
}
participant pileOuFace (const participant & p1, const participant & p2){
    int choix1 = litUnEntier();
    int choix2 = litUnEntier();

    if (choix1 == choix2){
        // meme choix, on rejoue
        return pileOuFace(p1, p2);
    }
    int resultat = rand() % 2;
    if (choix1 == resultat)
        return p1;
    return p2;
}

// Jeu 3 : Secret Number (p1 choisit le nb, p2 tente de deviner en 5 essais max)
participant secretNumber (const participant & p1, const participant & p2){
    int secret = litUnEntier();
    int guess = litUnEntier();
    unsigned cpt (1);

    while ((guess != secret) && (cpt < 5)){
        guess = litUnEntier();
        ++cpt;
    }
    if (guess == secret)
        return p2;
    return p1;
}
participant jeu(const participant & joueur1, const participant & joueur2, int numeroJeu){
    if (numeroJeu == 1)
        return pierreFeuilleCiseaux(joueur1, joueur2);
    else if (numeroJeu == 2)
        return pileOuFace(joueur1, joueur2);
    if (numeroJeu == 3)
        return secretNumber(joueur1, joueur2);
}
void jouerMatchs (const vector<participant> & joueurs, vector<participant> & gagnants, vector<participant> & perdants){
    for (unsigned i = 0; i < joueurs.size(); i +=2){
        participant gagnant = jeu(joueurs[i], joueurs[i+1],numeroJeu);
        if (gagnant.nom == joueurs[i].nom && gagnant.prenom == joueurs[i].prenom) {
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
    for (unsigned i = 0; i < anciens.size(); ++i){
        participant gagnant = jeu(anciens[i], nouveaux[i]);
        if (gagnant.nom == anciens[i].nom && gagnant.prenom == anciens[i].prenom) {
            gagnants.push_back(anciens[i]);
            elimines.push_back(nouveaux[i]);
        }
        else {
            gagnants.push_back(nouveaux[i]);
            elimines.push_back(anciens[i]);
        }
    }
}

void tournoiDoubleBracket(vector<participant> & joueurs, int numeroJeu){ 
    vector<participant> tableauGagnants = vPart;
    vector<participant> tableauPerdants;
    vector<vector<participant>> eliminesParTour; // tableau qui contient plusieurs tableaux afin de conserver les joueurs éliminés à chaque tour séparement

    while (taleauGagnants.size() > 1){
        vector<participant> nouveauxGagnants;
        vector<participant> nouveauxPerdants;

        vector<participant> vide;
        jouerMatchs(tableauGagnants, vide, nouveauxGagnants, nouveauxPerdants);
        tableauGagnants = nouveauxGagnants;

        vector<participant> survivants;
        vector<participant> elimines;

        if (tableauPerdants.empty()){
            jouerMatchs(nouveauxPerdants, vide, survivants, elimines);
            eliminesparTour.push_back(elimines);
        }
        else if (tableauPerdants.size() == nouveauxPerdants.size()){
            jouerMatchs(tableauPerdants, nouveauxPerdants, survivants, elimines);
            eliminesparTour.push_back(elimines);
        }
        else {
            vector<participant> survivantsIntermediaires;
            jouerMatchs(tableauPerdants, vide, survivantsIntermediaires, elimines);
            eliminesparTour.push_back(elimines);
            elimines.clear();
            jouerMatchs(survivantsIntermediaires, nouveauxPerdants, survivants, elimines);
            eliminesparTour.push_back(elimines);
        }
        tableauPerdants = survivants;
    }
}

int main()
{
    return 0;
}
