#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

double **allocateMatrix(int N);

void freeMatrix(double **matrix, int N);

void fillRandom(double **matrix, int N);

void fillZero(double **matrix, int N);

void printMatrix(double **matrix, int N);

int compareMatrices(double **A, double **B, int N);

double getTime();

void multiplySequential(double **A,
                        double **B,
                        double **C,
                        int N);

void multiplyOpenMPNaive(double **A,
                         double **B,
                         double **C,
                         int N);

void multiplyOpenMPBlock(double **A,
                         double **B,
                         double **C,
                         int N,
                         int blockSize);


void printBenchmark(const char *title,
                    double sequentialTime,
                    double parallelTime,
                    int threads);

void createCSV(const char *filename);

void saveCSV(const char *filename,
             int N,
             int threads,
             double sequentialTime,
             double parallelTime);

#endif