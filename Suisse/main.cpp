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
        return stoi(uneChaine.substr(7));  // on renvoie le nombre sans la chaine "Action:"
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
    return p1.nom < p2.nom;  // gestion des egalites
}

/**
 * Procedure affichVectString()qui prend en parametre un tableau
 * de strings et qui affiche le contenu de celui-ci sur le terminal.
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
* Entrées joueurs: (0: pierre, 1: feuille, 2: ciseaux)
* pierre(0) bat ciseaux(2) ; feuille(1) bat pierre(0) ; ciseaux(2) bat feuille(1)
*/
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
* Entrées joueurs: (0: pile, 1: face)
* Les 2 joueurs doivent choisir des faces différentes, sinon on rejoue
* La pièce est lancée (avec rand): celui qui a choisi le bon côté gagne
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
* Les joueurs donne chacun un nombre à chaque tour, jusqu'à ce que l'un d'entre eux trouve le nombre secret
* Si les deux le trouvent en même temps (au même tour), la manche est annulée et ils en rejouent une jusqu'à avoir un gagnant
*/
participant nombreSecret (const participant & p1, const participant & p2){
    int min, max, secret;
    min = 0;
    max = 20;
    secret = rand() % (max-min) + min;
    int guess1 = litUnEntier();
    int guess2= litUnEntier();

    while (guess1 != secret && guess2 != secret){
        guess1 = litUnEntier();
        guess2= litUnEntier();
    }
    
    if (guess1 == guess2){return (nombreSecret(p1, p2));}

    else{
        if (guess1 == secret){return (p1);}
        return p2;
    }
}

// Chaque manche fait jouer les 3 jeux aux deux joueurs, je suis pas sur que c'est ca
void jouerMatch (participant & p1, participant & p2){
    // 1pt par jeu
    // Jeu 1: PFC
    if (pierreFeuilleCiseau(p1, p2).nom == p1.nom)
        p1.score++;
    else
        p2.score++;

    // Jeu 2: Pile ou Face
    if (pileOuFace(p1, p2).nom == p1.nom)
        p1.score++;
    else
        p2.score++;

    // Jeu 3: Nombre Secret
    if (nombreSecret(p1, p2).nom == p1.nom)
        p1.score++;
    else
        p2.score++;
}

bool ontDejaJoue (const string & nom1, const string & nom2, const vector<string> & historique){
    string match1 = nom1 + "-" + nom2;
    string match2 = nom2 + "-" + nom1;
    for (size_t i (0); i < historique.size(); ++i){
        if (historique[i] == match1 || historique[i] == match2)
            return true;
    }
    return false;
}

vector<participant> lireParticipants (){
    const size_t KNbJoueurs (16); // On initialise le nombre de joueurs
    vector<participant> v;
    for (size_t i (0); i < KNbJoueurs; ++i){
        string ligne = litUneString();
        if (ligne.size() >= 7 && ligne.substr(0, 7) == "Joueur:")
            ligne = ligne.substr(7);
        v.push_back(participant{ligne, "", 0});
    }
    return v;
}

/* Procedure tournoiSuisse() prenant un tableau de
 * participants par reference, simule un tournoi suisse
 * en 5 rondes et trie le tableau selon le classement final.
 */
void tournoiSuisse (vector<participant> & vPart){
    const size_t KNbRondes (5);  // Nombre de rondes
    vector<string> historique;  // Historique des matchs deja joues

    for (size_t r (0); r < KNbRondes; ++r){
        // On trie par score decroissant avant chaque ronde,
        // on utilise le critere de classement donne par la
        // fonction compare2part()
        sort(vPart.begin(), vPart.end(), compare2part);

        // Tableau indiquant si chaque participant a deja joue dans cette ronde
        vector<bool> apparie(vPart.size(), false);

        for (size_t i (0); i < vPart.size(); ++i){
            if (!apparie[i]){
                bool trouve (false);
                size_t adv (0);
                size_t j (i + 1);

                // On cherche le premier adversaire disponible pas encore affronte
                while (j < vPart.size() && !trouve){
                    if (!apparie[j] && !ontDejaJoue(vPart[i].nom, vPart[j].nom, historique)){
                        adv = j;
                        trouve = true;
                    }
                    ++j;
                }

                // Si tous ont deja ete affrontes, on prend le premier disponible
                if (!trouve){
                    j = i + 1;
                    while (j < vPart.size() && !trouve){
                        if (!apparie[j]){
                            adv = j;
                            trouve = true;
                        }
                        ++j;
                    }
                }

                if (trouve){
                    apparie[i] = true;
                    apparie[adv] = true;
                    jouerMatch(vPart[i], vPart[adv]);
                    historique.push_back(vPart[i].nom + "-" + vPart[adv].nom);
                }
            }
        }
    }

    // Classement final
    sort(vPart.begin(), vPart.end(), compare2part);
}

int main (){
    srand(47);  // Initialisation d'une graine arbitraire

    // Initialisation d'un tableau de participants lus dans le fichier entree
    vector<participant> vParticipants = lireParticipants();

    // Deroulement du tournoi suisse et tri final des participants
    tournoiSuisse(vParticipants);

    // On affiche les participants dans l'ordre du classement.
    for (size_t i (0); i < vParticipants.size(); ++i){
        cout << (i + 1) << ":" << vParticipants[i].nom << endl;  // Affichage sous la forme : "[place]:[nom]"
    }

    return 0;
}
