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

participant pierreFeuilleCiseau (const participant & p1, const participant & p2) {
    string entreeP1, entreeP2;
    cout << "Joueur1 : quel coup ? : ";
    cin >> entreeP1;
    cout << "Joueur2 : quel coup ? : ";
    cin>> entreeP2;
    if (entreeP1 == entreeP2){
        cout << "égalité" << endl;
        return pierreFeuilleCiseau (p1, p2);
        }
    else if (entreeP1 == "puit") return p1;
    else if (entreeP2 == "puit") return p2;
    else if ((entreeP1 == "pierre") && (entreeP2 == "ciseau")) return p1;
    else if ((entreeP1 == "pierre") && (entreeP2 == "feuille")) return p2;
    else if ((entreeP1 == "ciseau") && (entreeP2 == "pierre")) return p2;
    else if ((entreeP1 == "ciseau") && (entreeP2 == "feuille")) return p1;
    else if ((entreeP1 == "feuille") && (entreeP2 == "pierre")) return p1;
    else if ((entreeP1 == "feuille") && (entreeP2 == "ciseau")) return p2;
}

participant pileOuFace (participant & p1, participant & p2){ 
    string entreeP1, entreeP2;
    cout << "Joueur 1 : Pile ou Face ?";
    cin >> entreeP1;
    cout << "Joueur 2 : Pile ou Face ?";
    cin >> entreeP2;
    for (char &c : entreeP1)
        c = tolower(c);
    for (char &c : entreeP2)
        c = tolower(c);
    srand(47);
    int resultat = rand() % 2;
    cout << (resultat == 0 ? "\nC'etait pile !" : "\nC'etait face !") << endl;

    if (entreeP1 == entreeP2) {
        cout << "Egalite !" << endl;
        return p1;
    }
    else if ((resultat == 0 && entreeP1 == "pile") || ((resultat == 1 && entreeP1 == "face"))){
        cout << "Joueur 1 a gagne !" <<endl;
        return p1;
    }
    else {
        cout << "Joueur 2 a gagne !" << endl;
        return p2;
    }
}

participant secretNumber (participant & p1, participant & p2){ 
    int secret;
    int guess;
    unsigned cpt = 0;
    
    cout << "Joueur 1 : entre ton nombre : ";
    cin >> secret;
    cout << "Joueur 2 : devine le nombre : ";
    cin >> guess;
    ++cpt;
    
    while ((guess != secret) && (cpt < 5)){
        if (guess < secret)
            cout << "Le nombre secret est plus grand" << endl;
        else
            cout <<"Le nombre secret est plus petit" << endl;
        cin >> guess;
        ++ cpt;
    }
    if (guess == secret){
    cout << "Bravo tu as trouvé le nombre secret en " << cpt << " tentatives" << endl;
    return p2;
    }
    else {
        cout << "Perdu" << endl;
        return p1;
    }
}

participant manche (participant & p1, participant & p2){
    for (unsigned i (0); i < 3; ++i) {
        if (p1.nom == pierreFeuilleCiseau(p1, p2).nom)
            ++p1.score;
        else
            ++p2.score;
    }
    if (p1.score>p2.score) return p1;
    return p2;
}

int main()
{
    return 0;
}
