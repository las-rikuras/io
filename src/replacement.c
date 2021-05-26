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

int g(Replacement *R, int x, int t){
    // G(5) = 0
    // G(4) = C(4,5) + G(5)
    // G(3) = C(3,4) + G(4)
    //        C(3, 5) + G(5)
    if(t == R->project_lifetime)
        return 0;

    printf("c[%d][%d](%d) + G(%d)(%d)\n", x, t+1, c(R, x, t+1), t+1, g(R, x, t+1));
    return c(R, t, t+1) + g(R, t, t+1);  
}


int get_min(Replacement *R, int **matrix, int col){
    int min = matrix[0][col];
    for(int i = 1; i < R->equipment_lifetime; i++){
        if(matrix[i][col] != -1 && matrix[i][col] < min)
            min = matrix[i][col];
    }
    return min;
}

int** fill_g(Replacement *R){
    int n = R->project_lifetime + 1;
    int m = R->equipment_lifetime;

    int **matrix = calloc(m, sizeof(int));
    for(int i = 0; i < m; i++)
        matrix[i] = calloc(n, sizeof(int));
    
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
            //printf("[%d][%d] = C[%d][%d] = %d + min(G(%d)) = %d\n", x, t, t, t+x+1, c(R,t,t+x+1), t+1, get_min(R, matrix, t+1));
            //printf("[%d][%d] =  %d + %d\n", x, t, c(R,t,t+x+1), get_min(R, matrix, t+1));
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
    R->g = fill_g(R);
}

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
}