#include "../include/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>

/* Fichier global pour la copie des sorties texte */
FILE *log_file = NULL;

/* Prototypes des fonctions internes au fichier main.c */
void logPrintf(const char *format, ...);
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

    // Ouverture du fichier texte pour enregistrer les sorties de la console
    // On s'assure que le dossier results existe, ou on le crée à la racine
    log_file = fopen("results/output.txt", "w");
    if (log_file == NULL) {
        printf("Attention : Impossible de creer le fichier results/output.txt\n");
    }

    logPrintf("=============================================\n");
    logPrintf(" PRODUIT MATRICIEL PARALLELE AVEC OPENMP\n");
    logPrintf("=============================================\n");

    // 1. Exécution de l'étude de scalabilité forte
    runStrongScalability();

    // 2. Exécution de l'étude de scalabilité faible
    runWeakScalability();

    logPrintf("\n=============================================\n");
    logPrintf(" Fin du programme\n");
    logPrintf("=============================================\n");

    // Fermeture propre du fichier log s'il a été ouvert
    if (log_file != NULL) {
        fclose(log_file);
    }

    return 0;
}

/**
 * Fonction personnalisée qui écrit à la fois sur l'écran (stdout)
 * et dans le fichier texte results/output.txt
 */
void logPrintf(const char *format, ...)
{
    va_list args;
    
    // Écriture sur l'écran (Console)
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // Écriture simultanée dans le fichier texte (.txt)
    if (log_file != NULL) {
        va_start(args, format);
        vfprintf(log_file, format, args);
        va_end(args);
        fflush(log_file); // Force l'écriture immédiate sur le disque
    }
}

/**
 * Exécute l'étude de Scalabilité Forte
 */
void runStrongScalability(void)
{
    const int N = MATRIX_SIZE_STRONG;
    int thread_cases[] = {1, 2, 4, 8, 16};
    int num_cases = sizeof(thread_cases) / sizeof(thread_cases[0]);
    const char *csv_filename = "results/strong_scalability.csv";

    logPrintf("\n>>> DEBUT DE L'ETUDE : SCALABILITE FORTE (N = %d) <<<\n", N);
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

    logPrintf("Temps sequentiel de reference : %f secondes\n", seqTime);

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

        logPrintf("\nNaive (%d threads) : ", threads);
        if (compareMatrices(C_seq, C_naive, N)) {
            logPrintf("OK\n");
        } else {
            logPrintf("ERREUR\n");
        }
        printBenchmark("OpenMP Naif", seqTime, parTimeNaive, threads);

        // --- Version par Blocs (Blocking) ---
        fillZero(C_block, N);
        start = getTime();
        multiplyOpenMPBlock(A, B, C_block, N, BLOCK_SIZE);
        double parTimeBlock = getTime() - start;

        logPrintf("Blocking (%d threads) : ", threads);
        if (compareMatrices(C_seq, C_block, N)) {
            logPrintf("OK\n");
        } else {
            logPrintf("ERREUR\n");
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
 */
void runWeakScalability(void)
{
    int thread_cases[] = {1, 2, 4, 8, 16};
    int num_cases = sizeof(thread_cases) / sizeof(thread_cases[0]);
    const char *csv_filename = "results/weak_scalability.csv";

    logPrintf("\n>>> DEBUT DE L'ETUDE : SCALABILITE FAIBLE (Charge/Thread Constante) <<<\n");
    createCSV(csv_filename);

    for (int i = 0; i < num_cases; i++)
    {
        int threads = thread_cases[i];
        
        // Ajustement géométrique de N : N_p = N_1 * cbrt(p)
        int currentN = (int)(MATRIX_SIZE_STRONG * cbrt((double)threads));

        logPrintf("\n----------------------------------\n");
        logPrintf("Configuration Scalabilite Faible\n");
        logPrintf("Threads : %d | Taille Matrice : %d x %d\n", threads, currentN, currentN);
        logPrintf("----------------------------------\n");

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

        logPrintf("Validation : ");
        if (compareMatrices(WCseq, WCblock, currentN)) {
            logPrintf("OK\n");
        } else {
            logPrintf("ERREUR\n");
        }

        printBenchmark("Weak Scalability (Blocking)", seqTime, parTime, threads);
        saveCSV(csv_filename, currentN, threads, seqTime, parTime);

        // Libération de la mémoire
        freeMatrix(WA, currentN);
        freeMatrix(WB, currentN);
        freeMatrix(WCseq, currentN);
        freeMatrix(WCblock, currentN);
    }
}