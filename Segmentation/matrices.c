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
