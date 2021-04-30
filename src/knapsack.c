#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.c"

int **tasks;
int **knapsack;
int **changed;
int **quantity;
int capacity;
int Q = 0;
int parts = 0;

int task_weight(int j, int k){
    return tasks[j][2] * k;
}

int prev(int i, int j){
    return knapsack[i][j - 1];
}

int get_max(int i, int j, int k){ 
    int v_j = tasks[j][1] * k;
    int c_j = task_weight(j, k);
    int new = v_j + knapsack[i-c_j][j-1];
    return prev(i, j) < new ? new : prev(i, j);
}

void bounded_knapsack(){
    for(int i = 0; i < capacity; i++){
        for(int j = 0; j < parts; j++){
            for(int k = 0; k <= Q; k++){
                if(task_weight(j, k) <= i){
                    knapsack[i][j] = get_max(i, j, k);
                    if(knapsack[i][j] != prev(i, j)) changed[i][j] = 1;
                    quantity[i][j] = k;
                } else {
                   // knapsack[i][j] = prev(i, j);
                }  
            }   
        }
    }
}

void unbounded_knapsack(){
    for(int i = 0; i < capacity; i++){
        for(int j = 0; j < parts; j++){

        }
    }
}

// knapsack 1 o 0
void bin_knapsack(){
    for(int i = 0; i < capacity; i++){
        for(int j = 0; j < parts; j++){
            if(task_weight(j, 1) <= i){
                knapsack[i][j] = get_max(i, j, 1);
                if(knapsack[i][j] != prev(i, j)) changed[i][j] = 1;
                quantity[i][j] = 1;
            } else {
                knapsack[i][j] = prev(i, j);
            }         
        }
    }
}

void set_tasks(int p, int *values, int *weight){
    for(int i = 0; i < p; i++){            
        tasks[i][0] = i+1;
        tasks[i][1] = values[i];
        tasks[i][2] = weight[i];
    }
}

void setup_knapsack(int max, int p, int c, int *v, int *w){
    Q = max;
    capacity = c + 1;
    parts = p;
    tasks = init_matrix(p, 3);
    knapsack = init_matrix(capacity, p);
    changed = init_matrix(capacity, p);
    quantity = init_matrix(capacity, p);
    set_tasks(p, v, w);
}

int** get_solution(){
    int **sol = init_matrix(2, parts);
    int i = capacity-1;
    for(int j = parts-1; j >= 0; j--){
        if(changed[i][j]){
            sol[0][j] = j+1;
            sol[1][j] = quantity[i][j];
            i = i - tasks[j][2];
        }
    }
    print_matrix(sol, 2, parts);
    return sol;
}

#define p 7

int main(int argc, char *argv[]){
    int v[p] = {11, 7, 12};
    int w[p] = {4, 3, 5};
    int cap = 10;
    //int v[p] = {7, 9, 5, 12, 14, 6, 12};
    //int w[p] = {3, 4, 2, 6, 7, 3, 5};
    //int cap = 15;
    // Q, # parts, capacity, values, weight
    setup_knapsack(10, p, cap, v, w);
    bounded_knapsack();
    print_matrix(knapsack, capacity, p);
    printf("===== Sol =====\n");
    get_solution();
}