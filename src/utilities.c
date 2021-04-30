#include <stdlib.h>
#include <stdio.h>

int* init_array(int n){
    int *array = calloc(n, sizeof(int));
    return array;
}

int** init_matrix(int n, int m){
    int **matrix = calloc(n, sizeof(int*));
    for(int i = 0; i < n; i++){
        matrix[i] = calloc(m, sizeof(int));
    }
    return matrix;
}

void free_matrix(int **matrix, int n){
    for(int i = 0; i < n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(int **matrix, int n, int m){
    for(int i = 0; i < n; i++){
        printf("%d |", i);
        for(int j = 0; j < m; j++){
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_array(int *array, int n){
    for(int i = 0; i < n; i++){
        printf("%d\t", array[i]);
    }
    printf("\n");
}

void test(int i){
    printf("caca%d\n",i);
}
