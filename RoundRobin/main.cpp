/**  ----- ROUND ROBIN -----
 Tournoi en Round Robin => tout le monde affronte tout le monde.
 
 Fonctionnement général:
   - 8 joueurs sont lus sur l'entrée standard
   - Chaque match oppose 2 joueurs sur une série de jeux:
        1. Pierre-Feuille-Ciseaux
        2. Pile ou Face
        3. Nombre Secret (joué uniquement si les 2 joeurs ont une victoire chacun)
   - Le vainqueur d'un match gagne 1 point au tournoi.
   - Le classement final est trié par score décroissant de score, puis par nom (ordre alphabétique).
 
 Les coups des joueurs sont lus sur l'entrée standard, dans l'ordre où le moteur les demande.
 Nous partons du principe que:
    - Tous les joueurs ont un nom différents
    - Le programme est exécuter sur un ordinateur sous Linux
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

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
    if (p1.score != p2.score)
        return p1.score > p2.score;
    return p1.nom < p2.nom;
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

// Jeu 1 : PFC (0: pierre, 1: feuille, 2: ciseaux)
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

// Jeu 2 : Pile ou Face (0: pile, 1: face)
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

/** --- JEU 3: Nombre Secret ---
(il départage les joueurs aprés qu'il y ait 1 partout).
Un nombre secret est tiré dans l'interval [min, max[
Les joueurs donne chacun un nombre à chaque tour, jusqu'à ce que l'un des deux trouve le nombre secret
Si les deux le trouvent en même temps (au même tour), la manche est annulée et ils en rejouent une jusqu'à avoir un gagnant
 */
participant NombreSecret (const participant & p1, const participant & p2){
    int min, max, secret;
    min = 0;
    max = 20;
    secret = rand() % (max-min) + min;

    // Première proposition de chaque joueur
    int guess1 = litUnEntier();
    int guess2= litUnEntier();

    while (guess1 != secret && guess2 != secret){
        guess1 = litUnEntier();
        guess2= litUnEntier();
    }
    
    if (guess1 == guess2){return (NombreSecret(p1, p2));}
    
    else{
        if (guess1 == secret){return (p1);}
        return p2;
    }
}

void jouerMatch (participant & p1, participant & p2){
    unsigned v1 (0);
    unsigned v2 (0);

    // Jeu 1 : PFC
    if (pierreFeuilleCiseau(p1, p2).nom == p1.nom) {v1++;}
    else {v2++;}

    // Jeu 2 : Pile ou Face
    if (pileOuFace(p1, p2).nom == p1.nom) {v1++;}
    else{v2++;}

    // On fait jouer le 3eme jeu uniquement si aucun des deux joueurs n'a déjà gagné les 2 premiers.
    if (v1 == 1 && v2 == 1){
        // Jeu 3 : Nombre Secret
        if (NombreSecret(p1, p2).nom == p1.nom){v1++;}
        else{v2++;}

        // Le gagnant du match gagne 1 point au tournoi
        if (v1 > v2){p1.score++;}
        else{p2.score++;}
    }
    else{
        if (v1 == 2){p1.score++;}
        else{p2.score++;}
    }
    
}

vector<participant> lireParticipants (){
    const unsigned KNbJoueurs (8);
    vector<participant> v;
    for (unsigned i (0); i < KNbJoueurs; ++i){
        string ligne = litUneString();
        if (ligne.size() >= 7 && ligne.substr(0, 7) == "Joueur:")
            ligne = ligne.substr(7);
        v.push_back(participant{ligne, "", 0});
    }
    return v;
}

void tournoiRoundRobin (vector<participant> & vPart){
    for (unsigned i=0; i < vPart.size(); i++){
        for (unsigned j (i + 1); j < vPart.size(); j++){
            jouerMatch(vPart[i], vPart[j]);
        }
    }
    sort(vPart.begin(), vPart.end(), compare2part);
}

int main (){
    srand(47);

    vector<participant> vParticipants = lireParticipants();

    tournoiRoundRobin(vParticipants);

    for (unsigned i (0); i < vParticipants.size(); ++i){
        cout << (i + 1) << ":" << vParticipants[i].nom << endl;
    }

    return 0;
}
