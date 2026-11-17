#!/bin/bash

# Demande des fichiers s'ils ne sont pas passes en arguments
if [ -z "$1" ]; then
    read -p "Fichier d'entree : " fichier_in
else
    fichier_in="$1"
fi

if [ -z "$2" ]; then
    read -p "Fichier oracle : " fichier_oracle
else
    fichier_oracle="$2"
fi

# Compilation
g++ -std=c++17 main.cpp -o moteur_tournoi

# Execution et diff
./moteur_tournoi < "$fichier_in" > resultat.out
diff resultat.out "$fichier_oracle"

# Nettoyage
rm -f resultat.out moteur_tournoi
