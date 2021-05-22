#include<stdio.h>
#include <stdlib.h>

typedef struct serie_struct {
    double **prob;
    double p_h;
    double p_r;
    int *format;
    int n;
} Series;

void print_series(Series *series){
    for(int i = 0; i < 2*(series->n)-1; i++){
        printf("%d ", series->format[i]);
    }
    printf("\n");
    for(int i = 0; i <= series->n; i++){
        for(int j = 0; j <= series->n; j++){
            printf("%0.4F ", series->prob[i][j]);
        }
        printf("\n");
    }
}

void init_Series(Series *series, double p_h, double p_r, int *format, int n){
    series->p_h = p_h;
    series->p_r = p_r;
    series->n = (n/2)+1;
    series->prob = (double**) calloc(series->n + 1, sizeof(double*));
    for(int i = 0; i <= series->n; i++){
        series->prob[i] = (double*) calloc(series->n + 1, sizeof(double));
        for(int j = 0; j <= series->n; j++){
            if(!i && !j){
                series->prob[i][j] = 0;
            }
            else if(!i){
                series->prob[i][j] = 1;
            }
            else if(!j){
                series->prob[i][j] = 0;
            }
            else{
                series->prob[i][j] = -1;
            }
        }
    }
    series->format = (int*) calloc(2*(series->n)-1, sizeof(int));
    for(int i = 0; i < 2*(series->n)-1; i++){
        series->format[i] = format[i];
    }
}

void solve_series(Series *series){
    for(int i = 1; i <= series->n; i++){
        for(int j = 1; j <= series->n; j++){
            int current_match = 2*(series->n) - i - j;
            double current_prob = 0;
            if(series->format[current_match]){
                double p = (series->p_h)*(series->prob[i-1][j]);
                double q = (1-(series->p_h))*(series->prob[i][j-1]);
                current_prob = p + q;
            }
            else{
                double p = (series->p_r)*(series->prob[i-1][j]);
                double q = (1-(series->p_r))*(series->prob[i][j-1]);
                current_prob = p + q;
            }
            series->prob[i][j] = current_prob;
        }
    }
}

/*
int main (){
    int n = 7;
    double p_h = 0.52;
    double p_r = 0.48;
    int format[7] = {1,1,1,1,0,0,0};

    
    int *format = (int*)calloc(n, sizeof(int));
    for(int i = 0; i < n; i++){
        format[i] = 1;
    }
    

    Series *series = (Series*)calloc(1,sizeof(Series));
    init_Series(series, p_h, p_r, format, n);

    solve_series(series);

    print_series(series);

    return 0;
}
*/