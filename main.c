//Nathan MARTORANA
//Pierre BOTHUA
//
//Projet GIF
//Automne 2022

#include <stdio.h>
#include <stdio.h>
#include <malloc.h>
//#include <windows.h>

//Structure des villes
typedef struct ville {
    int codePostal;
    struct ville * villeSuivante;
} Tville;

typedef Tville * PTville;


//Structure des lignes electriques
typedef struct lignesElectrique {
    int puissance;
    // pointeur sur une ville
    PTville villeDesservie;
    // liste simplement chainée
    struct lignesElectrique * ligneSuivante ;
} TlignesElectrique;

typedef TlignesElectrique * PTligneElectrique;


//Structure des centrales electriques
typedef struct centrale{
    int codeCentrale;
    int puissance_max;
    int puissance_restante;
    // Pointeur sur la liste des lignes
    PTligneElectrique villeDependante;
    // Liste doublement chainée
    struct centrale * ptsuivant;
    struct centrale * ptprecedent;
}Tcentrale;

typedef Tcentrale * PTcentrale;

void creation_test(PTcentrale pPremiereCentrale, PTville pPremiereVille){
    //Création pointeur temporaire
    PTcentrale pcentrale;
    PTville pville;


    /*
     * Création des villes
     */

    //Création première ville
    pville = pPremiereVille;
    pville->codePostal = 1;
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));

    //Création deuxieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 2;
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));

    //Création troisieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 3;
    pville->villeSuivante = NULL;


    /*
     * Création des centrales
     */

    //Création première centrale
    pcentrale = pPremiereCentrale;
    pcentrale->codeCentrale = 1;
    pcentrale->puissance_max = 1000;
    pcentrale->puissance_restante = 1000;
    pcentrale->ptprecedent = NULL;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));

    //Création deuxième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pPremiereCentrale->ptsuivant;
    pcentrale->codeCentrale = 2;
    pcentrale->puissance_max = 1500;
    pcentrale->puissance_restante = 1500;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));

    //Création troisième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pcentrale->ptsuivant;
    pcentrale->codeCentrale = 3;
    pcentrale->puissance_max = 2000;
    pcentrale->puissance_restante = 2000;
    pcentrale->ptsuivant = NULL;
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));


    /*
     * Création des liens
     */


    //Lien centrale 1 <-> ville 1
    pcentrale = pPremiereCentrale;


    pville = pPremiereVille;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 1000;
    pcentrale->puissance_restante -= 1000;
    pcentrale->villeDependante->ligneSuivante = NULL;


    //Lien centrale 2 <-> ville 2 & 3
    pcentrale = pcentrale->ptsuivant;


    pville = pville->villeSuivante;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 900;
    pcentrale->puissance_restante -= 900;
    pcentrale->villeDependante->ligneSuivante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));


    pville = pville->villeSuivante;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 400;
    pcentrale->puissance_restante -= 400;
    pcentrale->villeDependante->ligneSuivante = NULL;


    //Lien centrale 3 <-> ville 3
    pcentrale = pcentrale->ptsuivant;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 1500;
    pcentrale->puissance_restante -= 1500;
    pcentrale->villeDependante->ligneSuivante = NULL;

}


int main() {
    //Création pointeurs vers les centrales et les villes
    PTcentrale pPremiereCentrale, pcentrale;
    PTville pPremiereVille, pville;

    //Création adresse première ville et centrale
    pPremiereCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pPremiereVille = (PTville) malloc(sizeof (Tville));

    creation_test(pPremiereCentrale, pPremiereVille);



    return 0;
}
