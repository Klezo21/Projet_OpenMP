#include "../include/matrix.h"
#include <stdio.h>

/* * Récupération du pointeur de fichier et de la fonction de journalisation 
 * définis de manière globale dans le fichier main.c 
 */
extern FILE *log_file;
extern void logPrintf(const char *format, ...);

/*
 * ============================================================
 * Affiche les performances d'un test à l'écran et dans le fichier log
 * ============================================================
 */
void printBenchmark(const char *title,
                    double sequentialTime,
                    double parallelTime,
                    int threads)
{
    double speedup = sequentialTime / parallelTime;
    double efficiency = speedup / threads;

    logPrintf("\n==============================================\n");
    logPrintf("%s\n", title);
    logPrintf("==============================================\n");

    logPrintf("Temps séquentiel : %.6f s\n", sequentialTime);
    logPrintf("Temps parallèle  : %.6f s\n", parallelTime);
    logPrintf("Threads          : %d\n", threads);
    logPrintf("Speedup          : %.3f\n", speedup);
    logPrintf("Efficacité       : %.3f\n", efficiency);

    logPrintf("==============================================\n");
}

/*
 * ============================================================
 * Sauvegarde un résultat dans un fichier CSV
 * ============================================================
 */
void saveCSV(const char *filename,
             int N,
             int threads,
             double sequentialTime,
             double parallelTime)
{
    FILE *file = fopen(filename, "a");

    if (file == NULL)
    {
        // En cas d'erreur sur l'ouverture du CSV, on écrit l'alerte partout
        logPrintf("Impossible d'ouvrir le fichier CSV %s\n", filename);
        return;
    }

    double speedup = sequentialTime / parallelTime;
    double efficiency = speedup / threads;

    fprintf(file,
            "%d,%d,%.6f,%.6f,%.3f,%.3f\n",
            N,
            threads,
            sequentialTime,
            parallelTime,
            speedup,
            efficiency);

    fclose(file);
}

/*
 * ============================================================
 * Création d'un fichier CSV (Écrit l'en-tête de colonnes)
 * ============================================================
 */
void createCSV(const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        logPrintf("Impossible de créer le fichier CSV %s\n", filename);
        return;
    }

    fprintf(file,
            "MatrixSize,Threads,SequentialTime,ParallelTime,Speedup,Efficiency\n");

    fclose(file);
}