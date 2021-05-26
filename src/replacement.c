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
    int ***g;
    int initial_cost;
    int equipment_lifetime;
    int project_lifetime;
} Replacement;

void print_replacement(Replacement *R){
    printf("Project duration: %d\n", R->project_lifetime);
    printf("Initial Cost: %d\n", R->initial_cost);
    printf("Equipment lifetime: %d\n", R->equipment_lifetime);

    printf("Equipment");
    for(int i = 0; i < R->equipment_lifetime; i++)
        printf("Maintenance: %d Resale %d\n", R->time_units[i]->maintenance, R->time_units[i]->resale_price);
}

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

int fill_g(Replacement *R){
    int n = R->project_lifetime + 1;
    int m = R->equipment_lifetime;

    int **matrix = calloc(m, sizeof(int));
    for(int i = 0; i < m; i++){
        matrix[i] = calloc(n, sizeof(int));
        for(int j = 0; j < n; j++)
            matrix[i][j] = -1;
    }
    printf("Caca\n");
    int x = 0;
    matrix[m][0] = 0;
    for(int j = n-1; j > 0; j--){
        while(x < R->equipment_lifetime - j){
            printf("x = %d, t = %d\n", x, j);
            matrix[j][x] = matrix[j+1][x] + c(R,j,x);
            x++;
        }
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++)
            printf("G(%d) = %d", i, matrix[i][j]);
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