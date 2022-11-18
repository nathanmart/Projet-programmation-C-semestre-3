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


//Structure des villes
typedef struct ville {
    int codePostal;
    char nom[30];
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
 * Cette fonction renvoi l'adresse d'une centrale
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


/////////////// Fonctions de développement ///////////////

/*
 * Cette fonction est utile seulement pour le développement.
 * Elle créé 3 centrales et 3 villes.
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
 * Renvoie 1 si il reste assez de puissance
 * Renvoi 0 si il n'y a plus assez de puissance
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


/*
 * Cette fonction vérifie s'il ne manque pas de données pour la sauvegarde
 * Renvoie 0 si tout est bon
 * Renvoie 1 si une ville X n'a pas de nom
 * Renvoie 2 si la ville X à un code postal déjà utilisé
 * Renvoie 3 si la centrale X à un code de centrale déjà utilisé
 * Renvoie 4 si la centrale n'a pas de début de liste de lignes connectées
 * Renvoie 5 si la puissance restante dans la centrale est négative
 * Renvoie 6 si une ville à une puissance mais n'a pas de centrale assignée
 */
int check_presauvegarde_fichier(){
    PTcentrale pcentrale = pPremiereCentrale;
    PTville pville = pPremiereVille;
    PTligneElectrique pligne = NULL;

    if(nbville){
        while(pville){
            if(! pville->nom) return 1;
            else if(check_code_postal_utilise(pville->codePostal)) return 2;
            pville = pville->villeSuivante;
        }
    }

    if(nbcentrales){
        while(pcentrale){
            if(check_code_centrale_utilise(pcentrale->codeCentrale)) return 3;
            else if(! pcentrale->villeDependante) return 4;
            else if(get_puissance_restante_centrale(pcentrale) < 0) return 5;
            pligne = pcentrale->villeDependante;
            if(! pligne->puissance && ! pligne->villeDesservie)
                while(pligne) {
                    if (pligne->puissance && !pligne->villeDesservie) return 6;
                    pligne = pligne->ligneSuivante;
                }
            pcentrale = pcentrale->ptsuivant;
        }
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
 * Renvoie 2 si existe déjà
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
 */
int supprimer_centrale(int code_centrale){
    if(! check_existance_centrale(code_centrale)) return 0;

    PTcentrale pcentrale = pPremiereCentrale;

    // Dans le cas où il n'y a q'une seule centrale
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
        pPremiereCentrale = pcentrale->ptsuivant;
        nbcentrales --;
        return 1;
    }
    // Dans le cas où la centrale supprimée est la dernière
    while(pcentrale->ptsuivant) pcentrale = pcentrale->ptsuivant;
    if(pcentrale->codeCentrale == code_centrale) pcentrale->ptprecedent->ptsuivant = NULL;

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
 * Renvoie 1 si tout s'est bien passé
 * Renvoie 0 si la connexion n'existe pas
 */
int modifier_connexion(PTcentrale pcentrale, PTville pville, int puissance){
    PTligneElectrique pligne = pcentrale->villeDependante;
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
        pville = pville->villeSuivante;
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
        nbville--;
        return 1;
    }
        //Autres cas
    else {
        //Se place sur la ville précédent la suppression
        while (pville->villeSuivante->codePostal != code_postal) pville = pville->villeSuivante;
        pville->villeSuivante = pville->villeSuivante->villeSuivante;
        nbville--;
        return 1;
    }
}


/////////////// Fonctions sauvegarde ///////////////

/*
 * Cette fonction créée une sauvegarde des données
 * Le chemin doit être envoyé selon le format spécfique à l'OS.
 * Pour windows exemple :"C:\\Users\\nathan\\Documents\\sauvegarde.txt"
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


/////////////// Fonctions graphique ///////////////



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
        printf("Enrengistrer dans un fichier: 9\n");
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
            else if (retour == 1) printf("Centrale bien cree");
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
            else if (retour == 1) printf("Connexion cree");
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
            scanf("%d", puissance);
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

/////////////// Graphique ///////////////

int lireCaract(){

    char c=10;
    int fnt=0;

    c=getch();
    fnt=c;
    if (c==-32){c=getch();fnt = 400+c;}
    if (c==0)  {c=getch();fnt = 500+c;}
    return fnt;
}


void gotoLigCol( int lig, int col )
{
    // ressources
    COORD mycoord;

    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

int cvertical=186;
int chorizontal=205;
int cangleGH=201;
int cangleGB=200;
int cangleDH=187;
int cangleDB=188;
int cDoubleAngleGauche = 204;
int cDoubleAngleDroite = 185;

int largeur = 40;
int j = 8;
int position_choix = 11; //j + 3

void make_cadre(HANDLE hConsole){
    SetConsoleTextAttribute(hConsole, 15*16);
    //Barres horizontale droite et gauche
    for (int i = 2; i < 14; ++i) {
        gotoLigCol(i, j);
        printf("%c", cvertical);
        gotoLigCol(i, j + largeur - 1 );
        printf("%c", cvertical);
    }

    //Barres verticales
    for (int i = j; i < largeur + j; i ++){
        gotoLigCol(2, i);
        printf("%c", chorizontal);
        gotoLigCol(6, i);
        printf("%c", chorizontal);
        gotoLigCol(14, i);
        printf("%c", chorizontal);
    }

    // Angle gauche haut
    gotoLigCol(2, j);
    printf("%c", cangleGH);

    //Angle droite haut
    gotoLigCol(2, j + largeur - 1);
    printf("%c", cangleDH);

    //Angle milieu gauche
    gotoLigCol(6, j);
    printf("%c", cDoubleAngleGauche);

    //Angle milieu droit
    gotoLigCol(6, j + largeur - 1);
    printf("%c", cDoubleAngleDroite);

    //Angle bas gauche
    gotoLigCol(14, j);
    printf("%c", cangleGB);

    //Angle bas droite
    gotoLigCol(14, j + largeur - 1);
    printf("%c", cangleDB);


    //Menu
    gotoLigCol(4, j - 1 + largeur/2);
    printf("MENU");
    gotoLigCol(4, j);

}


void make_ville_default( HANDLE hConsole){

    SetConsoleTextAttribute(hConsole, 15*16);


    gotoLigCol(8, position_choix);
    printf("1 - Voir les villes");
    gotoLigCol(9, position_choix);
    printf("2 - Voir les centrales\n");
    gotoLigCol(10, position_choix);
    printf("3 - Enrengistrer dans un fichier\n");
    gotoLigCol(11, position_choix);
    printf("4 - Charger un fichier\n");
    gotoLigCol(12, position_choix);
    printf("5 - Affichage resume\n");
}

void menu(){
    int i;
    system("cls");
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    system("color f0");

    int index = 1;
    int couleur_selection = 113;

    make_cadre(hConsole);
    make_ville_default(hConsole);
    gotoLigCol(8, position_choix);
    SetConsoleTextAttribute(hConsole, couleur_selection);
    printf("1 - Voir les villes");

    while (1) {
        i = lireCaract();
        //Fleche haut
        if (i == 472 && index > 1) index--;
        //fleche bas
        else if (i == 480 && index < 5) index ++;
        else if (i == 27) break; //Echap
        else if (i == 13){ //Entrée
            gotoLigCol(1, 25);
            if (index == 1){
                printf("Villes");
                //afficher les villes
            } else if (index == 2){
                printf("Centrales");
                //Afficher les centrales
            } else if (index == 3){
                printf("Enrengistrer");
                //Enrengistrer dans un fichier
            } else if (index == 4){
                printf("Charger");
                //Charger un fichier
            } else if (index == 5){
                printf("Affichage");
                //Affichage résumé
            }
        }

        make_ville_default( hConsole);

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
            printf("3 - Enrengistrer dans un fichier\n");
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
    pPremiereVille = (PTville) malloc(sizeof (Tville));

    menu();


    scanf("%d", pPremiereCentrale);

    return 0;
}
