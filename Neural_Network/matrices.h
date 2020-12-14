#ifndef MATRICES_H
#define MATRICES_H

#include <stdio.h>
#include <stdlib.h>

struct Matrix
{
  int rows;
  int columns;

  double *mat;
};

void ChangeMatrix(struct Matrix matrix, int x, int y, float val);

double NavMatrix(struct Matrix matrix, int x, int y);

void InitMatrix(struct Matrix matrix);

void InitMatrixZero(struct Matrix matrix);

struct Matrix CreateMatrix(int rows, int columns);

void PrintMatrix(struct Matrix matrix);

double Random();

#endif
