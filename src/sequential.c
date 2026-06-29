#include "../include/matrix.h"

/*
 * ============================================================
 * Produit matriciel séquentiel
 *
 * C = A × B
 *
 * Complexité : O(N³)
 *
 * Cette version sert de référence pour :
 *  - vérifier l'exactitude des versions parallèles
 *  - calculer le Speedup
 *  - mesurer les performances
 * ============================================================
 */

void multiplySequential(double **A,
                        double **B,
                        double **C,
                        int N)
{
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