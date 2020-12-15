#include "matrices.h"

int CheckCoord(struct Matrix matrix, int x, int y)
{
	return (x < matrix.rows &&  y < matrix.columns); 
}

double Random()
{
        return (double)rand()/(double)RAND_MAX;
}

struct Matrix CreateMatrix(int rows, int columns)
{
	struct Matrix matrix;
       	
	matrix.rows = rows;
	matrix.columns = columns;

	matrix.mat = malloc(sizeof(double)*columns*rows);

	return matrix;
}

struct Matrix SetMatrix(int** mat, int rows, int cols)
{
	struct Matrix mat1 = CreateMatrix(rows,cols);
	for (int i = 0; i < rows; i++)
        {
                for(int j = 0; j < cols; j++)
                {
           	    ChangeMatrix(mat1, i, j, mat[i][j]);
                }
        }
	return mat1;
}

int** ReverseSetMatrix(struct Matrix mat)
{
	int **mat2;
    	mat2 = (int **) malloc(sizeof(int *) * mat.rows);
    	for (int i = 0; i < mat.rows; i++) mat2[i] = (int*) malloc(sizeof(int) * mat.columns);

        for (int i = 0; i < mat.rows; i++)
        {
                for(int j = 0; j < mat.columns; j++)
                {
                    mat2[i][j] = NavMatrix(mat, i, j);
                }
        }
        return mat2;
}

double NavMatrix(struct Matrix matrix, int x, int y)
{
	if (!CheckCoord(matrix, x, y))
	{
		printf("====================\n");
		printf("Error in coordinates\n");
		printf("====================\n");
	}
	int index = x * matrix.columns + y;
	return *(matrix.mat + index);
}

void ChangeMatrix(struct Matrix matrix, int x, int y, float val)
{
	if (!CheckCoord(matrix, x, y))
	{
		printf("====================\n");
                printf("Error in coordinates\n");
                printf("====================\n");
	}
        *(matrix.mat + x * matrix.columns + y) = val;
}

struct Matrix CopyMatrix(struct Matrix matrix, int x1, int y1, int x2, int y2)
{
	struct Matrix mat = CreateMatrix(x2-x1, y2-y1);

        for (int i = 0; i < mat.rows; i++)
        {
                for(int j = 0; j < mat.columns; j++)
                {
                        ChangeMatrix(mat, i, j, NavMatrix(matrix, i+x1, j+y1));
                }
        }
	return mat;
}


void PrintMatrix(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
	{
		for(int j = 0; j < matrix.columns; j++)
		{
			printf("%f ", NavMatrix(matrix, i, j));
		}
		printf("\n");
	}
}

void InitMatrix(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
        {
                for(int j = 0; j < matrix.columns; j++)
                {
                        ChangeMatrix(matrix, i, j, Random());
                }
        }    
}

void InitMatrixZero(struct Matrix matrix)
{
	for (int i = 0; i < matrix.rows; i++)
        {
                for(int j = 0; j < matrix.columns; j++)
                {
                        ChangeMatrix(matrix, i, j, 0);
                }
        }

}
