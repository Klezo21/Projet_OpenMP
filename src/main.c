#include "../include/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

/* Prototypes des fonctions internes au fichier main.c */
void createCSV(const char *filename);
void saveCSV(const char *filename, int N, int threads, double sequentialTime, double parallelTime);
void printBenchmark(const char *title, double sequentialTime, double parallelTime, int threads);

void runStrongScalability(void);
void runWeakScalability(void);

#define MATRIX_SIZE_STRONG 1000
#define BLOCK_SIZE 64

int main(void)
{
    // Initialisation du générateur de nombres aléatoires
    srand((unsigned int)time(NULL));

    printf("=============================================\n");
    printf(" PRODUIT MATRICIEL PARALLELE AVEC OPENMP\n");
    printf("=============================================\n");

    // 1. Exécution de l'étude de scalabilité forte
    runStrongScalability();

    // 2. Exécution de l'étude de scalabilité faible
    runWeakScalability();

    printf("\n=============================================\n");
    printf(" Fin du programme\n");
    printf("=============================================\n");

    return 0;
}

/**
 * Exécute l'étude de Scalabilité Forte
 * Garde la taille de la matrice fixe (N = 1000) et fait varier le nombre de threads.
 */
void runStrongScalability(void)
{
    const int N = MATRIX_SIZE_STRONG;
    int thread_cases[] = {1, 2, 4, 8, 16};
    int num_cases = sizeof(thread_cases) / sizeof(thread_cases[0]);
    const char *csv_filename = "results/strong_scalability.csv";

    printf("\n>>> DEBUT DE L'ETUDE : SCALABILITE FORTE (N = %d) <<<\n", N);
    createCSV(csv_filename);

    // Allocations des matrices de test
    double **A = allocateMatrix(N);
    double **B = allocateMatrix(N);
    double **C_seq = allocateMatrix(N);
    double **C_naive = allocateMatrix(N);
    double **C_block = allocateMatrix(N);

    // Remplissage des données initiales
    fillRandom(A, N);
    fillRandom(B, N);

    // --- ÉTAPE 1 : Calcul Séquentiel de Référence ---
    fillZero(C_seq, N);
    double start = getTime();
    multiplySequential(A, B, C_seq, N);
    double seqTime = getTime() - start;

    printf("Temps sequentiel de reference : %f secondes\n", seqTime);

    // --- ÉTAPE 2 : Variations des Threads ---
    for (int i = 0; i < num_cases; i++)
    {
        int threads = thread_cases[i];
        omp_set_num_threads(threads);

        // --- Version Naïve ---
        fillZero(C_naive, N);
        start = getTime();
        multiplyOpenMPNaive(A, B, C_naive, N);
        double parTimeNaive = getTime() - start;

        printf("\nNaive (%d threads) : ", threads);
        if (compareMatrices(C_seq, C_naive, N)) {
            printf("OK\n");
        } else {
            printf("ERREUR\n");
        }
        printBenchmark("OpenMP Naif", seqTime, parTimeNaive, threads);

        // --- Version par Blocs (Blocking) ---
        fillZero(C_block, N);
        start = getTime();
        multiplyOpenMPBlock(A, B, C_block, N, BLOCK_SIZE);
        double parTimeBlock = getTime() - start;

        printf("Blocking (%d threads) : ", threads);
        if (compareMatrices(C_seq, C_block, N)) {
            printf("OK\n");
        } else {
            printf("ERREUR\n");
        }
        printBenchmark("OpenMP Blocking", seqTime, parTimeBlock, threads);

        // Sauvegarde des métriques de la version Blocking pour les graphiques
        saveCSV(csv_filename, N, threads, seqTime, parTimeBlock);
    }

    // Libération mémoire
    freeMatrix(A, N);
    freeMatrix(B, N);
    freeMatrix(C_seq, N);
    freeMatrix(C_naive, N);
    freeMatrix(C_block, N);
}

/**
 * Exécute l'étude de Scalabilité Faible
 * Fait varier la taille de la matrice de façon proportionnelle à l'augmentation des threads
 * pour maintenir la charge de travail par thread constante (O(N^3)/p = constante).
 */
void runWeakScalability(void)
{
    int thread_cases[] = {1, 2, 4, 8, 16};
    int num_cases = sizeof(thread_cases) / sizeof(thread_cases[0]);
    const char *csv_filename = "results/weak_scalability.csv";

    printf("\n>>> DEBUT DE L'ETUDE : SCALABILITE FAIBLE (Charge/Thread Constante) <<<\n");
    createCSV(csv_filename);

    for (int i = 0; i < num_cases; i++)
    {
        int threads = thread_cases[i];
        
        // Ajustement géométrique de N : N_p = N_1 * cbrt(p)
        // Pour p=1, N=1000. Pour p=2, N ~ 1260, etc.
        int currentN = (int)(MATRIX_SIZE_STRONG * cbrt((double)threads));

        printf("\n----------------------------------\n");
        printf("Configuration Scalabilite Faible\n");
        printf("Threads : %d | Taille Matrice : %d x %d\n", threads, currentN, currentN);
        printf("----------------------------------\n");

        // Allocations dynamiques temporaires pour la taille courante
        double **WA = allocateMatrix(currentN);
        double **WB = allocateMatrix(currentN);
        double **WCseq = allocateMatrix(currentN);
        double **WCblock = allocateMatrix(currentN);

        fillRandom(WA, currentN);
        fillRandom(WB, currentN);
        fillZero(WCseq, currentN);
        fillZero(WCblock, currentN);

        // Temps séquentiel de référence pour cette taille spécifique
        double start = getTime();
        multiplySequential(WA, WB, WCseq, currentN);
        double seqTime = getTime() - start;

        // Temps parallèle optimisé (Blocking) pour cette taille spécifique
        omp_set_num_threads(threads);
        start = getTime();
        multiplyOpenMPBlock(WA, WB, WCblock, currentN, BLOCK_SIZE);
        double parTime = getTime() - start;

        printf("Validation : ");
        if (compareMatrices(WCseq, WCblock, currentN)) {
            printf("OK\n");
        } else {
            printf("ERREUR\n");
        }

        printBenchmark("Weak Scalability (Blocking)", seqTime, parTime, threads);
        saveCSV(csv_filename, currentN, threads, seqTime, parTime);

        // Libération de la mémoire de l'étape courante
        freeMatrix(WA, currentN);
        freeMatrix(WB, currentN);
        freeMatrix(WCseq, currentN);
        freeMatrix(WCblock, currentN);
    }
}