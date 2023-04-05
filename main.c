//
//  main.c
//  matrix
//
//  Created by Yassaman Ommi on 2022-10-20.
//  Copyright © 2022 Yassaman Ommi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


///////////////////////////////////////////////
// Read Matrix Function
//////////////////////////////////////////////
// Ref: https://stackoverflow.com/questions/48936647/read-a-matrix-from-a-file
double **read_matrix(size_t *rows, size_t *cols, const char *filename)
{
    if(rows == NULL || cols == NULL || filename == NULL)
        return NULL;
    
    *rows = 0;
    *cols = 0;
    
    FILE *fp = fopen(filename, "r");
    
    if(fp == NULL)
    {
        fprintf(stderr, "could not open %s: %s\n", filename, strerror(errno));
        return NULL;
    }
    
    double **matrix = NULL, **tmp;
    
    char line[1024];
    
    while(fgets(line, sizeof line, fp))
    {
        if(*cols == 0)
        {
            // determine the size of the columns based on
            // the first row
            char *scan = line;
            double dummy;
            int offset = 0;
            while(sscanf(scan, "%lf%n", &dummy, &offset) == 1)
            {
                scan += offset;
                (*cols)++;
            }
        }
        
        tmp = (double **) realloc(matrix, (*rows + 1) * sizeof *matrix);
        
        if(tmp == NULL)
        {
            fclose(fp);
            return matrix; // return all you've parsed so far
        }
        
        matrix = tmp;
        
        matrix[*rows] = (double *) calloc(*cols, sizeof *matrix[*rows]);
        
        if(matrix[*rows] == NULL)
        {
            fclose(fp);
            if(*rows == 0) // failed in the first row, free everything
            {
                fclose(fp);
                free(matrix);
                return NULL;
            }
            
            return matrix; // return all you've parsed so far
        }
        
        int offset = 0;
        char *scan = line;
        for(size_t j = 0; j < *cols; ++j)
        {
            if(sscanf(scan, "%lf%n", matrix[*rows] + j, &offset) == 1)
                scan += offset;
            else
                matrix[*rows][j] = 0; // could not read, set cell to 0
        }
        
        // incrementing rows
        (*rows)++;
    }
    
    fclose(fp);
    
    return matrix;
}

///////////////////////////////////////////////
// Write Matrix Function
//////////////////////////////////////////////
void write_matrix(double **matrix, size_t *rows, size_t *cols, const char *filename) {
    FILE *fp;
    fp = fopen(filename, "w");
    for (int i = 0; i < (int) *rows; i++) {
        for (int j = 0; j < (int) *cols; j++) {
            fprintf(fp, "%lf ", matrix[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

///////////////////////////////////////////////
// Matrix Memory Function
//////////////////////////////////////////////
void free_matrix_memory(double **matrix, size_t *rows) {
    for(size_t i = 0; i < *rows; ++i)
        free(matrix[i]);
    free(matrix);
}

///////////////////////////////////////////////
// Check if char * is a number string
//////////////////////////////////////////////
int is_number(char *s) {
    for (int i = 0; s[i]!= '\0'; i++) {
        if (isdigit(s[i]) == 0 && s[i] != '.') {
            return 0;
        }
    }
    return 1;
}

///////////////////////////////////////////////
// 1. Matrix Addition Function
// input: matrix a, a rows, a columns, matrix b, b rows, b columns
// output: matrix c, which is the sum of a and b
//////////////////////////////////////////////
double **matrix_add(double **a_matrix, size_t *a_rows, size_t *a_cols, double **b_matrix, size_t *b_rows, size_t *b_cols) {
    if (*a_rows != *b_rows || *a_cols != *b_cols) {
        return NULL;
    }
    double **c_matrix = (double **)malloc(*a_rows * sizeof(double*));
    for (int i = 0; i < (int) *a_rows; i++) {
        c_matrix[i] = (double *)malloc(*a_cols * sizeof(double));
    }
    for (int i = 0; i < (int) *a_rows; i++) {
        for (int j = 0; j < (int) *a_cols; j++) {
            c_matrix[i][j] = a_matrix[i][j] + b_matrix[i][j];
        }
    }
    return c_matrix;
}

///////////////////////////////////////////////
// 2. Matrix Subtraction  Function
// input: matrix a, a rows, a columns, matrix b, b rows, b columns
// output: matrix c, which is the substraction of a and b
//////////////////////////////////////////////
double **matrix_subtract(double **a_matrix, size_t *a_rows, size_t *a_cols, double **b_matrix, size_t *b_rows, size_t *b_cols) {
    if (*a_rows != *b_rows || *a_cols != *b_cols) {
        return NULL;
    }
    double **c_matrix = (double **)malloc(*a_rows * sizeof(double*));
    for (int i = 0; i < (int) *a_rows; i++) {
        c_matrix[i] = (double *)malloc(*a_cols * sizeof(double));
    }
    for (int i = 0; i < (int) *a_rows; i++) {
        for (int j = 0; j < (int) *a_cols; j++) {
            c_matrix[i][j] = a_matrix[i][j] - b_matrix[i][j];
        }
    }
    return c_matrix;
}

///////////////////////////////////////////////
// 3. Matrix Multiplication Function
// input: matrix a, a rows, a columns, matrix b, b rows, b columns
// output: matrix c, which is the product of a and b
//////////////////////////////////////////////
double **matrix_multiply(double **a_matrix, size_t *a_rows, size_t *a_cols, double **b_matrix, size_t *b_rows, size_t *b_cols) {
    if (*a_cols != *b_rows) {
        return NULL;
    }
    double **c_matrix = (double **)malloc(*a_rows * sizeof(double*));
    for (int i = 0; i < (int) *a_rows; i++) {
        c_matrix[i] = (double *)malloc(*b_cols * sizeof(double));
    }
    for (int i = 0; i < (int) *a_rows; i++) {
        for (int j = 0; j < (int) *b_cols; j++) {
            c_matrix[i][j] = 0;
            for (int k = 0; k < (int) *a_cols; k++) {
                c_matrix[i][j] += a_matrix[i][k] * b_matrix[k][j];
            }
        }
    }
    return c_matrix;
}

///////////////////////////////////////////////
// 4. Scalar Multiplication Function
// input: matrix a, a rows, a columns, scalar
// output: matrix c, which is the product of the scalar in the matrix
//////////////////////////////////////////////
double **matrix_multiply_scalar(double **a_matrix, size_t *a_rows, size_t *a_cols, double scalar) {
    double **c_matrix = (double **)malloc(*a_rows * sizeof(double*));
    for (int i = 0; i < (int) *a_rows; i++) {
        c_matrix[i] = (double *)malloc(*a_cols * sizeof(double));
    }
    for (int i = 0; i < (int) *a_rows; i++) {
        for (int j = 0; j < (int) *a_cols; j++) {
            c_matrix[i][j] = scalar * a_matrix[i][j];
        }
    }
    return c_matrix;
}

///////////////////////////////////////////////
// 5. Equal Matrix Function
// input: matrix a, a rows, a columns, matrix b, b rows, b columns
// output: prints if the two matrices are equal or not
//////////////////////////////////////////////
void matrix_equal(double **a_matrix, size_t *a_rows, size_t *a_cols, double **b_matrix, size_t *b_rows, size_t *b_cols) {
    if (*a_rows != *b_rows || *a_cols != *b_cols) {
        printf("Not equal.\n");
        return;
    }
    for (int i = 0; i < (int) *a_rows; i++) {
        for (int j = 0; j < (int) *a_rows; j++) {
            if (a_matrix[i][j] != b_matrix[i][j]) {
                printf("Not equal.\n");
                return;
            }
        }
    }
    printf("Equal.\n");
}

///////////////////////////////////////////////
// 6. Matrix  Trace Function
// input: matrix a, a rows, a columns
// output: prints the trace of the matrix
//////////////////////////////////////////////
void matrix_trace(double **a_matrix, size_t *a_rows, size_t *a_cols) {
    if (*a_rows != *a_cols) {
        printf("Matrix is not square. :( \n");
    }
    double trace = 0;
    for (int i = 0; i < (int) *a_rows; i++) {
        trace += a_matrix[i][i];
    }
    printf("Trace = %lf.\n", trace);
}

///////////////////////////////////////////////
// 7. Matrix Determinant Function
/////////
/////////
// 7.1 Row and Column Exclusion Function
// input: matrix, matrix's rows, matrix's columns, the index of the row to be excluded, the index of the column to be excluded
// output: the matrix with excluded row and column
///////////////////////////////////////////////
double **exclude_row_and_col(double **matrix, size_t *rows, size_t *cols, int ex_row, int ex_col) {
    double **excluded = (double **)malloc((*rows - 1) * sizeof(double*));
    for (int i = 0; i < (int) *rows; i++) {
        excluded[i] = (double *)malloc((*cols - 1) * sizeof(double));
    }

    for (int i = 0; i < (int) *rows; i++) {
        if (i == ex_row) {
            continue;
        }
        for (int j = 0; j < (int) *cols; j++) {
            if (j == ex_col) {
                continue;
            }
            int ex_i = i > ex_row ? i - 1 : i;
            int ex_j = j > ex_col ? j - 1 : j;
            excluded[ex_i][ex_j] = matrix[i][j];
        }
    }

    return excluded;
}
/////////
/////////
// 7.2 Matrix Determinent Function
// input: matrix, matrix's number if rows
// output: the matrix's determinent
///////////////////////////////////////////////

double matrix_det(double **matrix, size_t *rows) {
    if (*rows == 1) {
        return matrix[0][0];
    }
    double det = 0;
    for (int i = 0; i < (int) *rows; i++) {
        double coeff = i % 2 == 0 ? 1 : -1;
        size_t excluded_rows = *rows - 1;
        double **excluded = exclude_row_and_col(matrix, rows, rows, 0, i);
        det += coeff * matrix[0][i] * matrix_det(excluded, &excluded_rows);
        free_matrix_memory(excluded, &excluded_rows);
    }
    return det;
}


///////////////////////////////////////////////
// 8. Matrix Power Function
// input: matrix a, a rows, a columns, power as integer
// output: matrix c, which is the power of a
///////////////////////////////////////////////
double **matrix_power(double **a_matrix, size_t *a_rows, size_t *a_cols, int power) {
    double **c_matrix = (double **)malloc(*a_rows * sizeof(double*));
    for (int i = 0; i < (int) *a_rows; i++) {
        c_matrix[i] = (double *)malloc(*a_cols * sizeof(double));
    }
    for (int i = 0; i < (int) *a_cols; i++) {
        for (int j = 0; j < (int) *a_rows; j++) {
            c_matrix[i][j] = (double) a_matrix[i][j];
        }
    }
    
    for (int i = 0; i < power - 1; i++) {
        c_matrix = matrix_multiply(a_matrix, a_rows, a_cols, c_matrix, a_rows, a_cols);
    }

    return c_matrix;
}

///////////////////////////////////////////////
// Command Line
//////////////////////////////////////////////
int main(int argc, char *argv[]) {
    char *operation = argv[1];
    
    if (strcmp(operation, "add") == 0) {
        if (argc == 5) {
            size_t a_cols, a_rows, b_cols, b_rows;
            double **a_matrix = read_matrix(&a_rows, &a_cols, argv[2]);
            double **b_matrix = read_matrix(&b_rows, &b_cols, argv[3]);
            
            if(a_matrix == NULL || b_matrix == NULL) {
                fprintf(stderr, "could not read matrix\n");
                return 1;
            }
            
            double **c_matrix = matrix_add(a_matrix, &a_rows, &a_cols, b_matrix, &b_rows, &b_cols);
            
            if (c_matrix == NULL) {
                printf("Matrix sizes do not match.\n");
            }
            else {
                write_matrix(c_matrix, &a_rows, &a_cols, argv[4]);
                // because if sizes do match c will have the same row numbers as a & b
                free_matrix_memory(c_matrix, &a_rows);
            }
            
            free_matrix_memory(a_matrix, &a_rows);
            free_matrix_memory(b_matrix, &b_rows);
        }
        else {
            printf("Wrong number of arguments.\n");
        }
    }
    else if (strcmp(operation, "subtract") == 0) {
        if (argc == 5) {
            size_t a_cols, a_rows, b_cols, b_rows;
            double **a_matrix = read_matrix(&a_rows, &a_cols, argv[2]);
            double **b_matrix = read_matrix(&b_rows, &b_cols, argv[3]);
            
            if(a_matrix == NULL || b_matrix == NULL) {
                fprintf(stderr, "could not read matrix\n");
                return 1;
            }
            
            double **c_matrix = matrix_subtract(a_matrix, &a_rows, &a_cols, b_matrix, &b_rows, &b_cols);
            
            if (c_matrix == NULL) {
                printf("Matrix sizes do not match.\n");
            }
            else {
                write_matrix(c_matrix, &a_rows, &a_cols, argv[4]);
                // because if sizes do match c will have the same row numbers as a & b
                free_matrix_memory(c_matrix, &a_rows);
            }
            
            free_matrix_memory(a_matrix, &a_rows);
            free_matrix_memory(b_matrix, &b_rows);
        }
        else {
            printf("Wrong number of arguments.\n");
        }
    }
    else if (strcmp(operation, "multiply") == 0) {
        if (argc == 5) {
            size_t a_cols, a_rows, b_cols, b_rows;
            double **a_matrix = read_matrix(&a_rows, &a_cols, argv[2]);
            
            if (is_number(argv[3])) {
                double scalar;
                sscanf(argv[3], "%lf", &scalar);

                if (a_matrix == NULL) {
                    fprintf(stderr, "could not read matrix\n");
                    return 1;
                }

                double **c_matrix = matrix_multiply_scalar(a_matrix, &a_rows, &a_cols, scalar);
                write_matrix(c_matrix, &a_rows, &a_cols, argv[4]);
                
                free_matrix_memory(c_matrix, &a_rows);
                free_matrix_memory(a_matrix, &a_rows);
            }
            else {
                double **b_matrix = read_matrix(&b_rows, &b_cols, argv[3]);
            
                if(a_matrix == NULL || b_matrix == NULL) {
                    fprintf(stderr, "could not read matrix\n");
                    return 1;
                }
                
                double **c_matrix = matrix_multiply(a_matrix, &a_rows, &a_cols, b_matrix, &b_rows, &b_cols);
                
                if (c_matrix == NULL) {
                    printf("Cannot multiply these dimensions.\n");
                }
                else {
                    write_matrix(c_matrix, &a_rows, &b_cols, argv[4]);
                    // because if sizes do match c will have the same row numbers as a
                    free_matrix_memory(c_matrix, &a_rows);
                }
                
                free_matrix_memory(a_matrix, &a_rows);
                free_matrix_memory(b_matrix, &b_rows);
            }
        }
        else {
            printf("Wrong number of arguments.\n");
        }
    }
    else if (strcmp(operation, "equal") == 0) {
        if (argc == 4) {
            size_t a_cols, a_rows, b_cols, b_rows;
            double **a_matrix = read_matrix(&a_rows, &a_cols, argv[2]);
            double **b_matrix = read_matrix(&b_rows, &b_cols, argv[3]);
            
            if(a_matrix == NULL || b_matrix == NULL) {
                fprintf(stderr, "could not read matrix\n");
                return 1;
            }
            
            matrix_equal(a_matrix, &a_rows, &a_cols, b_matrix, &b_rows, &b_cols);
            
            free_matrix_memory(a_matrix, &a_rows);
            free_matrix_memory(b_matrix, &b_rows);
        }
        else {
            printf("Wrong number of arguments.\n");
        }
    }
    else if (strcmp(operation, "trace") == 0) {
        if (argc == 3) {
            size_t a_cols, a_rows;
            double **a_matrix = read_matrix(&a_rows, &a_cols, argv[2]);
            
            if(a_matrix == NULL) {
                fprintf(stderr, "could not read matrix\n");
                return 1;
            }
            
            matrix_trace(a_matrix, &a_rows, &a_cols);
            
            free_matrix_memory(a_matrix, &a_rows);
        }
        else {
            printf("Wrong number of arguments.\n");
        }
    }
    else if (strcmp(operation, "det") == 0) {
        if (argc == 3) {
            size_t a_cols, a_rows;
            double **a_matrix = read_matrix(&a_rows, &a_cols, argv[2]);
            
            if(a_matrix == NULL) {
                fprintf(stderr, "could not read matrix\n");
                return 1;
            }

            double det = matrix_det(a_matrix, &a_rows);
            printf("det: %lf\n", det);
            
            free_matrix_memory(a_matrix, &a_rows);
        }
        else {
            printf("Wrong number of arguments.\n");
        }
    }
    else if (strcmp(operation, "power") == 0) {
        if (argc == 5) {
            size_t a_cols, a_rows;
            double **a_matrix = read_matrix(&a_rows, &a_cols, argv[2]);
            int power = atoi(argv[3]);
            
            if(a_matrix == NULL) {
                fprintf(stderr, "could not read matrix\n");
                return 1;
            }

            double **c_matrix = matrix_power(a_matrix, &a_rows, &a_cols, power);
            
            write_matrix(c_matrix, &a_rows, &a_cols, argv[4]);
            
            free_matrix_memory(a_matrix, &a_rows);
        }
        else {
            printf("Wrong number of arguments.\n");
        }
    }
    else {
        printf("%s: Unknown operation %s\n", operation, operation);
    }
}
