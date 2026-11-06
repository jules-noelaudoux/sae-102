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

participant secretnumber (const participant & p1, const participant & p2){
    int min, max, secret;
    min = 0;
    max = 20;
    secret = rand() % (max-min) + min;
    cout << secret;
    int guess1 = litUnEntier();
    int guess2= litUnEntier();

    while (guess1 != secret && guess2 != secret){
        guess1 = litUnEntier();
        guess2= litUnEntier();
    }
    
    if (guess1 == guess2){return (secretnumber(p1, p2));}
    
    else{
        if (guess1 == secret){return (p1);}
        return p2;
    }
}
