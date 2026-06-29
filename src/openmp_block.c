#include "../include/matrix.h"

#include <omp.h>

/*
 * ============================================================
 * Produit matriciel parallèle OpenMP avec Blocking (Tiling)
 *
 * C = A × B
 *
 * Le Blocking améliore la localité mémoire en travaillant
 * sur des sous-matrices (tiles).
 *
 * blockSize :
 *      16
 *      32
 *      64
 *      ...
 *
 * Ce paramètre sera étudié dans les tests de performances.
 * ============================================================
 */

void multiplyOpenMPBlock(double **A,
                         double **B,
                         double **C,
                         int N,
                         int blockSize)
{
    int ii, jj, kk;
    int i, j, k;

    /* Initialisation de C */
    fillZero(C, N);

    /*
     * Chaque thread reçoit un ensemble de blocs.
     */
#pragma omp parallel for collapse(2) private(ii,jj,kk,i,j,k) schedule(static)
    for (ii = 0; ii < N; ii += blockSize)
    {
        for (jj = 0; jj < N; jj += blockSize)
        {

            for (kk = 0; kk < N; kk += blockSize)
            {

                int iMax = (ii + blockSize < N) ? ii + blockSize : N;
                int jMax = (jj + blockSize < N) ? jj + blockSize : N;
                int kMax = (kk + blockSize < N) ? kk + blockSize : N;

                for (i = ii; i < iMax; i++)
                {
                    for (j = jj; j < jMax; j++)
                    {
                        double sum = C[i][j];

                        for (k = kk; k < kMax; k++)
                        {
                            sum += A[i][k] * B[k][j];
                        }

                        C[i][j] = sum;
                    }
                }

            }
        }
    }
}