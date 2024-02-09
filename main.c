#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Structure représentant un tableau d'entiers longs
 */
typedef struct {
    /// Le tableau d'entiers longs
    long *tab;
    /// La dimension du tableau
    unsigned size;
} array;

/**
 * @brief Fonction ouvrant un fichier
 * @param f Le pointeur vers fichier
 * @param source L'adresse fichier
 * @param mode Le mode d'ouverture
 */
void openFile(FILE **f, char *source, char *mode) {
    *f = fopen(source, mode);
    if (*f == NULL) {
        fprintf(stderr, "Le fichier %s n'a pas pu être ouvert.\n", source);
        exit(1);
    } else {
        printf("Le fichier %s a été ouvert avec succès.\n", source);
    }
}

/**
 * @brief Fonction d'écriture binaire dans un fichier
 * @param f Le pointeur vers fichier
 * @param dest L'adresse fichier
 * @param tab Le tableau d'entiers longs à lire
 * @param size La dimension du tableau à lire
 */
void ecritTab(FILE *f, char *dest, long *tab, unsigned *size) {
    // On positionne notre curseur au début du fichier
    if (fseek(f, sizeof(unsigned), SEEK_SET)) {
        // On retourne les potentielles erreurs d'accès au fichier
        fprintf(stderr, "Problème lors de l'ouverture du fichier %s", dest);
        exit(2);
    }

    // On écrit sous forme d'un unsigned int la taille du tableau 
    if (fwrite(size, sizeof(unsigned), 1, f) == 0) {
        // On retourne les potentielles erreurs d'écriture
        fprintf(stderr, "Erreur lors de l'écriture de la dimension du fichier %s", dest);
        exit(3);
    }

    // On fait défiler le tableau
    for (unsigned i = 0; i < *size; i++) {
        // On écrit chaque case sous forme d'un long int
        if (fwrite(&tab[i], sizeof(long), 1, f) == 0) {
            // On retourne les potentielles erreurs d'écriture
            fprintf(stderr, "Erreur lors de l'écriture de la case %u : %ld", i, tab[i]);
            exit(3);
        }
    }
}

/**
 * @brief Fonction de lecture binaire dans un fichier
 * @param f Le pointeur vers fichier
 * @param dest L'adresse fichier
 * @param tab Le tableau d'entiers longs à implémenter
 * @param size La dimension du tableau à implémnter
 */
void litTab(FILE *f, char *source, long **tab, unsigned *size) {
    // On positionne notre curseur au début du fichier
    if (fseek(f, sizeof(unsigned), SEEK_SET)) {
        // On retourne les potentielles erreurs d'accès au fichier
        fprintf(stderr, "Problème lors de l'ouverture du fichier %s", source);
        exit(2);
    }

    // On lit dans le format unsigned int la dimension du tableau 
    if (fread(size, sizeof(unsigned), 1, f) == 0) {
        // On retourne les potentielles erreurs de lecture
        fprintf(stderr, "Erreur lors de la lecture de la dimension du fichier %s", source);
        exit(3);
    }

    // On alloue dynamiquement la mémoire nécessaires pour les éléments de type long int 
    *tab = malloc(*size * sizeof(long));
    // On fait défiler les données du fichiers par groupe de taille long int
    for (unsigned i = 0; i < *size; i++) {
        // On lit les données et les impélemente dans chaque case du tableau
        if (fread(&(*tab)[i], sizeof(long), 1, f) == 0) {
            // On retourne les potentielles erreurs de lecture
            fprintf(stderr, "Erreur lors de la lecture de la case %u : %ld", i, (*tab)[i]);
            exit(3);
        }
    }
}

/**
 * @brief Fonction retournant si deux arrays sont identiques
 * @param a Le premier array
 * @param b Le second array
 * @return bool : TRUE si a et b sont identiques ; FALSE sinon
 */
bool evaluate(array *a, array *b) {
    if (a->size != b->size)
        return false;

    for (unsigned i = 0; i < a->size; i++) {
        if (a->tab[i] != b->tab[i])
            return false;
    }
    return true;
}

int main() {
    // On génère un array
    array a;
    a.size = 9;
    a.tab = malloc(a.size * sizeof(long));
    if (a.tab == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        return 1;
    }
    for (long i = 0; i < a.size; i++)
        a.tab[i] = i + 1;

    // On ouvre l'écriture du fichier binaire
    char *source = "test.txt";
    FILE *f = NULL;
    openFile(&f, source, "wb");

    // On écrit dans le fichier
    printf("On commence l'écriture dans le fichier %s\n", source);
    ecritTab(f, source, a.tab, &a.size);
    printf("Écriture terminée\n");
    fclose(f);

    // On ouvre la lecture du fichier binaire
    f = NULL;
    openFile(&f, source, "rb");

    // On implémente un array selon la lecture
    array b;
    printf("On commence la lecture dans le fichier %s\n", source);
    litTab(f, source, &b.tab, &b.size);
    printf("Lecture terminée\n");
    fclose(f);
    f = NULL;

    // On vérifie qu'il n'y a pas eu d'erreur(s)
    if (evaluate(&a, &b))
        printf("Les structures array sont identiques !!\n");
    else
        fprintf(stderr, "Les structures array ne sont pas identiques...\n");

    // On libère la mémoire allouée
    free(a.tab); // Libérer la mémoire allouée pour a.tab
    free(b.tab); // Libérer la mémoire allouée pour b.tab
    return 0;
}
