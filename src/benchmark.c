#include "../include/matrix.h"

#include <stdio.h>

/*
 * ============================================================
 * Affiche les performances d'un test
 * ============================================================
 */

void printBenchmark(const char *title,
                    double sequentialTime,
                    double parallelTime,
                    int threads)
{
    double speedup = sequentialTime / parallelTime;

    double efficiency = speedup / threads;

    printf("\n==============================================\n");
    printf("%s\n", title);
    printf("==============================================\n");

    printf("Temps séquentiel : %.6f s\n", sequentialTime);
    printf("Temps parallèle  : %.6f s\n", parallelTime);

    printf("Threads          : %d\n", threads);

    printf("Speedup          : %.3f\n", speedup);

    printf("Efficacité       : %.3f\n", efficiency);

    printf("==============================================\n");
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
        printf("Impossible d'ouvrir %s\n", filename);
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
 * Création d'un fichier CSV
 * ============================================================
 */

void createCSV(const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Impossible de créer %s\n", filename);
        return;
    }

    fprintf(file,
            "MatrixSize,Threads,SequentialTime,ParallelTime,Speedup,Efficiency\n");

    fclose(file);
}