#include <iostream>
#include <string>
#include <cstdlib>    // Requis pour rand()
#include <ctime>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

int main() {
    srand(time(0));

    vector<string> choix = {
        "batman", "maths", "pomme", "voiture"
    };

    string solution = choix[rand()%choix.size()];
    int tentatives = 7;
    string affichage = "";
    string lettres_trouvees = "";

    for (char l : solution) {
        affichage += "_ ";
    }
    cout << "Bienvenu dans le pendu" << endl;

    while (tentatives > 0) {
        cout << "\nMot a deviner: " << affichage << endl;
        string proposition;
        cout << "Proposez une lettre : ";
        if (!(cin >> proposition)){
            break;
        }

        char lettre = tolower(proposition[0]);
        if(solution.find(lettre) != string::npos) {
            if (lettres_trouvees.find(lettre) == string::npos) {
                lettres_trouvees += lettre;
            }
            cout << "Oui" << endl;
        } else {
            tentatives--;
            cout << "Non" << endl;

            if (tentatives == 0)
            cout << " ==========Y= " << endl;
            if (tentatives <= 1)
            cout << " ||/       |  " << endl;
            if (tentatives <= 2)
            cout << " ||/       0  " << endl;
            if (tentatives <= 3)
            cout << " ||/      /|\\" << endl;
            if (tentatives <= 4)
            cout << " ||       /|  " << endl;
            if (tentatives <= 5)
            cout << "/||           " << endl;
            if (tentatives <= 6)
            cout << "==============" << endl;

        }

        affichage = "";
        for(char x : solution) {
            if (lettres_trouvees.find(x) != string::npos) {
                affichage += x;
                affichage += ' ';
            } else {
                affichage +="_ ";
            }
        }

        if(affichage.find('_') ==string::npos){
            cout << "Win !!" << endl;
            break;
        }
    }
    if (tentatives == 0) {
        cout << "\nLost, le mot était :" << solution << endl;
    }
    cout << "\n Fin de partie" << endl;
    return 0;
}