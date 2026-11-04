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
        if ((!cin) || (uneChaine.substr(0,2) != "//")) break;
    }
    return uneChaine;
}

int litUnEntier (){
    string uneChaine;
    while (true){
        getline (cin, uneChaine);
        if ((!cin) || (uneChaine.substr(0,2) != "//")) break;
    }
    return stoi(uneChaine);
}

struct participant {
    string nom;
    string prenom;
    int score;
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

participant pierreFeuilleCiseau (const participant & p1, const participant & p2) {
    string entreeP1, entreeP2;
    cout << "Joueur1 : quel coup ? : ";
    cin >> entreeP1;
    cout << "Joueur2 : quel coup ? : ";
    cin>> entreeP2;
    if (entreP1 == entreP2){
        cout << "égalité" << endl;
        pierreFeuilleCiseau (const participant & p1, const participant & p2);
    else if (entreeP1 == "puit") return p1;
    else if (entreeP2 == "puit") return p2;
    else if ((entreeP1 == "pierre") && (entreeP2 == "ciseau")) return p1;
    else if ((entreeP1 == "pierre") && (entreeP2 == "feuille")) return p2;
    else if ((entreP1 == "ciseau") && (entreP2 == "pierre")) return p2;
    else if ((entreP1 == "ciseau") && (entreP2 == "feuille")) return p1;
    else if ((entreP1 == "feuille") && (entreP2 == "pierre")) return p1;
    else if ((entreP1 == "feuille") && (entreP2 == "ciseau")) return p1;
}

participant manche (participant & p1, participant & p2){
    for (unsigned i (0); i < 3; ++i) {
        if (p1.nom == pierreFeuilleCiseau.nom(p1, p2))
            ++p1.score;
        else
            ++p2.score;
    }
    if (p1.score>p2.score) return p1;
    return p2;
}

participant manche2 (participant & p1, participant & p2){   #retravailler
    srand(time(0));
    string choix;
    cout << "Pile ou Face ?";
    cin >> choix;
    for (char &c : choix)
        c = tolower(c);
    int resultat = rand() % 2;
    cout << (resultat == 0 ?"\nC'etait pile !" : "\nC'etait face !") << endl;
    if((resultat == 0 && choix == "pile") || (resultat == 1 && choix == "face"))
        cout << "Gagné !" << endl;
    else
        cout << " Perdu !" << endl;
    if (p1.score>p2.score) return p1;
    return p2;
}

int main()
{
    //cout << "Hello World!" << endl;
    //vector<participant> vParticipant (19);
    /* on va detecter les glaces preférées des votants
    * tout en virant les commentaires
    */

    /* on vote sur 4 glaces */
    vector <string> vGlacePref;
    for (unsigned i (0); i < 4; ++i)
        vGlacePref.push_back(litUneString());

/* debug */
    affichVectString (vGlacePref);

    //On lit les datas du clavier, et on les stocke
/*    vector<participant> vParticipant;

    for (unsigned i (0); i < 10; ++i){
        string nom (litUneString());
        string prenom  (litUneString());
        int numGlace (litUnEntier());
        //cout << nom << endl << prenom << endl << numGlace << endl;
        vParticipant.push_back(participant{nom, prenom, numGlace});
              //  vJoueur[numEquipe-1].push_back(joueur {nom, prenom, numEquipe});
    }

    affichVectParticipants(vParticipant);
    cout << string (15, '-') << endl;
    sort (vParticipant.begin(), vParticipant.end(), compare2part);
    affichVectParticipants(vParticipant);
*/

        cout << "c'est la glace " << vGlacePref[0] << " qui a gagne" << endl;
    return 0;
}
