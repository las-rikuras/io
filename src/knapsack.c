#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.c"

int **tasks;
int **knapsack;
int **quantity;
int capacity;
int Q = 0;
int parts = 0;

int task_weight(int j, int k){
    return tasks[j][1] * k;
}

int prev(int i, int j){
    return knapsack[i][j - 1];
}

int get_max(int i, int j, int k){ 
    int v_j = tasks[j][0] * k;
    int c_j = task_weight(j, k);
    int new = v_j + knapsack[i-c_j][j-1];
    return prev(i, j) < new ? new : prev(i, j);
}

// bounded y 1 o 0 
void bounded_knapsack(){
    for(int i = 0; i < capacity; i++){
        for(int j = 0; j < parts; j++){
            int q = i/tasks[j][1] > Q ? Q : i/tasks[j][1];
            for(int k = 0; k <= q; k++){
                if(task_weight(j, k) <= i){
                    int t1 = get_max(i, j, k);
                    int t2 = knapsack[i][j];
                    if(t1 > t2){
                        knapsack[i][j] = t1;
                        quantity[i][j] = k;
                    } 
                } 
            }   
        }
    }
}
/*
void unbounded_knapsack(){ 
    for(int i = 0; i < capacity; i++){
        for(int j = 0; j < parts; j++){
            int k = 0;
            while(task_weight(j, k) <= i){
                int t1 = get_max(i, j, k);
                int t2 = knapsack[i][j];
                if(t1 > t2){
                    knapsack[i][j] = t1;
                    quantity[i][j] = k;
                }
                k++; 
            }
        }
    }
}
*/
void set_tasks(int p, int *values, int *weight){
    for(int i = 0; i < p; i++){   
        tasks[i][0] = values[i];
        tasks[i][1] = weight[i];
    }
}

void setup_knapsack(int max, int p, int c, int *v, int *w){
    Q = max;
    capacity = c + 1;
    parts = p;
    tasks = init_matrix(p, 2);
    knapsack = init_matrix(capacity, p);
    quantity = init_matrix(capacity, p);
    set_tasks(p, v, w);
}

int** get_solution(){
    int **sol = init_matrix(2, parts);
    int i = capacity-1;
    for(int j = parts-1; j >= 0; j--){
        sol[0][j] = j+1;
        if(quantity[i][j] != 0){        
            sol[1][j] = quantity[i][j];
            i = i - task_weight(j, quantity[i][j]);
        } 
    }
    return sol;
}

//#define p 3
#define p 7

int main(int argc, char *argv[]){
    /*
    int v[p] = {11, 7, 12};
    int w[p] = {4, 3, 5};
    int cap = 10; 
    */
    int v[p] = {7, 9, 5, 12, 14, 6, 12};
    int w[p] = {3, 4, 2, 6, 7, 3, 5};
    int cap = 15; 
    
    // Q, # parts, capacity, values, weight
    setup_knapsack(cap, p, cap, v, w);
    bounded_knapsack();
    print_matrix(knapsack, capacity, p);
    printf("===== Sol =====\n");
    int **sol = get_solution();
    print_matrix(sol, 2, parts);
}