#include "../include/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

/*
 * Alloue une matrice N x N
 */
double **allocateMatrix(int N)
{
    double **matrix = (double **)malloc(N * sizeof(double *));

    if (matrix == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++)
    {
        matrix[i] = (double *)malloc(N * sizeof(double));

        if (matrix[i] == NULL)
        {
            fprintf(stderr, "Erreur d'allocation mémoire.\n");
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}

/*
 * Libère une matrice
 */
void freeMatrix(double **matrix, int N)
{
    for (int i = 0; i < N; i++)
        free(matrix[i]);

    free(matrix);
}

/*
 * Remplit la matrice avec des valeurs aléatoires
 */
void fillRandom(double **matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = (double)(rand() % 100);
        }
    }
}

/*
 * Initialise une matrice à zéro
 */
void fillZero(double **matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = 0.0;
        }
    }
}

/*
 * Affiche une matrice
 */
void printMatrix(double **matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%8.2f ", matrix[i][j]);
        }

        printf("\n");
    }
}

/*
 * Compare deux matrices
 * Retourne :
 * 1 -> identiques
 * 0 -> différentes
 */
int compareMatrices(double **A, double **B, int N)
{
    const double EPSILON = 1e-9;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (fabs(A[i][j] - B[i][j]) > EPSILON)
            {
                return 0;
            }
        }
    }

    return 1;
}

/*
 * Retourne le temps courant
 */
double getTime()
{
    return omp_get_wtime();
}