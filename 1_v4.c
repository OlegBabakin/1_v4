/*1. В файле data.dat задан двумерный массив целых чисел в следующем виде: в первой строке задано количества строк и столбцов массива, а далee в каждой строке файла задается одна строка двумерного массива. Элементы массива задаются через пробел. Ограничений на количества строк и столбцов в массиве нет.
Ввести двумерный массив из файла в массив в программе, отведя соответствующим образом память. Определить, существует ли в массиве элемент, равный среднему арифметическому всех элементов массива. Если существует, то уничтожить из массива строку с найденым элементом. Если найдено несколько элементов, то использовать только первый из них. Если элемент не найден, то матрица не изменяется. Вывести в файл data.res
полученный двумерный массив.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

int processing(long M, long N, FILE *in, FILE *out, long **matrix, long row_num); // function to solve problem
long** matrix_construct_file(FILE *in,long num_rows, long num_col); // function to construct matrix from file 'in'
void scan_len(FILE *file, long *len);
long row_num_(long **arr, long l_s, long l_c);
void row_del(long**arr, long l_s, long l_c, long str);
void print_file(FILE *file, long **arr, long l_s, long l_c, long str);

void print_file(FILE *file, long **arr, long l_s, long l_c, long str)
{
	if(l_s > 1 && str != -1)
	{
		for(int i = 0; i < l_s-1; i++)
		{
			for(int j = 0; j < l_c; j++)
        	{
           		fprintf(file, "%ld ", arr[i][j]);
			}
			fprintf(file, "\n");
		}
	}
	else
	{
		for(int i = 0; i < l_s; i++)
		{
			for(int j = 0; j < l_c; j++)
        	{
           		fprintf(file, "%ld ", arr[i][j]);
			}
			fprintf(file, "\n");
		}
	}
}

void row_del(long **arr, long *l_s, long l_c, long str)
{
	if(*l_s > 1 && str < *l_s-1 && str > -1)
	{
		for(int i = str+1; i < *l_s; i++)
		{
			arr[i-1] = arr[i];
		}
		*l_s--;
	}
	else if(*l_s > 1 && str == *l_s-1)
	{
		*l_s--;
		arr[str] = NULL;
	}
    return;
}

long row_num_(long **arr, long l_s, long l_c)
{
	double sum = 0.;
	double average = 0.;
	for(int i = 0; i < l_s; i++)
	{
		for(int j = 0; j < l_c; j++)
		{
			sum += arr[i][j];
		}
	}
	average = sum/(double)(l_c*l_s);
	for(long i = 0; i < l_s; i++)
	{
		for(int j = 0; j < l_c; j++)
		{
			if(fabs((double)arr[i][j] - average) < DBL_EPSILON)
			{
				return i;
			}
		}
	}
	return -1;
}

void scan_len(FILE *file, long *len)
{
	fscanf(file, "%ld", len);
}

long** matrix_construct_file(FILE *in, long num_rows, long num_col)
{
	int c = 0;
	long **matrix = NULL;
	matrix = (long**)malloc(num_rows*sizeof(long*)+num_rows*num_col*sizeof(long));
    matrix[0] = (long*)(matrix + num_rows);
	for(int i = 1; i < num_rows; i++)
	{
		matrix[i] = matrix[i-1]+num_col;
	}
	for(int i = 0; i < num_rows; i++)
	{
		for(int j = 0; j < num_col; j++)
		{
			if(fscanf(in, "%ld", &matrix[i][j]) == 1)
				c++;
		}
	} 
	if(c != num_rows*num_col)
	{
		free(matrix);
		return NULL;
	}
	return matrix;
}

int processing(long M, long N, FILE *in, FILE *out, long **matrix, long row_num)
{
	// trying to open file "data.txt" and "res.txt"
	in = fopen("data.txt", "r");
	if(in == NULL)
	{
		printf("ERROR_1\n");
		return -1;
	}
	out = fopen("res.txt", "w");
	if(out == NULL)
	{
		fclose(in);
		printf("ERROR_2\n");
		return -1;
	}

	// trying to scan M and N 
	scan_len(in, &M);
	scan_len(in, &N);
	if(M == 0 || N == 0)
	{
		printf("LENGTH IS 0\n");
		return -1;
	}

	// trying to construct matrix
	matrix = matrix_construct_file(in, M, N);
	if(matrix == NULL)
	{
		printf("ERROR_3\n");
		return -1;
	}

	row_num = row_num_(matrix, M, N); // searching number of row to delete
	row_del(matrix, &M, N, row_num); // 'deleting' some row of matrix
	print_file(out, matrix, M, N, row_num); // printing final matrix to file "res.txt"
	
	free(matrix);
	fclose(in);
	fclose(out);
}

int main(void)
{
	long M = 0, N = 0; // number of rows and number of columns of matrix
	long row_num = -1; // number of row to delete
	FILE *in = NULL, *out = NULL; // input file and output file
    long **matrix = NULL; // matrix
	processing(M, N, in, out, matrix, row_num); // solving problem
	return 0;
}