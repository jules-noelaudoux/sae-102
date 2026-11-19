/**  ----- ROUND ROBIN -----
* Tournoi en Round Robin => tout le monde affronte tout le monde.
* 
* Fonctionnement général:
*   - 8 joueurs sont lus sur l'entrée standard
*   - Chaque match oppose 2 joueurs sur une série de jeux:
*        1. Pierre-Feuille-Ciseaux
*        2. Pile ou Face
*        3. Nombre Secret (joué uniquement si les 2 joeurs ont une victoire chacun)
*   - Le vainqueur d'un match gagne 1 point au tournoi.
*   - Le classement final est trié par score décroissant de score, puis par nom (ordre alphabétique).
* 
* Les coups des joueurs sont lus sur l'entrée standard, dans l'ordre où le moteur les demande.
* Nous partons du principe que:
*    - Tous les joueurs ont un nom différents
*    - Le programme est exécuté sur un ordinateur sous Linux
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

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
        return stoi(uneChaine.substr(7)); // on renvoie le nombre sans la chaine "Action:"
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
 * Fonction compare2part() prenant en paramètres
 * deux participants et renvoie si le participant 1
 * a le meilleur score ou non.
 * Si egalite alors on classe en fonction du nom
 * par ordre alphabetique.
 */
bool compare2part (const participant & p1, const participant & p2){
    if (p1.score != p2.score)
        return p1.score > p2.score;
    return p1.nom < p2.nom; // gestion des egalites
}

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

/** --- Jeu 1: PFC --- 
* - Entrées joueurs: (0: pierre, 1: feuille, 2: ciseaux)
* - pierre(0) bat ciseaux(2) ; feuille(1) bat pierre(0) ; ciseaux(2) bat feuille(1)
*/
participant pierreFeuilleCiseau (const participant & p1, const participant & p2) {
    int coup1 = litUnEntier();
    int coup2 = litUnEntier();

    if (coup1 == coup2){
        // match nul donc on rejoue
        return pierreFeuilleCiseau(p1, p2);
    }
    // 0 bat 2; 1 bat 0; 2 bat 1
    if ((coup1 == 0 && coup2 == 2) ||
        (coup1 == 1 && coup2 == 0) ||
        (coup1 == 2 && coup2 == 1)){
        return p1;
    }
    return p2;
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
        // meme choix, on rejoue
        return pileOuFace(p1, p2);
    }
    //On lance la pièce (0: pile, 1: face):
    int resultat = rand() % 2;
    if (choix1 == resultat)
        return p1;
    return p2;
}

/** --- JEU 3: Nombre Secret ---
* (il départage les joueurs aprés qu'il y ait 1 partout).
* Un nombre secret est tiré dans l'interval [min, max[
* Les joueurs donne chacun un nombre à chaque tour, jusqu'à ce que l'un d'entre eux trouve le nombre secret
* Si les deux le trouvent en même temps (au même tour), la manche est annulée et ils en rejouent une jusqu'à avoir un gagnant
*/
participant nombreSecret (const participant & p1, const participant & p2){
    int min, max, secret;
    min = 0;
    max = 20;
    secret = rand() % (max-min) + min;

    // 1ere proposition de chaque joueur
    int guess1 = litUnEntier();
    int guess2= litUnEntier();

    // Tant que personne n'a trouvé, on lit un nouveau tour de propositions.
    while (guess1 != secret && guess2 != secret){
        guess1 = litUnEntier();
        guess2= litUnEntier();
    }

    // ICI => au moins un joueur a trouvé le nombre secret
    // Si guess1 == guess2, c'est que les deux ont trouvé en même temps: donc égalité => on rejoue
    if (guess1 == guess2)
        return (nombreSecret(p1, p2));

    // Sinon on retourne le gagnant:
    else{
        if (guess1 == secret)
            return (p1);
        return p2;
    }
}

void jouerMatch (participant & p1, participant & p2){
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

    // On fait jouer le 3eme jeu uniquement si aucun des deux joueurs n'a déjà gagné les 2 premiers.
    if (v1 == 1 && v2 == 1){
        // Jeu 3: Nombre Secret
        if (nombreSecret(p1, p2).nom == p1.nom) 
            v1++;
        else 
            v2++;

        // Le gagnant du match gagne 1 point au tournoi
        if (v1 > v2) 
            p1.score++;
        else 
            p2.score++;
    }
    else{
        if (v1 == 2) 
            p1.score++;
        else 
            p2.score++;
    }
    
}

// Lit les 8 participants depuis l'entrée (sous format "Joueur:Nom" ou "Nom").
// Et leur score initialisé à 0
vector<participant> lireParticipants (){
    const size_t KNbJoueurs (8);
    vector<participant> v;
    for (size_t i (0); i < KNbJoueurs; ++i){
        string ligne = litUneString();
        // Retire le début "Joueur:" (7 caractères) si il est présent
        if (ligne.size() >= 7 && ligne.substr(0, 7) == "Joueur:")
            ligne = ligne.substr(7);
        v.push_back(participant{ligne, "", 0});
    }
    return v;
}

/**
 * Tournoi Round Robin: chaque participant affronte une seule fois chacun des autres
 * La double boucle for (j commence à i+1) génère chaque paire (i, j) une seule fois
 * soit n*(n-1)/2 matchs ( avec 8 joueurs ça nous fait 28 matchs).
 * A la fin, le vecteur est trié pour obtenir le classement! final.
 */
void tournoiRoundRobin (vector<participant> & vPart){
    for (size_t i=0; i < vPart.size(); ++i){
        for (size_t j (i + 1); j < vPart.size(); ++j){
            jouerMatch(vPart[i], vPart[j]);
        }
    }
    // Classement final: tri par score décroissant, puis par ordre alphabétique pour les scores égaux.
    sort(vPart.begin(), vPart.end(), compare2part);
}

int main (){
    srand(47); // Graine fixe (arbitrairement 47): pour les tirages aléatoires (pile ou face et nombre secret)

    vector<participant> vParticipants = lireParticipants();

    tournoiRoundRobin(vParticipants);

    // Affichage du classement comme "rang:nom"
    for (size_t i (0); i < vParticipants.size(); ++i){
        cout << (i + 1) << ":" << vParticipants[i].nom << endl;
    }

    return 0;
}
