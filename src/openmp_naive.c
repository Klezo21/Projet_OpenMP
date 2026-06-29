#include "../include/matrix.h"

#include <omp.h>

/*
 * ============================================================
 * Produit matriciel parallèle OpenMP (Version naïve)
 *
 * Chaque thread calcule une partie des lignes de la matrice C.
 *
 * La répartition est statique :
 *      schedule(static)
 *
 * Complexité :
 *      O(N³)
 *
 * Les accès mémoire restent identiques à la version séquentielle.
 * ============================================================
 */

void multiplyOpenMPNaive(double **A,
                         double **B,
                         double **C,
                         int N)
{
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            double sum = 0.0;

            for (int k = 0; k < N; k++)
            {
                sum += A[i][k] * B[k][j];
            }

            C[i][j] = sum;
        }
    }
}