#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

int main() {
    srand(time(0));
    string choix;
    cout << "Pierre, feuille ou ciseaux ?";
    cin >> choix;
    for (char &c : choix)
    c = tolower(c);
    int resultat = rand() % 3;
    string ordi;

    if (resultat == 0) 
        ordi = "pierre";
    else if (resultat == 1)
        ordi = "feuille";
    else if (resultat == 2)
        ordi = "ciseaux";

    cout << "L'oridnateur a choisi : " << ordi << endl;

    if (choix == ordi)
        cout << "Egalite !" << endl;
    else if ((choix == "pierre" && ordi == "ciseaux") || 
             (choix == "feuille" && ordi == "pierre") || 
             (choix == "ciseaux" && ordi == "feuille"))
        cout << "Gagne !" << endl;
    else
        cout << "Perdu !" << endl;

    return 0;
}