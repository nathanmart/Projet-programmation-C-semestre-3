//Nathan MARTORANA
//Pierre BOTHUA
//
//Projet GIF
//Automne 2022

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <dirent.h>

// Indice ASCII des caractères d'affichage
// A changer si spécificité sur la machine d'execution
#define cvertical 186
#define chorizontal 205
#define cangleGH 201
#define cangleGB 200
#define cangleDH 187
#define cangleDB 188
#define cDoubleAngleGauche 204
#define cDoubleAngleDroite 185

// Indice des caractères clavier
#define fleche_haut 472
#define fleche_bas 480
#define fleche_droite 477
#define fleche_gauche 475
#define entree 13
#define echap 27


//Structure des villes
typedef struct ville {
    int codePostal;
    char nom[30];
    struct ville * villeSuivante;
    struct ville * villePrecedente;
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
    char nom[30];
    int puissance_max;
    // Pointeur sur la liste des lignes
    PTligneElectrique villeDependante;
    // Liste doublement chainée
    struct centrale * ptsuivant;
    struct centrale * ptprecedent;
}Tcentrale;
//Pointeur associé à la structure
typedef Tcentrale * PTcentrale;


//Structure pour l'affichage des fichiers
typedef struct fichier{
    char nom[100];
    struct fichier * ptsuivant;
} Tfichier;
typedef Tfichier * PTfichier;

//On utilise ici des variables globales

//Création pointeurs vers les centrales et les villes
PTcentrale pPremiereCentrale;
PTville pPremiereVille;
FILE* sauvegarde = NULL;

//Compteur nombre ville && centrale
int nbville = 0;
int nbcentrales = 0;
int nbconnexion = 0;



/////////////// Fonctions get ///////////////

/*
 * Cette fonction renvoie l'adresse d'une centrale
 */
PTcentrale get_adresse_centrale(int code_central){
    PTcentrale pcentrale = pPremiereCentrale;
    while (pcentrale){
        if (pcentrale->codeCentrale == code_central) return pcentrale;
        pcentrale = pcentrale->ptsuivant;
    }
    return 0;
}


/*
 * Cette fonction renvoie l'adresse d'une ville
 */
PTville get_adresse_ville(int code_postal){
    PTville pville = pPremiereVille;
    while (pville){
        if (pville->codePostal == code_postal) return pville;
        pville = pville->villeSuivante;
    }
    return 0;
}


/*
 * Cette fonction renvoie la puissance restante d'une centrale
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
 * Cette fonction renvoie la puissance totale reçue d'une ville
 */
int get_puissance_recu(PTville pville){
    PTcentrale pcentrale = pPremiereCentrale;
    PTligneElectrique pligne;
    int compteur = 0;
    if (nbcentrales){
        while (pcentrale){
            pligne = pcentrale->villeDependante;
            while (pligne && pligne->villeDesservie){
                if (pligne->villeDesservie == pville) compteur += pligne->puissance;
                pligne = pligne->ligneSuivante;
            }
            pcentrale = pcentrale->ptsuivant;
        }
    }
    return compteur;
}



/////////////// Fonctions de développement ///////////////

/*
 * Cette fonction est utile seulement pour le développement.
 * Elle créée 3 centrales et 3 villes.
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
    strcpy(pville->nom, "Ville_1");
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));
    nbville++;

    //Création deuxieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 2;
    strcpy(pville->nom, "Ville_2");
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));
    nbville++;

    //Création troisieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 3;
    strcpy(pville->nom, "Ville_3");
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));
    nbville++;

    //Création quatrieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 4;
    strcpy(pville->nom, "Ville_4");
    pville->villeSuivante = NULL;
    nbville++;


    /*
     * Création des centrales
     */

    //Création première centrale
    pcentrale = pPremiereCentrale;
    pcentrale->codeCentrale = 1;
    strcpy(pcentrale->nom, "Centrale_1");
    pcentrale->puissance_max = 1000;
    pcentrale->ptprecedent = NULL;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;
    nbcentrales++;

    //Création deuxième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pPremiereCentrale->ptsuivant;
    pcentrale->codeCentrale = 2;
    strcpy(pcentrale->nom, "Centrale_2");
    pcentrale->puissance_max = 1500;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;
    nbcentrales++;

    //Création troisième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pcentrale->ptsuivant;
    pcentrale->codeCentrale = 3;
    strcpy(pcentrale->nom, "Centrale_3");
    pcentrale->puissance_max = 2000;
    pcentrale->ptsuivant = (PTcentrale) malloc(sizeof(Tcentrale));
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;
    nbcentrales++;

    //Création quatrième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pcentrale->ptsuivant;
    pcentrale->codeCentrale = 4;
    strcpy(pcentrale->nom, "Centrale_4");
    pcentrale->puissance_max = 5000;
    pcentrale->ptsuivant = NULL;
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->puissance = 0;
    nbcentrales++;


    /*
     * Création des liens
     */

    //Lien centrale 1 <-> ville 1
    pcentrale = pPremiereCentrale;

    pville = pPremiereVille;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 1000;
    pcentrale->villeDependante->ligneSuivante = NULL;
    nbconnexion++;


    //Lien centrale 2 <-> ville 2 & 3
    pcentrale = pcentrale->ptsuivant;

    pville = pville->villeSuivante;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 900;
    pcentrale->villeDependante->ligneSuivante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    nbconnexion++;



    pville = pville->villeSuivante;
    pligne = pcentrale->villeDependante->ligneSuivante;
    pligne->villeDesservie = pville;
    pligne->puissance = 400;
    pligne->ligneSuivante = NULL;
    nbconnexion++;



    //Lien centrale 3 <-> ville 3
    pcentrale = pcentrale->ptsuivant;
    pcentrale->villeDependante->villeDesservie = pville;
    pcentrale->villeDependante->puissance = 1500;
    pcentrale->villeDependante->ligneSuivante = NULL;
    nbconnexion++;

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
    printf("IL y a %d villes, %d centrales et %d connexions", nbville, nbcentrales, nbconnexion);


    /*
     * Affichage des villes
     */

    printf("Affichage des villes; \n\n");
    pville = pPremiereVille;
    while (pville && nbville){
        //Calcul puissance total
        int puissance_recu = 0;
        pcentrale = pPremiereCentrale;
        while (pcentrale && nbcentrales){
            pligne = pcentrale->villeDependante;
            while (pligne){
                if (pligne->villeDesservie == pville) puissance_recu += pligne->puissance;
                pligne = pligne->ligneSuivante;
            }
            pcentrale = pcentrale->ptsuivant;
        }

        printf("Nom: %s, Code postal: %d, puissance recu: %d\n", pville->nom, pville->codePostal, puissance_recu);
        pville = pville->villeSuivante;
    }
    printf("************************************************************\n");


    /*
     * Affichage des centrales
     */

    printf("Affichage des centrales:\n\n");
    pcentrale = pPremiereCentrale;
    while (pcentrale && nbcentrales){
        printf("Nom: %s, Centrale numero %d, de puissance %dMG, il reste %dMG disponible\n", pcentrale->nom, pcentrale->codeCentrale, pcentrale->puissance_max,
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


/////////////// Fonctions check ///////////////

/*
 * Cette fonction vérifie si le code de la centrale est déjà utilisée
 * Renvoie 0 si il n'est pas utilisé
 * Renvoie 1 si il est utilisé
 */
int check_code_centrale_utilise(int code_centrale){
    PTcentrale pcentrale = pPremiereCentrale;

    while(pcentrale && nbcentrales){
        if(pcentrale->codeCentrale == code_centrale) return 1;
        pcentrale = pcentrale->ptsuivant;
    }
    return 0;
}


/*
 * Cette fonction vérifie si la centrale existe
 * Renvoie 0 si elle n'existe pas
 * Renvoie 1 si elle existe
 */
int check_existance_centrale(int code_centrale){
    PTcentrale pcentrale = pPremiereCentrale;

    while(pcentrale && nbcentrales){
        if(pcentrale->codeCentrale == code_centrale) return 1;
        pcentrale = pcentrale->ptsuivant;
    }
    return 0;
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
int check_existance_connexion(PTcentrale pcentrale, PTville pville){
    PTligneElectrique pligne = pcentrale->villeDependante;
    while (pligne && pligne->villeDesservie){
        if (pligne->villeDesservie == pville) return 1;
        pligne = pligne->ligneSuivante;
    }
    return 0;
}


/*
 * Cette fonction vérifie si un code postal est déja utilisé pour une ville
 * Renvoie 0 si il n'est pas utilisé
 * Renvoi 1 si il est déja utilisé
 */
int check_code_postal_utilise(int code_postal){
    PTville pville = pPremiereVille;
    while (pville && nbville){
        if (pville->codePostal == code_postal) return 1;
        pville = pville->villeSuivante;
    }
    return 0;
}


/////////////// Fonctions centrales ///////////////

/*
 * Cette fonction permet de modifier une centrale
 * Renvoie 0 si la centrale n'existe pas
 * Renvoie 1 si tout est bon
 * Renvoie 2 si la puissance est insufisante
 */
int modifier_centrale(int code_centrale, int puissance_max){
    if(! check_existance_centrale(code_centrale)) return 0;
    PTcentrale pcentrale = get_adresse_centrale(code_centrale);
    if(puissance_max < pcentrale->puissance_max - get_puissance_restante_centrale(get_adresse_centrale(code_centrale))) return 2;

    pcentrale = pPremiereCentrale;

    while(pcentrale){
        if(pcentrale->codeCentrale == code_centrale){
            pcentrale->puissance_max = puissance_max;
            return 1;
        }
        pcentrale = pcentrale->ptsuivant;
    }

    return 0;
}


/*
 * Cette fonction permet d'ajouter une centrale
 * Renvoie 0 si le code de la centrale est déja utilisé
 * Renvoie 1 si tout est bon
 */
int ajout_centrale(int code_centrale, int puissance_max, char nom_centrale[]){
    if(check_code_centrale_utilise(code_centrale)) return 0;

    PTcentrale pcentrale = pPremiereCentrale;

    if(nbcentrales){
        while(pcentrale->ptsuivant) pcentrale = pcentrale->ptsuivant;

        pcentrale->ptsuivant = (PTcentrale)malloc(sizeof (Tcentrale));
        pcentrale->ptsuivant->ptprecedent = pcentrale;
        pcentrale = pcentrale->ptsuivant;
    }
    else{
        pcentrale->ptprecedent = NULL;
    }

    strcpy(pcentrale->nom, nom_centrale);
    pcentrale->ptsuivant = NULL;
    pcentrale->codeCentrale = code_centrale;
    pcentrale->puissance_max = puissance_max;
    pcentrale->villeDependante = (PTligneElectrique) malloc(sizeof (TlignesElectrique));
    pcentrale->villeDependante->puissance = 0;
    pcentrale->villeDependante->ligneSuivante = NULL;
    pcentrale->villeDependante->villeDesservie = NULL;
    nbcentrales ++;

    return 1;
}


/*
 * Cette fonction permet de supprimer une centrale
 * Renvoie 0 si la centrale n'existe pas
 * Renvoie 1 si tout est bon
 * Renvoie 2 en cas d'erreur
 */
int supprimer_centrale(int code_centrale){
    if(! check_existance_centrale(code_centrale)) return 0;

    PTcentrale pcentrale = pPremiereCentrale;

    // Dans le cas où il n'y a qu'une seule centrale
    if(! pcentrale->ptsuivant){
        pcentrale->ptprecedent = NULL;
        pcentrale->ptsuivant = NULL;
        pcentrale->codeCentrale = 0;
        pcentrale->puissance_max = 0;
        pcentrale->villeDependante = NULL;
        nbcentrales --;
        return 1;
    }

    // Dans le cas où la centrale supprimée est la première
    else if(pcentrale->codeCentrale == code_centrale){
        pcentrale->ptsuivant->ptprecedent = NULL;
        pPremiereCentrale = pcentrale->ptsuivant;
        nbcentrales --;
        return 1;
    }
    // Dans le cas où la centrale supprimée est la dernière
    while(pcentrale->ptsuivant) pcentrale = pcentrale->ptsuivant;
    if(pcentrale->codeCentrale == code_centrale){
        pcentrale->ptprecedent->ptsuivant = NULL;
        nbcentrales --;
        return 1;
    }

    else {
        pcentrale = pPremiereCentrale;
        while (pcentrale->ptsuivant) {
            if (pcentrale->codeCentrale == code_centrale) {
                pcentrale->ptsuivant->ptprecedent = pcentrale;
                pcentrale->ptprecedent->ptsuivant = pcentrale->ptsuivant;
                nbcentrales--;
                return 1;
            }
            pcentrale = pcentrale->ptsuivant;
        }
    }
    return 2;
}


/////////////// Fonctions connexion ///////////////

/*
 * Cette fonction additionne une nouvelle puissance avec celle existante d'une connexion
 * Renvoie 1 si tout s'est bien passé
 * Renvoie 0 en si la connexion n'existe pas
 */
int additionner_connexion(PTcentrale pcentrale, PTville pville, int puissance){
    PTligneElectrique pligne = pcentrale->villeDependante;
    while (pligne){
        if (pligne->villeDesservie == pville){
            pligne->puissance += puissance;
            return 1;
        }
        pligne = pligne->ligneSuivante;
    }
    return 0;
}


/*
 * Cette fonction change la puissance d'une connexion existante
 * Renvoie 0 si la connexion n'existe pas
 * Renvoie 1 si la connexion a ete modifié
 * Renvoie 2 si la puissance de la centrale est insuffisante
 */
int modifier_connexion(PTcentrale pcentrale, PTville pville, int puissance){
    PTligneElectrique pligne = pcentrale->villeDependante;
    if (puissance > pligne->puissance){
        if (!check_puissance_suffisante(pcentrale, puissance - pligne->puissance)) return 2;
    }
    while (pligne){
        if (pligne->villeDesservie == pville){
            pligne->puissance = puissance;
            return 1;
        }
        pligne = pligne->ligneSuivante;
    }
    return 0;
}


/*
 * Cette fonction permet d'ajouter une connexion entre une centrale et une ville
 * Signification valeur de retour:
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

    //Si la centrale à aucune connexion
    PTligneElectrique pligne = pcentrale->villeDependante;
    if(! pligne->villeDesservie){
        pligne->villeDesservie = pville;
        pligne->puissance = puissance;
        nbconnexion ++;
        return 1;
    }

    //On vient se placer à la dernière connexion
    //La nouvelle connexion est donc ajoutée à la fin de la liste
    pligne = pcentrale->villeDependante;
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

    nbconnexion ++;
    return 1;
}


/*
 * Cette fonction permet de supprimer une connexion ligne/centrale
 * Renvoi 0 si la connexion n'existe pas
 * Renvoi 1 si la connexion existe
 */
int supprimer_connexion(PTcentrale pcentrale, PTville pville){
    //Vérification existance connexion
    if (!check_existance_connexion(pcentrale, pville)) return 0;

    PTligneElectrique pligne = pcentrale->villeDependante;

    //Cas où il n'y a qu'une seule connexion sur cette centrale
    if (! pligne->ligneSuivante){
        pligne->villeDesservie = NULL;
        pligne->ligneSuivante = NULL;
        pligne->puissance = 0;
        nbconnexion --;
        return 1;
    }
        //Cas où la connexion à supprimer est la première
    else if (pligne->villeDesservie == pville){
        pcentrale->villeDependante = pligne->ligneSuivante;
        nbconnexion --;
        return 1;
    }
        //Autres cas
    else {
        //Se place sur la ligne précédent la suppression
        while (pligne->ligneSuivante->villeDesservie != pville) pligne = pligne->ligneSuivante;
        pligne->ligneSuivante = pligne->ligneSuivante->ligneSuivante;
        nbconnexion --;
        return 1;
    }
}



/////////////// Fonctions ville ///////////////

/*
 * Cette fonction permet d'ajouter une ville
 * Renvoi 0 si le code postal est déja utilisé
 * Renvoi 1 si la ville a bien été créée
 */
int ajouter_ville(int code_postal, char nom_ville[]){
    //Vérification code postal
    if (check_code_postal_utilise(code_postal)) return 0;

    PTville pville = pPremiereVille;

    //Si il y a déja des villes
    if(nbville) {
        //Déplacement vers la dernière ville enrengistrée
        while (pville->villeSuivante) pville = pville->villeSuivante;

        pville->villeSuivante = (PTville) malloc(sizeof (Tville));
        pville->villeSuivante->villePrecedente = pville;
        pville = pville->villeSuivante;
    } else{
        pville->villePrecedente = NULL;
    }

    pville->codePostal = code_postal;
    strcpy(pville->nom, nom_ville);
    pville->villeSuivante = NULL;
    nbville++;

    return 1;
}


/*
 * Cette fonction permet de supprimer une ville
 * Renvoi 0 si la ville n'existe pas
 * Renvoi 1 si la ville a bien été supprimée
 */
int supprimer_ville(int code_postal){
    //Vérification de l'existence de la ville
    if(! check_code_postal_utilise(code_postal)) return 0;


    //On supprime toutes les connexions que cette ville pourrait avoir
    PTcentrale pcentrale = pPremiereCentrale;
    while (pcentrale && nbcentrales){
        supprimer_connexion(pcentrale, get_adresse_ville(code_postal));
        pcentrale = pcentrale->ptsuivant;
    }

    PTville pville = pPremiereVille;

    //Cas où il n'y a qu'une seule ville
    if (! pville->villeSuivante){
        //On créer une nouvelle adresse pour pPremiereVille, et on l'utilise comme bidon
        pPremiereVille = (PTville) malloc(sizeof (Tville));
        pPremiereVille->codePostal = -1;
        pPremiereVille->villeSuivante = NULL;
        nbville--;
        return 1;
    }
        //Cas où la ville à supprimer est la première
    else if(pville->codePostal == code_postal){
        //On change l'adresse du pointeur pPremiereVille
        pPremiereVille = pville->villeSuivante;
        pville->villeSuivante->villePrecedente = NULL;
        nbville--;
        return 1;
    }
        //Autres cas
    else {
        //Se place sur la ville précédent la suppression
        while (pville->villeSuivante->codePostal != code_postal) pville = pville->villeSuivante;
        pville->villeSuivante = pville->villeSuivante->villeSuivante;
        if (pville->villeSuivante){
            pville->villeSuivante->villePrecedente = pville;
        }
        nbville--;
        return 1;
    }
}



/////////////// Fonctions sauvegarde ///////////////

/*
 * Cette fonction créée une sauvegarde des données
 * Renvoie 0 si erreur
 * Renvoie 1 si tout est bon
 */
int sauvegarde_fichier(char chemin[200]){
    PTcentrale pcentrale = pPremiereCentrale;
    PTville pville = pPremiereVille;
    PTligneElectrique pligne = NULL;

    sauvegarde = fopen(chemin, "w");

    if(! sauvegarde){
        fclose(sauvegarde);
        return 0;
    }

    fprintf(sauvegarde, "%d\n", nbville);

    if(nbville) {
        while (pville) {
            fprintf(sauvegarde, "%d %s\n", pville->codePostal, pville->nom);
            pville = pville->villeSuivante;
        }
    }

    fprintf(sauvegarde, "%d\n", nbcentrales);

    if(! nbcentrales) {
        fclose(sauvegarde);
        return 1;
    }

    while(pcentrale){
        fprintf(sauvegarde, "%d %d %s\n", pcentrale->codeCentrale, pcentrale->puissance_max, pcentrale->nom);
        pcentrale = pcentrale->ptsuivant;
    }

    fprintf(sauvegarde, "%d\n", nbconnexion);
    pcentrale = pPremiereCentrale;
    while(pcentrale) {
        pligne = pcentrale->villeDependante;
        if (!pligne->villeDesservie){
            fclose(sauvegarde);
            return 1;
        }
        while (pligne) {
            pville = pligne->villeDesservie;
            fprintf(sauvegarde, "%d %d %d\n", pcentrale->codeCentrale, pville->codePostal, pligne->puissance);
            pligne = pligne->ligneSuivante;
        }
        pcentrale = pcentrale->ptsuivant;
    }

    fclose(sauvegarde);
    return 1;
}


/*
 * Cette fonction lit le fichier sauvegarde que l'on veut
 * Renvoie 0 si le fichier n'existe pas
 * Renvoie 1 si tout est bon
 */
int chargement_sauvegarde(char chemin[200]) {
    pPremiereCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pPremiereVille = (PTville) malloc(sizeof (Tville));

    sauvegarde = NULL;
    int compteur_ville = 0, compteur_centrale = 0, compteur_connexion = 0, code_postale, code_centrale, puissance;
    char nom_ville[30] = {0}, nom_centrale[30] = {0};

    sauvegarde = fopen(chemin, "r");
    if(!sauvegarde) return 0;
    nbville = 0;
    nbcentrales = 0;
    nbconnexion = 0;

    nbcentrales = 0, nbville = 0, nbconnexion = 0;

    fscanf(sauvegarde, "%d", &compteur_ville);
    while (compteur_ville) {
        fscanf(sauvegarde, "%d %s", &code_postale, nom_ville);
        ajouter_ville(code_postale, nom_ville);
        compteur_ville --;
    }

    fscanf(sauvegarde, "%d", &compteur_centrale);
    while(compteur_centrale){
        fscanf(sauvegarde, "%d %d %s", &code_centrale, &puissance, nom_ville);
        ajout_centrale(code_centrale, puissance, nom_ville);
        compteur_centrale --;
    }

    fscanf(sauvegarde, "%d", &compteur_connexion);
    while(compteur_connexion){
        fscanf(sauvegarde, "%d %d %d", &code_centrale, &code_postale, &puissance);
        ajouter_connexion(get_adresse_centrale(code_centrale), get_adresse_ville(code_postale), puissance);
        compteur_connexion --;
    }

    fclose(sauvegarde);
    return 1;
}



/////////////// Fonctions graphique test ///////////////

/*
 * Cette fonction est une IHM rudimentaire
 * Elle est utilisées pour les tests finaux
 */
void programme_console(){
    while(1){
        printf("Que voulez-vous faire?:\n");
        printf("Ajouter une ville: 1\n");
        printf("Supprimmer une ville: 2\n");
        printf("Ajouter une centrale: 3\n");
        printf("Modifier la puissance d'une centrale: 4\n");
        printf("Supprimer une centrale: 5\n");
        printf("Ajouter une connexion: 6\n");
        printf("Modifier une connexion: 7\n");
        printf("Supprimer une connexion: 8\n");
        printf("Enregistrer dans un fichier: 9\n");
        printf("Charger un fichier: 10\n");
        printf("Affichage general: 11\n");
        printf("Votre choix: ");

        int choix;
        scanf("%d", &choix);

        if(choix == 1){
            printf("Nom de la nouvelle ville: ");
            char nom[50];
            scanf("%s", &nom);
            printf("Code postal: ");
            int code_postal;
            scanf("%d", &code_postal);
            int retour = ajouter_ville(code_postal, nom);
            if (retour == 0) printf("Code postal deka utilise");
            else printf("Ville ajoutee");
        }
        else if(choix == 2){
            printf("Supprimer ville, code postal: ");
            int code_postal;
            scanf("%d", &code_postal);
            int retour = supprimer_ville(code_postal);
            if (retour == 0) printf("La ville n'existe pas");
            else printf("Ville supprimee");
        }
        else if(choix == 3){
            printf("Ajouter centrale, son numero d'identification");
            int numero;
            scanf("%d", &numero);
            printf("Son nom: ");
            char nom[50];
            scanf("%s", &nom);
            printf("Sa puissance max: ");
            int puissance;
            scanf("%d", &puissance);
            int retour = ajout_centrale(numero, puissance, nom);
            if (retour == 0) printf("Ce code est déja utilisé");
            else if (retour == 1) printf("Centrale bien ajoutee");
            else printf("Cette centrale existe deja");
        }
        else if(choix == 4){
            printf("Modifier puissance centrale, son numero d'identification: ");
            int numero;
            scanf("%d", &numero);
            printf("Sa nouvelle puissance: ");
            int puissance;
            scanf("%d", &puissance);
            int retour = modifier_centrale(numero, puissance);
            if (retour == 0) printf("La centrale n'existe pas");
            else if (retour == 1) printf("Puissance mis a jour");
            else if (retour == 2) printf("Puissance insufisante, supprimez d'abord des connexions");
        }
        else if(choix == 5){
            printf("Numero de la centrale a supprimer: ");
            int numero;
            scanf("%d", &numero);
            int retour = supprimer_centrale(numero);
            if (retour == 0) printf("La centrale n'existe pas");
            else printf("Centrale supprimee");
        }
        else if(choix == 6){
            printf("Ajouter connexion entre centrale numero: ");
            int numero;
            scanf("%d", &numero);
            printf("et la ville: ");
            int code_postal;
            scanf("%d", &code_postal);
            printf("pour une puissance de: ");
            int puissance;
            scanf("%d", &puissance);
            int retour = ajouter_connexion(get_adresse_centrale(numero), get_adresse_ville(code_postal), puissance);
            if (retour == 0) printf("Pas assez de puissance dispo");
            else if (retour == 1) printf("Connexion ajoutee");
            else if (retour == 2) printf("Connexion existante, elles ont ete additionnees");
        }
        else if (choix == 7){
            printf("Modifier connexion entre central numero: ");
            int numero;
            scanf("%d", &numero);
            printf("et la ville: ");
            int code_postal;
            scanf("%d", &code_postal);
            printf("pour une nouvelle puissance de: ");
            int puissance;
            scanf("%d", &puissance);
            int retour = modifier_connexion(get_adresse_centrale(numero), get_adresse_ville(code_postal), puissance);
            if (retour == 0) printf("La connexion n'existe pas");
            else printf("Connexion modifiee");
        }
        else if (choix == 8){
            printf("Supprimer une connexion entre la centrale numero:");
            int numero;
            scanf("%d", &numero);
            printf("et la ville: ");
            int code_postal;
            scanf("%d", &code_postal);
            int retour = supprimer_connexion(get_adresse_centrale(numero), get_adresse_ville(code_postal));
            if (retour == 0) printf("La connexion n'existe pas");
            else printf("La connexion a ete supprimee");
        }
        else if (choix == 9){
            printf("Entrez le nom avec le nom du fichier: ");
            char chemin[200];
            scanf("%s", &chemin);
            int retour = sauvegarde_fichier(chemin);
            if (retour == 1) printf("Sauverage avec succes");
            else printf("Erreur de sauvegarde");
        }
        else if (choix == 10){
            printf("Entrez le nom du fichier a charger: ");
            char chemin[200];
            scanf("%s", chemin);
            int retour = chargement_sauvegarde(chemin);
            if (retour == 0) printf("Le fichier n'existe pas");
            else printf("Fichier charge");
        }
        else if (choix == 11) affichage_general();


        printf("\n\n\n\n");

    }
}

/////////////// Graphique final ///////////////
//EN COUR DE DVLP

/*
 * Cette fonction renvoie le code du caractère clavier tapé
 */
int lireCaract(){

    char c=10;
    int fnt=0;

    c=getch();
    fnt=c;
    if (c==-32){c=getch();fnt = 400+c;}
    if (c==0)  {c=getch();fnt = 500+c;}
    return fnt;
}


/*
 * Cette fonction place le curseur à l'endroit souhaité dans la console
 */
void gotoLigCol( int lig, int col )
{
    // ressources
    COORD mycoord;

    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}


// Emplacement coin haut gauche des cadres
int x = 8;
int y = 2;

/*
 * Cette fonction dessine un rectangle
 */
void make_rectangle(HANDLE hConsole, int largeur, int hauteur){
    //Création de barres verticales
    for (int i = y; i < hauteur + y; i++){
        gotoLigCol(i, x);
        printf("%c", cvertical);
        gotoLigCol(i, x + largeur );
        printf("%c", cvertical);
    }

    //Création des barres horizontales
    for (int i = x - 1; i < largeur + x; i++){
        gotoLigCol(y, i + 1);
        printf("%c", chorizontal);
        gotoLigCol(y + hauteur, i +1);
        printf("%c", chorizontal);
    }

    //Coin
    gotoLigCol(y, x);
    printf("%c", cangleGH);

    gotoLigCol(y, x + largeur);
    printf("%c", cangleDH);

    gotoLigCol(y + hauteur, x);
    printf("%c", cangleGB);

    gotoLigCol(y + hauteur, x + largeur);
    printf("%c", cangleDB);
}

/*
 * Partie graphique qui créée une ligne
 */
void make_ligne_horizontal(HANDLE hConsole, int largeur, int ligne, int colonne){
    gotoLigCol(ligne, colonne);
    for (int i = 0; i < largeur; ++i) {
        printf("%c", chorizontal);
    }
}

/*
 * Partie graphique qui créée un dessin de centrale
 */
void make_dessin_centrale(int posX, int posY){
    gotoLigCol(posX, posY);
    printf("   ___");
    gotoLigCol(posX + 1, posY);
    printf("   | |");
    gotoLigCol(posX + 2, posY);
    printf("  /   \\");
    gotoLigCol(posX + 3, posY);
    printf(" /     \\");
    gotoLigCol(posX + 4, posY);
    printf(" -------");
}


/*
 * cote = 0 --> bouton +
 * cote = 1 --> bouton Edit
 * cote = 2 --> bouton Sup
 */
void make_bouton(HANDLE hConsole, int cote, int couleur){
    if (couleur) SetConsoleTextAttribute(hConsole, 113);
    else SetConsoleTextAttribute(hConsole, 15*16);

    for (int i = 0; i < 5; ++i) {
        if (cote == 0 || cote == 2) {
            //Carré de gauche
            gotoLigCol(y + 2, x + 3 + i);
            printf("%c", chorizontal);
            gotoLigCol(y + 4, x + 3 + i);
            printf("%c", chorizontal);
            gotoLigCol(y + 2 + 1, x + 3);
            printf("%c", cvertical);
            gotoLigCol(y + 2 + 1, x + 3 + 4);
            printf("%c", cvertical);
        }
        else if (cote == 1){
            //Carré de droite
            gotoLigCol(y + 2, x + 3 + i + 25);
            printf("%c", chorizontal);
            gotoLigCol(y + 4, x + 3 + i + 25);
            printf("%c", chorizontal);
            gotoLigCol(y + 2 + 1, x + 3 + 25);
            printf("%c", cvertical);
            gotoLigCol(y + 2 + 1, x + 3 + 4 + 25);
            printf("%c", cvertical);
        }
    }
    if (cote == 0) {
        gotoLigCol(y + 3, x + 4);
        printf(" + ");
        gotoLigCol(y + 2, x + 3);
        printf("%c", cangleGH);
        gotoLigCol(y + 2, x + 7);
        printf("%c", cangleDH);
        gotoLigCol(y + 4, x + 3);
        printf("%c", cangleGB);
        gotoLigCol(y + 4, x + 7);
        printf("%c", cangleDB);
    } else if (cote == 1){
        gotoLigCol(y + 3, x + 29);
        printf(" E ");
        gotoLigCol(y + 2, x + 3 + 25);
        printf("%c", cangleGH);
        gotoLigCol(y + 2, x + 7 + 25);
        printf("%c", cangleDH);
        gotoLigCol(y + 4, x + 3 + 25);
        printf("%c", cangleGB);
        gotoLigCol(y + 4, x + 7 + 25);
        printf("%c", cangleDB);

    } else if (cote == 2){
        gotoLigCol(y + 3, x + 4);
        printf(" X ");
        gotoLigCol(y + 2, x + 3);
        printf("%c", cangleGH);
        gotoLigCol(y + 2, x + 7);
        printf("%c", cangleDH);
        gotoLigCol(y + 4, x + 3);
        printf("%c", cangleGB);
        gotoLigCol(y + 4, x + 7);
        printf("%c", cangleDB);
    }
}


void affichage_resume(HANDLE hConsole){
    //Initialisation
    system("cls");
    system("color f0");
    SetConsoleTextAttribute(hConsole, 15*16);
    gotoLigCol(y + 1, x + 2);
    printf("Affichage de toutes les infos:");

    make_ligne_horizontal(hConsole, 35, y + 2, x);

    gotoLigCol(y + 3, x + 2);
    printf("Il y a: %d centrales", nbcentrales);
    gotoLigCol(y + 4, x + 10);
    printf("%d villes", nbville);
    gotoLigCol(y + 5, x + 10);
    printf("%d lignes electrique", nbconnexion);

    make_ligne_horizontal(hConsole, 35, y + 6, x);

    //Affichage des centrales & connexion
    gotoLigCol(y + 7, x + 8);
    printf("CENTRALES");
    PTcentrale pcentrale = pPremiereCentrale;
    PTligneElectrique pligne;
    int i = 8; //Compteur position
    while (pcentrale && nbcentrales){
        //Affichage centrale
        gotoLigCol(y + i, x + 2);
        printf("Nom: %s, Code: %d", pcentrale->nom, pcentrale->codeCentrale);
        i++;

        //Affichage connexion centrale
        pligne = pcentrale->villeDependante;
        while (pligne && pligne->villeDesservie){
            gotoLigCol(y + i, x + 4);
            printf("<->%s(%d) -> %d MW", pligne->villeDesservie->nom, pligne->villeDesservie->codePostal, pligne->puissance);
            pligne = pligne->ligneSuivante;
            i++;
        }
        i++;
        pcentrale = pcentrale->ptsuivant;
    }
    i--;
    make_ligne_horizontal(hConsole, 35, y + i, x);
    i++;

    //Affichage des villes
    gotoLigCol(y + i, x + 8);
    printf("VILLES");
    i++;
    PTville pville = pPremiereVille;
    while (pville && nbville){
        gotoLigCol(y + i, x + 2);
        printf("Nom: %s, Code: %d", pville->nom, pville->codePostal);
        i++;
        gotoLigCol(y + i, x + 4);
        printf("Recoit %d MW", get_puissance_recu(pville));
        i++;
        pville = pville->villeSuivante;
        i++;
    }

    make_rectangle(hConsole, 35, i);

    while (1){
        i = lireCaract();
        if (i == echap) break;
    }
}


/*
 * Partie de l'affichage de sauvegarde d'un fichier
 */
void graph_enrengistrer_fichier(HANDLE hConsole){
    //Initialisation
    system("cls");
    system("color f0");
    SetConsoleTextAttribute(hConsole, 15*16);
    gotoLigCol(y + 1, x + 2);
    printf("Enregistrement d'un fichier");
    gotoLigCol(y + 2, x + 2);
    printf("Nom:");
    make_rectangle(hConsole, 30, 5);

    char nom[100];
    gotoLigCol(y + 2, x + 7);
    scanf("%s", &nom);
    strcat(nom, ".tpgif");

    int retour = sauvegarde_fichier(nom);

    gotoLigCol(y + 4, x + 2);
    if (retour == 1) printf("Fichier bien enregistre");
    else if (retour == 0) printf("ERREUR");

    sleep(3);
}


/*
 * Partie de l'affichage de chargement d'un fichier
 */
void graph_charger_fichier(HANDLE hConsole){
    //Initialisation
    system("cls");
    system("color f0");
    SetConsoleTextAttribute(hConsole, 15*16);
    gotoLigCol(y + 1, x + 2);
    printf("Chargement d'un fichier");
    gotoLigCol(y + 2, x + 2);
    printf("Entree pour valider");

    PTfichier pPremierFichier = (PTfichier) malloc(sizeof(Tfichier));
    PTfichier pfichier = pPremierFichier;
    struct dirent *dir;
    DIR *d = opendir(".");
    char *p;
    int compteur = 0;

    // Lecture des fichiers
    if(d){
        while ((dir = readdir(d)) != NULL){
            p = strchr(dir->d_name, '.');
            if (p && !strcmp(p, ".tpgif")){
                //Enregistrement du nom des fichiers
                if (compteur != 0){
                    pfichier->ptsuivant = (PTfichier) malloc(sizeof(Tfichier));
                    pfichier = pfichier->ptsuivant;
                }
                strcpy(pfichier->nom, dir->d_name);
                pfichier->ptsuivant = NULL;
                compteur++;
            }
        }
        closedir(d);
    }

    //Cas où il n'y a pas de fichier
    if (!compteur){
        gotoLigCol(y + 4, x + 2);
        printf("Aucun fichier dispo");
        gotoLigCol(y + 5, x + 2);
        printf("Retour au menu dans 5 secondes");
        make_rectangle(hConsole, 35, 6);
        sleep(5);
        return;
    }
    //Cas où il y a des fichiers
    pfichier = pPremierFichier;
    for (int i = 0; i < compteur; ++i) {
        gotoLigCol(y + 4 + i, x + 4);
        printf("%s", pfichier->nom);
        pfichier = pfichier->ptsuivant;
    }

    make_rectangle(hConsole, 35, 5 + compteur);
    int index = 0;
    int i;

    while (1){
        //Nettoyage des croix
        for (int i = 0; i < compteur; ++i) {
            gotoLigCol(y + 4 + i, x + 2);
            printf(" ");
        }

        gotoLigCol(y + 4 + index, x + 2);
        printf("X");

        i = lireCaract();

        if (i == fleche_bas && index < (compteur - 1)) index ++;
        else if (i == fleche_haut && index != 0) index --;
        else if (i == entree){
            pfichier = pPremierFichier;
            for (int j = 0; j < index; ++j) {
                pfichier = pfichier->ptsuivant;
            }

            int retour = chargement_sauvegarde(pfichier->nom);
            gotoLigCol(y + 4 + compteur, x + 2);
            if (retour == 0) printf("ERREUR");
            else if (retour == 1) printf("Fichier bien charge en memoire");

            sleep(3);
            return;
        }
        else if (i == echap) break;
    }
}


/*
 * Cette fonction créé les boutons des centrales
 * avec la couleur par defaut
 */
void make_bouton_centrale_default(HANDLE hConsole, int nb){
    SetConsoleTextAttribute(hConsole, 15*16);
    make_bouton(hConsole, 0, 0);
    make_bouton(hConsole, 1, 0);


    for (int i = 0; i < nb; ++i) {
        gotoLigCol(y + 15 + (i * 3), x + 2);
        printf("SUPPR");
        gotoLigCol(y + 16 + (i * 3), x+2);
        printf("MODIF");
    }
}


/*
 * Partie de l'affichage de l'ajout de connexion
 */
void graph_ajouter_connexion(HANDLE hConsole, PTcentrale pcentrale){
    system("cls");
    system("color f0");
    make_dessin_centrale(y + 1, x + 12);

    gotoLigCol(y + 6, x + 2);
    printf("Ajouter une connexion");
    gotoLigCol(y + 7, x + 2);
    printf("Code postal ville:");
    gotoLigCol(y + 8, x + 2);
    printf("Puissance:");

    make_rectangle(hConsole, 35, 12);

    int code_postal, puissance;

    gotoLigCol(y + 7, x + 20);
    scanf("%d", &code_postal);
    gotoLigCol(y + 8, x + 13);
    scanf("%d", &puissance);

    gotoLigCol(y + 10, x + 2);
    if (check_code_postal_utilise(code_postal)){
        int retour = ajouter_connexion(pcentrale, get_adresse_ville(code_postal), puissance);
        if (retour == 0) printf("Puissance centrale insuffisante");
        else if (retour == 1) printf("Nouvelle connexion ajoutee");
        else if (retour == 2) {
            printf("Connexion deja existante");
            gotoLigCol(y + 11, x + 2);
            printf("Elle a ete modifiee");
        }
    }else printf("La ville n'existe pas");
    sleep(3);
}


/*
 * Partie de l'affichage des modifications de centrales
 */
void graph_modifier_centrale(HANDLE hConsole, PTcentrale pcentrale){
    system("cls");
    system("color f0");
    make_dessin_centrale(y + 1, x + 12);

    make_bouton(hConsole, 1, 0);
    make_bouton(hConsole, 2, 1);

    SetConsoleTextAttribute(hConsole, 15*16);
    gotoLigCol(y + 6, x + 2);
    printf("Nom: %s", pcentrale->nom);
    gotoLigCol(y + 7, x + 2);
    printf("Code: %d", pcentrale->codeCentrale);
    gotoLigCol(y + 8, x + 2);
    printf("Old puissance max: %d", pcentrale->puissance_max);
    gotoLigCol(y + 11, x + 2);
    printf("Appuyez sur Entree");
    make_rectangle(hConsole, 35, 14);

    // 0 = supprimer, 1 = modifier
    int index =0;
    int i;

    while (1){
        i = lireCaract();

        //Fleche gauche
        if (i == fleche_gauche && index == 1){
            make_bouton(hConsole, 1, 0);
            make_bouton(hConsole, 2, 1);
            index = 0;
        }
        //Fleche droite
        else if (i == fleche_droite && index == 0){
            make_bouton(hConsole, 1, 1);
            make_bouton(hConsole, 2, 0);
            index = 1;
        }
        //Entrée
        else if (i == entree){
            //Supprimer
            if (index == 0){
                int retour = supprimer_centrale(pcentrale->codeCentrale);
                gotoLigCol(y + 10, x + 2);
                if (retour == 1) printf("Centrale suprrimee");
                else if (retour == 0) printf("Inexistant");
                else if (retour == 2) printf("ERREUR");

                sleep(3);
                break;
            }
            // Modifier
            else if (index == 1){
                gotoLigCol(y + 9, x + 2);
                printf("New puissance max:");

                int puissance_max;
                gotoLigCol(y + 9, x + 21);
                scanf("%d", &puissance_max);

                int retour = modifier_centrale(pcentrale->codeCentrale, puissance_max);
                gotoLigCol(y + 12, x + 2);
                if (retour == 0) printf("ERREUR");
                else if (retour == 1) printf("Nouvelle puissance accepte");
                else if (retour == 2) {
                    printf("La puissance est insuffissante");
                    gotoLigCol(y + 13, x + 2);
                    printf("aux vues des connexions");
                }
                sleep(3);
                break;
            }
        }
        //Quitter
        else if (i == echap) break;
    }
}


/*
 * Partie de l'affichage des ajouts de centrales
 */
void graph_ajouter_centrale(HANDLE hConsole){
    system("cls");
    system("color f0");
    make_dessin_centrale(y + 1, x + 12);

    gotoLigCol(y + 6, x + 2);
    printf("Nom:");
    gotoLigCol(y + 7, x + 2);
    printf("Code:");
    gotoLigCol(y + 8, x + 2);
    printf("Puissance max:");

    gotoLigCol(y + 10, x + 2);
    printf("Appuyez sur Entree");

    make_rectangle(hConsole, 35, 13);

    char nom[50];
    int code, puissance_max;

    gotoLigCol(y + 6, x + 7);
    scanf("%s", &nom);
    gotoLigCol(y + 7, x + 8);
    scanf("%d", &code);
    gotoLigCol(y + 8, x + 17);
    scanf("%d", &puissance_max);

    int retour = ajout_centrale(code, puissance_max, nom);
    gotoLigCol(y + 11, x + 2);
    if (retour == 0) printf("Code deja utilise");
    else if (retour == 1) printf("Centrale ajoutee");

    sleep(3);
}

/*
 * Partie de l'affichage des centrales
 */
int affichage_centrale(HANDLE hConsole){
    PTcentrale pcentrale = pPremiereCentrale;
    PTligneElectrique pligne;
    int i;
    int nb_connexion;
    // 0 = nouvelle centrale, 1 = edit centrale, 2 = nouvelle connexion
    // Ensuite, si impaire = supprimer, si paire = modifier
    int index;

    // Partie centrale
    rebuild_centrale:

    index = 0;
    system("cls");
    system("color f0");
    make_dessin_centrale(y + 1, x + 12);

    SetConsoleTextAttribute(hConsole, 15*16);
    gotoLigCol(y + 6, x + 2);
    printf("Nom:");
    gotoLigCol(y + 7, x + 2);
    printf("Code:");
    gotoLigCol(y + 8, x + 2);
    printf("Puissance max:");
    gotoLigCol(y + 9, x + 2);
    printf("Puissance restante:");
    gotoLigCol(y + 13, x + 2);
    printf("Connexion avec les villes:");

    // Gestion des données
    while (1){
        SetConsoleTextAttribute(hConsole, 15*16);
        // Si il n'y a pas de centrale
        if (!nbcentrales){
            gotoLigCol(y + 6, x + 2);
            printf("Nom: Pas de centrale");
            gotoLigCol(y + 7, x + 2);
            printf("Code: --");
            gotoLigCol(y + 8, x + 2);
            printf("Puissance max: --");
            gotoLigCol(y + 9, x + 2);
            printf("Puissance restante: --");
        }
        // Si il y a des centrales
        else{
            gotoLigCol(y + 6, x + 2);
            printf("Nom: %s", pcentrale->nom);
            gotoLigCol(y + 7, x + 2);
            printf("Code: %d", pcentrale->codeCentrale);
            gotoLigCol(y + 8, x + 2);
            printf("Puissance max: %d", pcentrale->puissance_max);
            gotoLigCol(y + 9, x + 2);
            printf("Puissance restante: %d", get_puissance_restante_centrale(pcentrale));

            gotoLigCol(y + 11, x + 2);
            printf("*AJOUTER CONNEXION*");
        }

        // Affichage des connexions
        pligne = pcentrale->villeDependante;
        nb_connexion = 0;
        while (pligne && pligne->villeDesservie){
            gotoLigCol(y + 15 + (nb_connexion * 3), x + 8);
            printf("Nom: %s(%d)", pligne->villeDesservie->nom, pligne->villeDesservie->codePostal);
            gotoLigCol(y + 16 + (nb_connexion * 3), x + 8);
            printf("Puissance: %d", pligne->puissance);

            nb_connexion++;
            pligne = pligne->ligneSuivante;
        }

        // Ajout elements graphique
        make_rectangle(hConsole, 35, 15 + (3*nb_connexion));
        make_bouton_centrale_default(hConsole, nb_connexion);

        // Surlignage du bouton en cour
        SetConsoleTextAttribute(hConsole, 113);
        if (index == 0) make_bouton(hConsole, 0, 1);
        else if (index == 1) make_bouton(hConsole, 1, 1);
        else if (index == 2){
            gotoLigCol(y + 11, x + 2);
            printf("*AJOUTER CONNEXION*");
        }
        else if (index % 2 == 1){
            gotoLigCol(y + 15 + (((index/2)-1) * 3), x + 2);
            printf("SUPPR");
        }
         else if (index % 2 == 0){
            gotoLigCol(y + 16 + ((index-3)/2) * 3, x + 2);
            printf("MODIF");
        }

        // Lecture du clavier
        i = lireCaract();
        //Fleche droite
        if (i == fleche_droite){
            if (pcentrale->ptsuivant) {
                pcentrale = pcentrale->ptsuivant;
                goto rebuild_centrale;
            }
        }
        //Fleche gauche
        else if (i == fleche_gauche){
            if (pcentrale->ptprecedent) {
                pcentrale = pcentrale->ptprecedent;
                goto rebuild_centrale;
            }
        }
        //Echap
        else if (i == echap){
            return 1;
        }
        //Fleche haut
        else if (i == fleche_haut && index > 0) index--;
        //FLeche bas
        else if (i == fleche_bas && index < ((nb_connexion * 2) + 2)){
            if (nbcentrales) index ++;
        }
        //Entrée
        else if (i == entree){
            if (index == 0){
                graph_ajouter_centrale(hConsole);
                goto rebuild_centrale;
            }
            else if (index == 1){
                graph_modifier_centrale(hConsole, pcentrale);
                pcentrale = pPremiereCentrale;
                goto rebuild_centrale;
            }
            else if (index == 2){
                graph_ajouter_connexion(hConsole, pcentrale);
                goto rebuild_centrale;
            }
            //Supprimer connexion
            else if (index % 2 == 1){
                // On se place sur la première connexion
                pligne = pcentrale->villeDependante;
                for (int j = 0; j < (index-3)/2; ++j) {
                    pligne = pligne->ligneSuivante;
                }
                supprimer_connexion(pcentrale, pligne->villeDesservie);
                goto rebuild_centrale;

            }
            //Modifier connexion
            else if (index % 2 == 0){
                pligne = pcentrale->villeDependante;
                for (int j = 0; j < (index-3)/2; ++j) {
                    pligne = pligne->ligneSuivante;
                }
                gotoLigCol(y + 16 + ((index-3)/2) * 3, x + 19);
                SetConsoleTextAttribute(hConsole, 15*16);
                printf("                ");
                gotoLigCol(y + 16 + ((index-3)/2) * 3, x + 19);
                int puissance;
                scanf("%d", &puissance);
                int retour = modifier_connexion(pcentrale, pligne->villeDesservie, puissance);
                gotoLigCol(y + 17 + ((index-3)/2) * 3, x + 2);
                if (retour == 0) printf("ERREUR");
                else if (retour == 1) printf("Connexion modifie");
                else if (retour == 2) printf("Puissance centrale insuffisante");

                sleep(3);
                goto rebuild_centrale;
            }
        }
    }
}


/*
 * Cette fonction dessine une maison
 */
void make_dessin_maison(int posX, int posY){
    gotoLigCol(posX + 1, posY + 1);
    printf("      ^     ");
    gotoLigCol(posX + 2, posY + 1);
    printf("     / \\   ");
    gotoLigCol(posX + 3, posY + 1);
    printf("    /   \\   ");
    gotoLigCol(posX + 4, posY + 1);
    printf("   /     \\  ");
    gotoLigCol(posX + 5, posY + 1);
    printf("   -------   ");
    gotoLigCol(posX + 6, posY + 1);
    printf("   |     |    ");
    gotoLigCol(posX + 7, posY + 1);
    printf("   |     |   ");
    gotoLigCol(posX + 8, posY + 1);
    printf("   -------");
}


/*
 * Parti de l'affichage des villes
 */
void graph_ajouter_ville(HANDLE hConsole){
    int code_postal = 0;
    char nom[50];
    int retour = 0;

    system("cls");
    system("color f0");
    make_rectangle(hConsole, 35, 22);
    make_dessin_maison(y + 1, x + 9);

    gotoLigCol(y + 11, x + 2);
    printf("Nom ville:");
    gotoLigCol(y + 12, x + 2);
    printf("Code postal:");

    gotoLigCol(y + 11, x + 13);
    scanf("%s", &nom);
    gotoLigCol(y + 12, x + 15);
    scanf("%d", &code_postal);

    retour = ajouter_ville(code_postal, nom);
    if (retour){
        gotoLigCol(y + 15, x + 2);
        printf("La ville a bien ete ajoutee");
    }
    else {
        gotoLigCol(y + 15, x + 2);
        printf("Le code postal est deja utilise");
    }
    sleep(3);
}


void affichage_ville(HANDLE hConsole){
    creation:
    system("cls");
    system("color f0");
    make_rectangle(hConsole, 35, 22);

    make_dessin_maison(y + 1, x + 9);

    gotoLigCol(y + 11, x + 2);
    printf("Nom ville:");
    gotoLigCol(y + 12, x + 2);
    printf("Code postal:");
    gotoLigCol(y + 13, x + 2);
    printf("Puissance recu: ");

    gotoLigCol(y + 15, x + 2);
    printf("[*]Ajouter nouvelle ville");
    gotoLigCol(y + 16, x + 2);
    printf("[ ]Supprimer cette ville");

    gotoLigCol(y + 18, x + 2);
    printf("Echap pour revenir au menu");
    gotoLigCol(y + 19, x + 2);
    printf("Fleches pour se deplacer");
    gotoLigCol(y + 20, x + 2);
    printf("Entree pour valider");

    PTville pville = pPremiereVille;
    int i;
    // 0=Ajouter, 1=Supprimer
    int index = 0;


    while (1){
        //Si il n'y a pas de ville
        if (!nbville){
            gotoLigCol(y + 11, x + 13);
            printf("PAS DE VILLE          ");
            gotoLigCol(y + 12, x + 15);
            printf("PAS DE VILLE        ");
            gotoLigCol(y + 13, x + 18);
            printf("PAS DE VILLE     ");
            gotoLigCol(y + 16, x + 1);
            printf("                                  ");
            index = 0;
            gotoLigCol(y + 15, x + 2);
            printf("[*]Ajouter nouvelle ville");
        }
        else{
            //Affichage des informations
            gotoLigCol(y + 11, x + 13);
            printf("                      ");
            gotoLigCol(y + 12, x + 15);
            printf("                    ");
            gotoLigCol(y + 13, x + 18);
            printf("                 ");

            gotoLigCol(y + 11, x + 13);
            printf("%s", pville->nom);
            gotoLigCol(y + 12, x + 15);
            printf("%d", pville->codePostal);
            gotoLigCol(y + 13, x + 18);
            printf("%d MW", get_puissance_recu(pville));
        }

        i = lireCaract();

        //Fleche droite
        if (i == fleche_droite){
            if (pville->villeSuivante) pville = pville->villeSuivante;
        }
        //Fleche gauche
        else if(i == fleche_gauche){
            if (pville->villePrecedente) pville = pville->villePrecedente;
        }
        //Fleche bas
        else if(i == fleche_bas && index != 1 && nbville){
            index = 1;
            gotoLigCol(y + 15, x + 2);
            printf("[ ]Ajouter nouvelle ville");
            gotoLigCol(y + 16, x + 2);
            printf("[*]Supprimer cette ville");
        }
        //Fleche haut
        else if(i == fleche_haut && index != 0 && nbville){
            index = 0;
            gotoLigCol(y + 15, x + 2);
            printf("[*]Ajouter nouvelle ville");
            gotoLigCol(y + 16, x + 2);
            printf("[ ]Supprimer cette ville");
        }
        //Echap
        else if(i == echap) break;

        //Entrée
        else if (i == entree){
            //Supression ville
            if (index == 1){
                if (pville->villePrecedente) {

                    pville = pville->villePrecedente;
                    supprimer_ville(pville->villeSuivante->codePostal);
                }
                else if (pville->villeSuivante){
                    pville = pville->villeSuivante;
                    supprimer_ville(pville->villePrecedente->codePostal);
                }
                else {
                    supprimer_ville(pville->codePostal);
                    pville = pPremiereVille;
                }
            }
            else if (index == 0){
                graph_ajouter_ville(hConsole);
                goto creation;
            }
        }
    }
}


/*
 * Affichage de confirmation de quitter le programme
 */
int confirmer_quitter(HANDLE hConsole){
    system("cls");
    system("color f0");
    make_rectangle(hConsole, 38, 10);

    gotoLigCol(y + 3, x + 4);
    SetConsoleTextAttribute(hConsole, 244);
    printf("Voulez-vous vraiment quitter ?");

    gotoLigCol(y + 6, x + 6);
    SetConsoleTextAttribute(hConsole, 240);
    printf("[*]NON");
    gotoLigCol(y + 6, x + 17);
    printf("[ ]OUI");

    gotoLigCol(y + 8, x + 4);
    printf("Appuyez sur Entree");


    // 0=rester, 1=quitter
    int index = 0;
    int i;

    while (1){
        i = lireCaract();

        //Entree
        if (i == entree){
            if (index == 0) return 0;
            else return 1;
        }
        //Fleche droite
        else if(i == fleche_droite){
            gotoLigCol(y + 6, x + 6);
            SetConsoleTextAttribute(hConsole, 240);
            printf("[ ]NON");
            gotoLigCol(y + 6, x + 17);
            printf("[*]OUI");
            index = 1;
        }
        //Fleche gauche
        else if(i == fleche_gauche){
            gotoLigCol(y + 6, x + 6);
            SetConsoleTextAttribute(hConsole, 240);
            printf("[*]NON");
            gotoLigCol(y + 6, x + 17);
            printf("[ ]OUI");
            index = 0;
        }
    }
}


/*
 * Création du cadre pour le menu
 */
void make_cadre_menu(HANDLE hConsole, int x, int y, int largeur){
    SetConsoleTextAttribute(hConsole, 15*16);
    //Barres horizontale droite et gauche
    for (int i = y; i < 14; ++i) {
        gotoLigCol(i, x);
        printf("%c", cvertical);
        gotoLigCol(i, x + largeur - 1 );
        printf("%c", cvertical);
    }

    //Barres verticales
    for (int i = x; i < largeur + x; i ++){
        gotoLigCol(2, i);
        printf("%c", chorizontal);
        gotoLigCol(6, i);
        printf("%c", chorizontal);
        gotoLigCol(14, i);
        printf("%c", chorizontal);
    }

    // Angle gauche haut
    gotoLigCol(2, x);
    printf("%c", cangleGH);

    //Angle droite haut
    gotoLigCol(2, x + largeur - 1);
    printf("%c", cangleDH);

    //Angle milieu gauche
    gotoLigCol(6, x);
    printf("%c", cDoubleAngleGauche);

    //Angle milieu droit
    gotoLigCol(6, x + largeur - 1);
    printf("%c", cDoubleAngleDroite);

    //Angle bas gauche
    gotoLigCol(14, x);
    printf("%c", cangleGB);

    //Angle bas droite
    gotoLigCol(14, x + largeur - 1);
    printf("%c", cangleDB);

    //Menu
    gotoLigCol(4, x - 1 + largeur/2);
    printf("MENU");
    gotoLigCol(4, x);
}


/*
 * Affichage des différents choix
 */
void make_menu_default(HANDLE hConsole, int position_choix){

    SetConsoleTextAttribute(hConsole, 15*16);


    gotoLigCol(8, position_choix);
    printf("1 - Voir les villes");
    gotoLigCol(9, position_choix);
    printf("2 - Voir les centrales\n");
    gotoLigCol(10, position_choix);
    printf("3 - Enregistrer dans un fichier\n");
    gotoLigCol(11, position_choix);
    printf("4 - Charger un fichier\n");
    gotoLigCol(12, position_choix);
    printf("5 - Affichage resume\n");
}



/*
 * Gestion du menu
 */
void menu(){
    int i;
    int largeur = 40;
    int position_choix = 11; //x + 3

    rebuild:
    //Nettoyage et configuration de la console
    system("cls");
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    system("color f0");

    int index = 1;
    int couleur_selection = 113;


    //Surligne le pemier choix
    make_cadre_menu(hConsole, x, y, largeur);
    make_menu_default(hConsole, position_choix);
    gotoLigCol(8, position_choix);
    SetConsoleTextAttribute(hConsole, couleur_selection);
    printf("1 - Voir les villes");

    while (1) {
        i = lireCaract();

        if (i == fleche_haut && index > 1)index--;//Fleche haut

        else if (i == fleche_bas && index < 5) index ++;//fleche bas

        else if (i == echap) { //Echap
            if (confirmer_quitter(hConsole)) break;
            else goto rebuild;
        }

        else if (i == entree){ //Entrée
            gotoLigCol(1, 25);
            if (index == 1){
                //afficher les villes
                affichage_ville(hConsole);
                goto rebuild;

            } else if (index == 2){
                //Afficher les centrales
                affichage_centrale(hConsole);
                goto rebuild;
            } else if (index == 3){
                //Enregistrer dans un fichier
                graph_enrengistrer_fichier(hConsole);
                goto rebuild;
            } else if (index == 4){
                //Charger un fichier
                graph_charger_fichier(hConsole);
                goto rebuild;

            } else if (index == 5){
                //Affichage résumé
                affichage_resume(hConsole);
                goto rebuild;
            }
        }


        //Surligne le nouveau choix et met les autres choix à la normal
        make_menu_default(hConsole, position_choix);

        if (index == 1) {
            gotoLigCol(8, position_choix);
            SetConsoleTextAttribute(hConsole, couleur_selection);
            printf("1 - Voir les villes");
        }
        else if (index == 2) {
            gotoLigCol(9, position_choix);
            SetConsoleTextAttribute(hConsole, couleur_selection);
            printf("2 - Voir les centrales\n");
        }

        else if (index == 3) {
            gotoLigCol(10, position_choix);
            SetConsoleTextAttribute(hConsole, couleur_selection);
            printf("3 - Enregistrer dans un fichier\n");
        }

        else if (index == 4) {
            gotoLigCol(11, position_choix);
            SetConsoleTextAttribute(hConsole, couleur_selection);
            printf("4 - Charger un fichier\n");
        }

        else if (index == 5) {
            gotoLigCol(12, position_choix);
            SetConsoleTextAttribute(hConsole, couleur_selection);
            printf("5 - Affichage resume\n");
        }
    }
}

/////////////// Main ///////////////

int main() {
    //Création adresse première ville et centrale
    pPremiereCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pPremiereCentrale->ptprecedent = NULL;
    pPremiereCentrale->ptsuivant = NULL;
    pPremiereCentrale->codeCentrale = 0;
    pPremiereCentrale->puissance_max = 0;
    pPremiereCentrale->villeDependante = NULL;
    pPremiereVille = (PTville) malloc(sizeof (Tville));
    pPremiereVille->codePostal = -1;
    pPremiereVille->villeSuivante = NULL;

    // Programme graphique
    menu();

    return 0;
}
