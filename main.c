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
 * Cette fonction renvoie la puissance restrance d'une centrale
 */
int get_puissance_restante_centrale(PTcentrale pcentral){
    int puissance_restante = pcentral->puissance_max;
    PTligneElectrique pligne = pcentral->villeDependante;

    while (pligne != NULL){
        puissance_restante -= pligne->puissance;
        pligne = pligne->ligneSuivante;
    }

    return puissance_restante;
}



/*
 * Cette fonction est utile seulement pour le développement.
 * Elle créé 3 centrales et 3 villes.
 *
 * Les connexions entre ces élements sont les suivants :
 * La centrale 1 et la ville 1 simplement reliées entre elles
 * La centrale 2 reliée aux villes 2 et 3
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
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));

    //Création quatrieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 4;
    pville->villeSuivante = NULL;


    /*
     * Création des centrales
     */

    //Création première centrale
    pcentrale = pPremiereCentrale;
    pcentrale->codeCentrale = 1;
    pcentrale->puissance_max = 1000;
    pcentrale->ptprecedent = NULL;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;

    //Création deuxième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pPremiereCentrale->ptsuivant;
    pcentrale->codeCentrale = 2;
    pcentrale->puissance_max = 1500;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;

    //Création troisième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pcentrale->ptsuivant;
    pcentrale->codeCentrale = 3;
    pcentrale->puissance_max = 2000;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;

    //Création quatrième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pcentrale->ptsuivant;
    pcentrale->codeCentrale = 4;
    pcentrale->puissance_max = 5000;
    pcentrale->ptsuivant = NULL;
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));

    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;


    /*
     * Création des liens
     */

    //Lien centrale 1 <-> ville 1
    pcentrale = pPremiereCentrale;

    pville = pPremiereVille;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 1000;
    pcentrale->villeDependante->ligneSuivante = NULL;


    //Lien centrale 2 <-> ville 2 & 3
    pcentrale = pcentrale->ptsuivant;

    pville = pville->villeSuivante;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 900;
    pcentrale->villeDependante->ligneSuivante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));


    pville = pville->villeSuivante;
    pligne = pcentrale->villeDependante->ligneSuivante;
    pligne->villeDesservie = pville;
    pligne->puissance = 400;
    pligne->ligneSuivante = NULL;


    //Lien centrale 3 <-> ville 3
    pcentrale = pcentrale->ptsuivant;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 1500;
    pcentrale->villeDependante->ligneSuivante = NULL;
}


/*
 * Cette fonction est utile seulement pour le développement
 * Elle permet d'afficher dans la console l'ensemble des villes et des centrales, ainsi que les connexions avec
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
        printf("Centrale numero %d, de puissance %dMG, il reste %dMG disponible\n", pcentrale->codeCentrale, pcentrale->puissance_max,
               get_puissance_restante_centrale(pcentrale));
        printf("Les connexions sont les suivantes:\n");
        pligne = pcentrale->villeDependante;

        while (pligne && pligne->villeDesservie){
            printf("Liaison avec la ville %d pour une puissance de %dMG\n", pligne->villeDesservie->codePostal, pligne->puissance);
            pligne = pligne->ligneSuivante;
        }


        printf("\n");
        pcentrale = pcentrale->ptsuivant;
    }

    printf("************************************************************\n");
}



/*
 * Cette fonction vérifie si la puissance que l'on veut prendre à la centrale est disponible
 * Renvoie 1 s'il reste assez de puissance
 * Renvoi 0 s'il n'y a plus assez de puissance
 */
int check_puissance_suffisante(PTcentrale pcentrale, int puissance){
    if (get_puissance_restante_centrale(pcentrale) >= puissance) return 1;
    else return 0;
}

/*
 * Vérifie si une connexion entre une centrale et une ville existe
 * Renvoie 1 si elle existe
 * Renvoie 0 si elle n'existe pas
 */
int check_existance_connexion(PTcentrale pcentrale, PTville pvile){
    PTligneElectrique pligne = pcentrale->villeDependante;
    while (pligne && pligne->villeDesservie){
        if (pligne->villeDesservie == pvile) return 1;
        pligne = pligne->ligneSuivante;
    }
    return 0;
}


/*
 * Cette fonction additionne une nouvelle puissance avec celle existante d'une connexion
 * Renvoie 1 si tout s'est bien passé
 * Renvoie 0 en cas d'erreur
 */
int additionner_connexion(PTcentrale pcentrale, PTville pvile, int puissance){
    PTligneElectrique pligne = pcentrale->villeDependante;
    while (pligne){
        if (pligne->villeDesservie == pvile){
            pligne->puissance += puissance;
            return 1;
        }
        pligne = pligne->ligneSuivante;
    }
    return 0;
}


/*
 * Cette fonction change la puissance d'une connexion existante
 * Renvoie 1 si tout s'est bien passé
 * Renvoie 0 en cas d'erreur
 */
int modifier_connexion(PTcentrale pcentrale, PTville pvile, int puissance){
    PTligneElectrique pligne = pcentrale->villeDependante;
    while (pligne){
        if (pligne->villeDesservie == pvile){
            pligne->puissance = puissance;
            return 1;
        }
        pligne = pligne->ligneSuivante;
    }
    return 0;
}


/*
 * Cette fonction permet d'ajouter une connexion entre une centrale et une ville
 * Signification valeur de retour :
 *      0 --> Pas assez de puissance disponible dans la centrale
 *      1 --> Nouvelle connexion créé
 *      2 --> Modification d'une connexion existante
 */
int ajouter_connexion(PTcentrale pcentrale, PTville pville, int puissance){
    //Vérification de la puissance restante de la centrale
    if (!check_puissance_suffisante(pcentrale, puissance)) return 0;

    //Si la connexion existe déja, on additionne les deux puissance
    if (check_existance_connexion(pcentrale, pville)){
        additionner_connexion(pcentrale, pville, puissance);
        return 2;
    }

    //Création d'une nouvelle connexion

    //On vient se placer à la dernière connexion
    //La nouvelle connexion est donc ajoutée à la fin de la liste
    PTligneElectrique pligne = pcentrale->villeDependante;
    while(pligne->ligneSuivante != NULL) pligne = pligne->ligneSuivante;

    //Dans le cas où il y a déja une connexion sur cette centrale, on se place sur la connexion suivante
    if (pcentrale->villeDependante->puissance != 0){
        pligne->ligneSuivante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
        pligne = pligne->ligneSuivante;
    }

    //Paramétrage de la nouvelle connexion
    pligne->ligneSuivante = NULL;
    pligne->villeDesservie = pville;
    pligne->puissance = puissance;

    return 1;
}


int main() {
    //Création adresse première ville et centrale
    pPremiereCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pPremiereVille = (PTville) malloc(sizeof (Tville));

    creation_test();
    affichage_general();

    modifier_connexion(pPremiereCentrale->ptsuivant->ptsuivant, pPremiereVille->villeSuivante->villeSuivante, 250);

    affichage_general();



    return 0;
}

//30/10/2022 18:24