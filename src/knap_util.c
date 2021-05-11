#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct knapsack_struct {
    int **tasks;
    int **knapsack;
    int **quantity;
    int capacity;
    int Q;
    int parts;
} Knapsack;

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

int save_knapsack(char *file_name, Knapsack *K){
    char *fn = malloc((strlen(file_name) + 5));
    strcpy(fn, file_name);
    strcat(fn, ".kn");
    FILE *fp = fopen(fn, "w");

    if (fp == NULL) {
        printf("Error saving file!\n");
        return -1;
    }
    fprintf(fp, "%d;\n", K->parts);
    fprintf(fp, "%d;\n", K->capacity-1);
    fprintf(fp, "%d;\n", K->Q);
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < K->parts; j++){
            fprintf(fp, "%d;", K->tasks[i][j]);
        }
        fprintf(fp, "\n");      
    }
    fclose(fp);
    return 0;
}

Knapsack* load_knapsack(char *file_name){
    char *fn = malloc((strlen(file_name) + 5));
    strcpy(fn, file_name);
    strcat(fn, ".fd");
    FILE *fp = fopen(fn, "r");
    if (fp == NULL){
        printf("File doesn't exits\n");
        exit(1);
    }
    int res;
    Knapsack *K = (Knapsack*)calloc(1,sizeof(Knapsack));

    res = fscanf(fp, "%d;\n", &K->parts);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }

    res = fscanf(fp, "%d;\n", &K->capacity);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }

    res = fscanf(fp, "%d;\n", &K->Q);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }
    
    int **tasks = init_matrix(2, K->parts);

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < K->parts; j++){
            res = fscanf(fp, "%d;\n", &tasks[i][j]);
            if(res < 1){
                printf("Incorrect file format\n");
                exit(1);
            }
        }
    }

    K->tasks = tasks;
    fclose(fp);
    return K;
}