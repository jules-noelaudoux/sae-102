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

/**
 * struct participant representant un participant
 * materialise avec son nom, son prenom et son score
 */
struct participant {
    string nom;
    string prenom;
    size_t score;
};

/**
 * Procedure prenant en parametre un tableau de strings
 * et qui affiche le contenu de celui-ci sur le terminal.
 */
void affichVectString (const vector<string> & v){
    for (const string & val : v)
        cout << val << '\t';
    cout << endl;
}

/**
 * Procedure prenant en parametre un tableau de participants
 * et qui affiche les attributs de ceux-ci.
 */
void affichVectParticipants (const vector<participant> & vPart){
    for (const participant & part : vPart){
        cout << part.nom << endl;
        cout << part.prenom << endl;
        cout << part.score << endl;
    }
}

// jeux 

/** --- Jeu 1: PFC --- 
* - Entrées joueurs: (0: pierre, 1: feuille, 2: ciseaux)
* - pierre(0) bat ciseaux(2) ; feuille(1) bat pierre(0) ; ciseaux(2) bat feuille(1)
*/
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

/** --- JEU 2: Pile ou Face ---
* - Entrées joueurs: (0: pile, 1: face)
* - Les 2 joueurs doivent choisir des faces différentes, sinon on rejoue 
* - La pièce est lancée (avec rand): celui qui a choisi le bon côté gagne
*/
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


/** --- JEU 3: Nombre Secret ---
* (il départage les joueurs aprés qu'il y ait 1 partout).
* Un nombre secret est tiré dans l'interval [min, max[
* Les joueurs donne chacun un nombre en fonction du nombre, celui le plus proche du nombre secret gagne
* Si les deux ont le meme nombre choisi ou alors la meme distance avec le nombre secret, on relance le jeu jusqu'a avoir un gagnant
*/
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

/**
* Fonction prenant en parametre deux joueurs 
* et qui execute les differents jeux pour ces deux joueurs
* si egalite a la fin des deux premiers jeux, on en joue un troisieme
* elle renvoie le gagnant entre les deux joueurs 
*/
participant jouerMatch (const participant & p1, const participant & p2){
    size_t v1 (0);
    size_t v2 (0);

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

/**
* Procedure prenant en parametre les joueurs en jeu, et leur positionnement dans le classement (tableau Winner ou Looser)
* et qui repartie les differents joueurs en fonction de leurs résultats
* elle permet notamment que lorsque la taille des deux tableaux (Winner/ Looser) est différente, de pouvoir réajuster les tailles 
* en faisant un tour qu'avec les joueurs dans le tableau des perdants
*/
void jouerMatchs (const vector<participant> & joueurs, vector<participant> & gagnants, vector<participant> & perdants){
    for (size_t i = 0; i < joueurs.size(); i += 2){
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

/**
* Procedure prenant en parametre les joueurs en jeu,et leur positionnement dans le classement (tableau Winner ou Looser)
* et qui repartie les differents joueurs en fonction de leurs résultats
* lorsque la taille du tableau des winners et égale à celle du tableau des Loosers
*/
void jouerMatchsCroises (const vector<participant> & anciens, const vector<participant> & nouveaux, vector<participant> & gagnants, vector<participant> & elimines){
    if (anciens.size() != nouveaux.size()) {
        cout << "Erreur : les deux tableaux n'ont pas la meme taille." << endl;
        return;
    }
    for (size_t i = 0; i < anciens.size(); ++i){
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

vector<participant> tournoiDoubleBracket(vector<participant> & joueurs){
    vector<participant> tableauGagnants = joueurs;
    vector<participant> tableauPerdants;
    participant troisieme;
    participant quatrieme;

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
                if (tableauPerdants.size() == 2 && eliminesIntermediaires.size() == 1) {
                    quatrieme = eliminesIntermediaires[0];
                }
                tableauPerdants = survivantsIntermediaires;
                
            }
            if(tableauPerdants.size() < nouveauxPerdants.size()){
                cout << "Erreur: effectifs incorrects dans le Loser Bracket" << endl;
                return {joueurs[0]};
            }
            if(tableauPerdants.size() == nouveauxPerdants.size()){
                jouerMatchsCroises(tableauPerdants, nouveauxPerdants, survivants, elimines);
            }
            if (tableauPerdants.size() == 1 && elimines.size() == 1) {
                    troisieme = elimines[0];
                }
        }
        tableauPerdants = survivants;
    }
    // Grande finale;
    participant gagnant = jouerMatch(tableauGagnants[0], tableauPerdants[0]);
    // Si le champion du looser bracket gagne on rejoue une finale
    if(gagnant.nom == tableauPerdants[0].nom)
        gagnant = jouerMatch(tableauGagnants[0], tableauPerdants[0]);

    participant deuxieme = (gagnant.nom == tableauGagnants[0].nom) ? tableauPerdants[0] : tableauGagnants[0];
    
    return {gagnant, deuxieme, troisieme, quatrieme}    ;
}

const size_t KNbJoueurs (64); // On initialise le nombre de joueurs

/**
* Lit les 8 participants depuis l'entrée (sous format "Joueur:Nom" ou "Nom").
* Et leur score initialisé à 0
*/
vector<participant> lireParticipants (){
    vector<participant> v;
    for (size_t i (0); i < KNbJoueurs; ++i){
        string ligne = litUneString();
        if (ligne.size() >= 7 && ligne.substr(0, 7) == "Joueur:")
            ligne = ligne.substr(7);
        v.push_back(participant{ligne, "", 0});
    }
    return v;
}

int main()
{
    srand(47); // Graine fixe (arbitrairement 47) : pour les tirages aléatoires (pile ou face ou nombreSecret)
    vector<participant> joueurs = lireParticipants();
    vector<participant> top4 = tournoiDoubleBracket(joueurs);
    // Affichage du classement des 4 premiers
    for (size_t i = 0; i < top4.size(); ++i) {
        cout << (i + 1) << ":" << top4[i].nom << endl;
    }
    return 0;
}
