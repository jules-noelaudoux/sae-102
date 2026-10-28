#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
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

    return 0;
}