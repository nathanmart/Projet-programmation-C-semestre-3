//Nathan MARTORANA
//Pierre BOTHUA
//
//Projet GIF
//Automne 2022

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>


//Structure des villes
typedef struct ville {
    int codePostal;
    struct ville * villeSuivante;
} Tville;
//Pointeur associé à la structure
typedef Tville * PTville;


//Structure des lignes electriques
typedef struct lignesElectrique {
    int puissance;
    // pointeur sur une ville
    PTville villeDesservie;
    // liste simplement chainée
    struct lignesElectrique * ligneSuivante ;
} TlignesElectrique;
//Pointeur associé à la structure
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
//Pointeur associé à la structure
typedef Tcentrale * PTcentrale;


//Création pointeurs vers les centrales et les villes
//On utilise ici des variables globales
PTcentrale pPremiereCentrale;
PTville pPremiereVille;

/*
 * Cette fonction est utile seulement pour le développement.
 * Elle créé 3 centrales et 3 villes.
 *
 * Les connexions entre ces élements sont les suivants:
 * La centrale 1 et la ville 1 simplement reliées entre elles
 * La centrale 2 reliée aux ville 2 et 3
 * La centrale 3 relié à la ville 3
 */
void creation_test(){
    //Création pointeur temporaire
    PTcentrale pcentrale;
    PTville pville;
    PTligneElectrique pligne;


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
    pligne = pcentrale->villeDependante->ligneSuivante;
    pligne->villeDesservie = pville;
    pligne->puissance = 400;
    pcentrale->puissance_restante -= 400;
    pligne->ligneSuivante = NULL;


    //Lien centrale 3 <-> ville 3
    pcentrale = pcentrale->ptsuivant;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 1500;
    pcentrale->puissance_restante -= 1500;
    pcentrale->villeDependante->ligneSuivante = NULL;
}


/*
 * Cette fonction est utile seulement pour le développement
 * Elle permet d'afficher dands la console l'ensemble des villes et des centrales, ainsi que les connexions avec
 * les différentes valeurs
 */
void affichage_general(){
    //Création pointeur temporaire
    PTcentrale pcentrale;
    PTville pville;
    PTligneElectrique pligne;

    printf("************************************************************\n");


    /*
     * Affichage des villes
     */

    printf("Affichage des villes; \n\n");
    pville = pPremiereVille;
    while (pville){
        //Calcul puissance total
        int puissance_recu = 0;
        pcentrale = pPremiereCentrale;
        while (pcentrale){
            pligne = pcentrale->villeDependante;
            while (pligne){
                if (pligne->villeDesservie == pville) puissance_recu += pligne->puissance;
                pligne = pligne->ligneSuivante;
            }
            pcentrale = pcentrale->ptsuivant;
        }

        printf("Code postal: %d, puissance recu: %d\n", pville->codePostal, puissance_recu);
        pville = pville->villeSuivante;
    }
    printf("************************************************************\n");


    /*
     * Affichage des centrales
     */

    printf("Affichage des centrales:\n\n");
    pcentrale = pPremiereCentrale;
    while (pcentrale){
        printf("Centrale numero %d, de puissance %dMG, il reste %dMG disponible\n", pcentrale->codeCentrale, pcentrale->puissance_max, pcentrale->puissance_restante);
        printf("Les connexions sont les suivantes:\n");
        pligne = pcentrale->villeDependante;
        while (pligne){
            printf("Liaison avec la ville %d pour une puissance de %dMG\n", pligne->villeDesservie->codePostal, pligne->puissance);
            pligne = pligne->ligneSuivante;
        }

        printf("\n");
        pcentrale = pcentrale->ptsuivant;
    }

    printf("************************************************************\n");
}


int main() {
    //Création adresse première ville et centrale
    pPremiereCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pPremiereVille = (PTville) malloc(sizeof (Tville));

    creation_test();
    affichage_general();



    return 0;
}
