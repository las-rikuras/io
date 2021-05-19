#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct knap_task_struct {
    int weight;
    int value;
} knap_task;

typedef struct knapsack_struct {
    knap_task **tasks;
    int **knapsack;
    int **quantity;
    int *copies;
    int capacity;
    int Q;
    int parts;
    int type;
} Knapsack;

int** get_solution(Knapsack *self);

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

void print_knapsack(Knapsack *K){
    printf("Parts: %d\n", K->parts);
    printf("Capacity: %d\n", K->capacity-1);
    printf("Max: %d\n", K->Q);
    printf("====== Parts ======\n");
    for(int i = 0; i < K->parts; i++){
        printf("Value: %d\tWeight: %d\n", K->tasks[i]->value, K->tasks[i]->weight);
    }
    printf("===== Knapsack ====\n");
    print_matrix(K->knapsack, K->capacity, K->parts);
    printf("===== Solution ====\n");
    int **sol = get_solution(K);
    print_matrix(sol, 2, K->parts);
    free_matrix(sol, 2);
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
    fprintf(fp, "%d;\n", K->capacity);
    fprintf(fp, "%d;\n", K->Q);
    fprintf(fp, "%d;\n", K->type);

    for(int i = 0; i < K->parts; i++){
        fprintf(fp, "%d;%d;\n", K->tasks[i]->value, K->tasks[i]->weight);
    }

    for(int i = 0; i < K->parts; i++){
        fprintf(fp, "%d;\n", K->copies[i]);
    }
    fclose(fp);
    return 0;
}

Knapsack* load_knapsack(char *file_name){
    FILE *fp = fopen(file_name, "r");
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

    res = fscanf(fp, "%d;\n", &K->type);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }
    
    K->tasks = malloc(K->parts * sizeof(knap_task));


    for(int i = 0; i < K->parts; i++){
        K->tasks[i] = malloc(sizeof(knap_task));
        res = fscanf(fp, "%d;%d;\n", &K->tasks[i]->value, &K->tasks[i]->weight);
        if(res < 1){
            printf("Incorrect file format\n");
            exit(1);
        }
    }

    K->copies = calloc(K->parts, sizeof(int));
    for(int i = 0; i< K->parts; i++){
        K->tasks[i] = malloc(sizeof(knap_task));
        res = fscanf(fp, "%d;\n", &K->copies[i]);
        if(res < 1){
            printf("Incorrect file format\n");
            exit(1);
        }
    }
    
    fclose(fp);
    return K;
}