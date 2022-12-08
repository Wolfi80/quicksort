#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FICHIER_ENTREE "ListeIUT2021niveau2.txt" // Choix du fichier non trié
#define FICHIER_SORTIE "Fichier_Out2.txt" // Choix du fichier de sortie

// Structure étudiant
struct etudiant{
    char *prenom;
    char *nom;
    int identifiant;
};

// Prototypes des fonctions et des procédures
int nombreLignes(FILE *fichier);
void swap(struct etudiant *a, struct etudiant *b);
int partition(struct etudiant *T, int moins, int plus);
void TriRapide(struct etudiant *T, int moins, int plus);

int main(void){
    FILE *fichierIn = NULL;
    FILE *fichierOut = NULL;
    struct etudiant *etudiants = NULL;
    int nbrLigne = 0;

    printf("Programme du groupe F-03.\n");

    fichierIn = fopen(FICHIER_ENTREE, "r"); // ouverture du fichier non trié en mode lecture
    if(fichierIn == NULL){ // si le fichier n'existe pas
        fprintf(stderr, "Le fichier n'a pas pu être ouvert\n"); // message d'erreur
        return EXIT_FAILURE; // on quitte le programme
    }

    nbrLigne = nombreLignes(fichierIn); // on stocke le resultat de la fonction "nombreLignes"
    etudiants = malloc(nbrLigne*sizeof(struct etudiant)); // allocation dynamique de la variable "etudiants"

    if(etudiants == NULL){ // si la variable "etudiants" ne contient rien
        fclose(fichierIn); // on ferme le fichier
        fprintf(stderr, "Erreur d'allocation.");
        return EXIT_FAILURE;
    }

    for(int i = 0; i < nbrLigne; i++){
        char prenombuff[256]; // variable temporaire
        char nombuff[256];
        fscanf(fichierIn, "%s %s %d", prenombuff, nombuff, &etudiants[i].identifiant); // on lit les chaînes formatées du fichier non trié
        etudiants[i].prenom = strdup(prenombuff); // on alloue et on affecte la valeur de notre variable temporaire a notre variable "etudiants"
        etudiants[i].nom = strdup(nombuff);
        if(etudiants[i].prenom == NULL || etudiants[i].nom == NULL){
            fprintf(stderr, "Pas assez de mémoire disponible.");
            return EXIT_FAILURE;
        }
    }

    TriRapide(etudiants,0,nbrLigne-1); // on effectue le tri rapide

    fichierOut = fopen(FICHIER_SORTIE, "w"); // ouverture du fichier en mode écriture
    if(fichierOut == NULL){ // si le fichier n'existe pas ou qu'il n'a pas pu être créé
        fprintf(stderr, "Le fichier n'a pas pu être ouvert\n");
        return EXIT_FAILURE;
    }

    for(int i = 0; i < nbrLigne; i++){
        if(i == nbrLigne-1){
            fprintf(fichierOut,"%s %s %d", etudiants[i].prenom, etudiants[i].nom, etudiants[i].identifiant); // on écrit dans le fichier
        }else{
            fprintf(fichierOut,"%s %s %d\n", etudiants[i].prenom, etudiants[i].nom, etudiants[i].identifiant);
        }
        free(etudiants[i].prenom); // on libère la mémoire
        free(etudiants[i].nom);
    }

    fclose(fichierOut); // on ferme le fichier trié
    free(etudiants);
    return 0;
}


int nombreLignes(FILE *fichier){ // fonction qui compte le nombre de lignes d'un fichier
    char tmp[256];
    int contenu = 0;
    int nbrLignes = 0;
    rewind(fichier); // on se place au début du fichier
    while(fgets(tmp, sizeof(tmp), fichier) != NULL){ // on lit le fichier tant que nous ne sommes pas en fin de fichier
        contenu = strlen(tmp);
        for(int i = 0; i < contenu; i++){
            if(tmp[i] == '\n'){
                nbrLignes++; // incrémentation de la variable "nbrLignes"
            }
        }
    }
    rewind(fichier);
    return nbrLignes+1; // on retourne le nombre de lignes du fichier
}


void swap(struct etudiant *a, struct etudiant *b){ // Procédure qui permet d'interchanger les valeurs de deux variables
    struct etudiant t = *a;
    *a = *b;
    *b = t;
}


int partition(struct etudiant *T, int moins, int plus){ // Fonction permettant de séparer la suite à trier en deux tableaux
    struct etudiant *pivot = &T[plus];
    int i = (moins - 1);
    for(int j = moins; j <= plus-1; j++){
        if(strcasecmp(T[j].nom, pivot->nom) <= 0){  // Compare la valeur j de la chaine de caractère à la valeur pivot. Si j est inférieur à pivot
            i++;
            swap(&T[i], &T[j]);  // Alors la procédure swap intervertit la valeur précédente à la valeur j pour la classer.
        }
    }
    swap(&T[i + 1], &T[plus]);  // La procédure swap intervertit la prochaine valeur avec la valeur de la variable plus
    return (i + 1);
}


void TriRapide(struct etudiant *T, int moins, int plus){ // Procédure appliquant le tri rapide
    if(moins < plus){ 
        int pivot = partition(T, moins, plus); // La variable pivot prend la valeur de la fonction partition
        TriRapide(T, moins, pivot - 1); // On appelle la procédure de tri à s'effectuer une première fois sur la partie basse du tableau
        TriRapide(T, pivot + 1, plus);  // Puis une deuxième fois sur la partie haute du tableau cette fois-ci
    }
}
