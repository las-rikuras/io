#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "knap_util.c"

int task_weight(Knapsack *self, int j, int k){
    return self->tasks[j]->weight * k;
}

int prev(Knapsack *self, int i, int j){
    return self->knapsack[i][j - 1];
}

int get_max(Knapsack *self, int i, int j, int k){ 
    int v_j = self->tasks[j]->value * k;
    int c_j = task_weight(self, j, k);
    int new = v_j + self->knapsack[i-c_j][j-1];
    return prev(self, i, j) < new ? new : prev(self, i, j);
}

void bounded_knapsack(Knapsack *self){
    for(int i = 0; i < self->capacity; i++){
        for(int j = 0; j < self->parts; j++){
            int q = i/self->tasks[j]->weight > self->Q ? self->Q : i/self->tasks[j]->weight;
            for(int k = 0; k <= q; k++){
                if(task_weight(self, j, k) <= i){
                    int t1 = get_max(self, i, j, k);
                    int t2 = self->knapsack[i][j];

                    if(t1 > t2){
                        self->knapsack[i][j] = t1;
                        self->quantity[i][j] = k;
                    } 
                } 
            }   
        }
    }
}

void setup_knapsack(Knapsack *self, int max, int p, int c, int *v, int *w){
    self->Q = max;
    self->capacity = c + 1;
    self->parts = p;
    self->tasks = malloc(p * sizeof(knap_task));
    self->knapsack = init_matrix(self->capacity, p);
    self->quantity = init_matrix(self->capacity, p);
    for(int i = 0; i < p; i++){  
        self->tasks[i] = malloc(sizeof(knap_task));
        self->tasks[i]->value = v[i];
        self->tasks[i]->weight = w[i];
    }
}

void setup_knapsack_from_file(Knapsack *self){
    self->capacity = self->capacity + 1;
    self->knapsack = init_matrix(self->capacity, self->parts);
    self->quantity = init_matrix(self->capacity, self->parts);
}

int** get_solution(Knapsack *self){
    int **sol = init_matrix(2, self->parts);
    int i = self->capacity-1;
    for(int j = self->parts-1; j >= 0; j--){
        sol[0][j] = j+1;
        if(self->quantity[i][j] != 0){
            sol[1][j] = self->quantity[i][j];
            i = i - task_weight(self, j, self->quantity[i][j]);
        } 
    }
    return sol;
}

// #define p 3
// #define p 7
#define p 4

int main(int argc, char *argv[]){
    // from file
    // Knapsack *K = load_knapsack("chirripo.kn");
    // setup_knapsack_from_file(K);
    // from interface
 
    // int v[p] = {11, 7, 12};
    // int w[p] = {4, 3, 5};
    // int cap = 10; 
    
    // int v[p] = {7, 9, 5, 12, 14, 6, 12};
    // int w[p] = {3, 4, 2, 6, 7, 3, 5};
    // int cap = 15; 

    int v[p] = {0, 0, 0, 0};
    int w[p] = {1, 1, 1, 1};
    int cap = 15; 

    Knapsack *K = (Knapsack*)calloc(1,sizeof(Knapsack));
    setup_knapsack(K, 1, p, cap, v, w);


    // ===================
    bounded_knapsack(K); 
    print_knapsack(K);
    //save_knapsack("chirripo3", K);
}
