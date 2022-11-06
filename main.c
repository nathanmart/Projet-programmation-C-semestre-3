//Nathan MARTORANA
//Pierre BOTHUA
//
//Projet GIF
//Automne 2022

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

//Compteur nombre ville && centrale
int nbville = 0;
int nbcentrales = 0;

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
    nbville++;

    //Création deuxieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 2;
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));
    nbville++;

    //Création troisieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 3;
    pville->villeSuivante = (PTville) malloc(sizeof (Tville));
    nbville++;

    //Création quatrieme ville
    pville = pville->villeSuivante;
    pville->codePostal = 4;
    pville->villeSuivante = NULL;
    nbville++;


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
    nbcentrales++;

    //Création deuxième centrale
    pcentrale->ptsuivant->ptprecedent = pcentrale;
    pcentrale = pPremiereCentrale->ptsuivant;
    pcentrale->codeCentrale = 2;
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
 * Cette fonction permet de modifier une centrale
 * Renvoie 0 si erreur
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
 * Renvoie 0 si erreur
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
    pcentrale->villeDependante = (PTligneElectrique)malloc(sizeof (TlignesElectrique));
    pcentrale->villeDependante->villeDesservie = NULL;
    pcentrale->villeDependante->ligneSuivante = NULL;
    pcentrale->villeDependante->puissance = 0;
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
 * Cette fonction additionne une nouvelle puissance avec celle existante d'une connexion
 * Renvoie 1 si tout s'est bien passé
 * Renvoie 0 en cas d'erreur
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
 * Renvoie 0 en cas d'erreur
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

    //Si la connexion créée est la première
    PTligneElectrique pligne = pcentrale->villeDependante;
    if(! pligne->villeDesservie){
        pligne->villeDesservie = pville;
        pligne->puissance = puissance;
        return 1;
    }

    //S'il y a déjà une connexion sur la centrale

    //On vient se placer à la dernière connexion
    //La nouvelle connexion est donc ajoutée à la fin de la liste
    pligne = pcentrale->villeDependante;
    while(pligne->ligneSuivante != NULL) pligne = pligne->ligneSuivante;

    //Dans le cas où il y a déja une connexion sur cette centrale, on se place sur la connexion suivante

    pligne->ligneSuivante = (PTligneElectrique) malloc(sizeof(TlignesElectrique));
    pligne = pligne->ligneSuivante;

    //Paramétrage de la nouvelle connexion
    pligne->ligneSuivante = NULL;
    pligne->villeDesservie = pville;
    pligne->puissance = puissance;

    return 1;
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
        return 1;
    }
    //Cas où la connexion à supprimer est la première
    else if (pligne->villeDesservie == pville){
        pcentrale->villeDependante = pligne->ligneSuivante;
        return 1;
    }
    //Autres cas
    else {
        //Se place sur la ligne précédent la suppression
        while (pligne->ligneSuivante->villeDesservie != pville) pligne = pligne->ligneSuivante;
        pligne->ligneSuivante = pligne->ligneSuivante->ligneSuivante;
        return 1;
    }
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
    while (pcentrale){
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

int main() {
    //Création adresse première ville et centrale
    pPremiereCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pPremiereVille = (PTville) malloc(sizeof (Tville));

    ajouter_ville(56100, "Lorient");
    ajouter_ville(56000, "Vannes");
    ajouter_ville(56880, "Ploeren");
    ajout_centrale(1, 1673, "Paris");
    ajout_centrale(2, 2891, "Strasbourg");
    ajout_centrale(3, 1345, "Lyon");
    ajouter_connexion(get_adresse_centrale(1), get_adresse_ville(56000), 1000);
    ajouter_connexion(get_adresse_centrale(2), get_adresse_ville(56100), 2000);
    ajouter_connexion(get_adresse_centrale(2), get_adresse_ville(56000), 500);
    ajouter_connexion(get_adresse_centrale(3), get_adresse_ville(56880), 1000);



    affichage_general();

    return 0;
}
