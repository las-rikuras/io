#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct time_unit_struct {
    int maintenance;
    int resale_price;
} Unit;

typedef struct replacement_struct {
    Unit **time_units;
    int **g;
    int **changes;
    int initial_cost;
    int equipment_lifetime;
    int project_lifetime;
} Replacement;

int c(Replacement *R, int t, int x){
    int n = x - t;
    int maintenance = 0;
    for(int i = 0; i < n; i++)
        maintenance += R->time_units[i]->maintenance;
    return R->initial_cost + maintenance - R->time_units[n-1]->resale_price;
}

int** init_matrix(int n, int m){
    int **matrix = calloc(n, sizeof(int*));
    for(int i = 0; i < n; i++){
        matrix[i] = calloc(m, sizeof(int));
    }
    return matrix;
}

int get_min(Replacement *R, int **matrix, int col){
    int min = matrix[0][col];
    for(int i = 1; i < R->equipment_lifetime; i++){
        if(matrix[i][col] != -1 && matrix[i][col] < min)
            min = matrix[i][col];
    }

    for(int i = 0; i < R->equipment_lifetime; i++){
        if(matrix[i][col] != -1 && matrix[i][col] == min && col != R->project_lifetime)
            R->changes[i][col] = 1;
    }
    return min;
}

int get_next_nodes_size(Replacement *R, int time_unit){
    int n = 0;
    for(int i = 0; i < R->equipment_lifetime; i++){
        if(R->changes[i][time_unit])
            n++;
    }  
    return n; 
}

int* get_next_nodes(Replacement *R, int time_unit){
    int n = get_next_nodes_size(R, time_unit);
    int *res = calloc(n, sizeof(int));
    int  index = 0;
    for(int i = 0; i < R->equipment_lifetime; i++){
        if(R->changes[i][time_unit])
            res[index++] = time_unit + i + 1;          
    }
    return res;
}

int** fill_g(Replacement *R){
    int n = R->project_lifetime + 1;
    int m = R->equipment_lifetime;

    int **matrix = init_matrix(n, m);
    
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++)
            matrix[i][j] = -1;
    }

    int x = 0;
    matrix[x][n-1] = 0;
    int dif;
    for(int t = n-2; t >= 0; t--){
        dif = R->project_lifetime - t <= R->equipment_lifetime ? R->project_lifetime - t : R->equipment_lifetime;
        while(x < dif){
            matrix[x][t] = get_min(R, matrix, t+x+1) + c(R,t,t+x+1);
            x++;
        }
        x = 0;
    }
    return matrix;
}

void print_replacement(Replacement *R){
    printf("Project duration: %d\n", R->project_lifetime);
    printf("Initial Cost: %d\n", R->initial_cost);
    printf("Equipment lifetime: %d\n", R->equipment_lifetime);

    printf("Equipment\n");
    for(int i = 0; i < R->equipment_lifetime; i++)
        printf("Maintenance: %d Resale %d\n", R->time_units[i]->maintenance, R->time_units[i]->resale_price);

    printf("\n");
    for(int i = R->project_lifetime; i >= 0; i--)
        printf("G(%d) = %d\n", i, get_min(R, R->g, i));

    printf("\nChanges\n");
    for(int i = 0; i < R->equipment_lifetime; i++){
        for(int j = 0; j < R->project_lifetime+1; j++){
            printf("%d ", R->changes[i][j]);
        }
        printf("\n");
    }
}


void init_replacement(Replacement *R, int initial_cost, int equipment_lifetime, int project_lifetime, int **time_units){
    R->initial_cost = initial_cost;
    R->equipment_lifetime = equipment_lifetime;
    R->project_lifetime = project_lifetime;

    R->time_units = malloc(R->equipment_lifetime * sizeof(Unit));
    for(int i = 0; i < R->equipment_lifetime; i++){
        R->time_units[i] = malloc(sizeof(Unit));
        R->time_units[i]->maintenance = time_units[i][0];
        R->time_units[i]->resale_price = time_units[i][1];
    }   
    R->changes = init_matrix(R->project_lifetime + 1, R->equipment_lifetime);
    R->g = fill_g(R);
}

void init_replacement_from_file(Replacement *R){  
    R->changes = init_matrix(R->project_lifetime + 1, R->equipment_lifetime);
    R->g = fill_g(R);
}

int save_replacement(char *file_name, Replacement *R){
    char *fn = malloc((strlen(file_name) + 5));
    strcpy(fn, file_name);
    strcat(fn, ".rp");
    FILE *fp = fopen(fn, "w");

    if (fp == NULL) {
        printf("Error saving file!\n");
        return -1;
    }
    fprintf(fp, "%d;\n", R->initial_cost);
    fprintf(fp, "%d;\n", R->project_lifetime);
    fprintf(fp, "%d;\n", R->equipment_lifetime);

    for(int i = 0; i < R->equipment_lifetime; i++){
        fprintf(fp, "%d;%d;\n", R->time_units[i]->maintenance, R->time_units[i]->resale_price);
    }

    fclose(fp);
    return 0;
}

Replacement* load_replacement(char *file_name){
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL){
        printf("File doesn't exits\n");
        exit(1);
    }
    int res;
    Replacement *R = malloc(sizeof(Replacement));

    res = fscanf(fp, "%d;\n", &R->initial_cost);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }

    res = fscanf(fp, "%d;\n", &R->project_lifetime);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }

    res = fscanf(fp, "%d;\n", &R->equipment_lifetime);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }

    R->time_units = malloc(R->equipment_lifetime * sizeof(Unit));
    for(int i = 0; i < R->equipment_lifetime; i++){
        R->time_units[i] = malloc(sizeof(Unit));
        res = fscanf(fp, "%d;%d;\n", &R->time_units[i]->maintenance, &R->time_units[i]->resale_price);
        if(res < 1){
            printf("Incorrect file format\n");
            exit(1);
        }
    }
    fclose(fp);
    return R;
}

/*
int main(){
    int equipment_lifetime = 3;
    int initial_cost = 500;
    int project_lifetime = 5;

    int equipment[3][2] = {
        {30, 400},
        {40, 300},
        {60, 250},
    };

    int **matrix = calloc(3, sizeof(int*));
    for(int i = 0; i < equipment_lifetime; i++){
        matrix[i] = calloc(2, sizeof(int));
        matrix[i][0] = equipment[i][0];
        matrix[i][1] = equipment[i][1];
    }

    Replacement *R = malloc(sizeof(Replacement));

    init_replacement(R, initial_cost, equipment_lifetime, project_lifetime, matrix);
    print_replacement(R);

    fill_g(R);

    save_replacement("ejemplo1", R);
    load_replacement("ejemplo1.rp");
}
*/