#include <stdlib.h>
#include <stdio.h>

typedef struct value_struct {
    int is_infinite;
    int value;
} Value;

typedef struct floyd_struct {
    Value ****D;
    int ***P;
    int n;
} Floyd;

/* Prints D(x) table */
void print_D(Floyd *attr, int x){
    int i, j;
    printf("D(%d):\n", x);
    for(i = 0; i < attr->n; i++){
        printf("\t");
        for(j = 0; j < attr->n; j++){
            if(attr->D[x][i][j]->is_infinite)
                printf("i ");
            else
                printf("%d ", attr->D[x][i][j]->value);
        }
        printf("\n");
    }
}

/* Prints P Corresponding to D(x) table */
void print_P(Floyd *attr, int x){
    int i, j;
    printf("P(%d):\n", x);
    for(i = 0; i < attr->n; i++){
        printf("\t");
        for(j = 0; j < attr->n; j++){
            printf("%d ", attr->P[x][i][j]);
        }
        printf("\n");
    }
}

void print_floyd(Floyd *attr){
    for(int i = 0; i < attr->n+1; i++){
        print_D(attr, i);
        print_P(attr, i);
    }
}

void copy_D_table(Value ***d_x, Value ***d_y, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            d_x[i][j]->is_infinite = d_y[i][j]->is_infinite;
            d_x[i][j]->value = d_y[i][j]->value;
        }
    }
}

void copy_P_table(int **p_x, int **p_y, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            p_x[i][j] = p_y[i][j];
        }
    }
}

/* Inits the strutc used for the main program */
int init_floyd(Floyd *attr, Value ***d_0, int n){
    attr->n = n;
    attr->D = (Value****)calloc(n, sizeof(Value**));
    for(int i = 0; i <= n; i++){
        attr->D[i] = (Value***)calloc(n, sizeof(Value*));
        for(int j = 0; j < n; j++){
            attr->D[i][j] = (Value**)calloc(n, sizeof(Value));
            for(int k = 0; k < n; k++){
                Value *temp = (Value*)calloc(1, sizeof(Value));
                temp->is_infinite = 0;
                temp->value = 0;
                attr->D[i][j][k] = temp;
            }
        }
    }
    copy_D_table(attr->D[0], d_0, n);

    int ***p = (int***)calloc(n, sizeof(int**));
    for(int i = 0; i <= n; i++){
        p[i] = (int**)calloc(n, sizeof(int*));
        for(int j = 0; j < n; j++){
            p[i][j] = (int*)calloc(n, sizeof(int));
            for(int k = 0; k < n; k++){
                p[i][j][k] = 0;
            }
        }
    }
    attr->P = p;
}

/**/
void floyd(Floyd *attr){
    for(int k = 1; k <= attr->n; k++){

        copy_D_table(attr->D[k], attr->D[k-1], attr->n); 
        copy_P_table(attr->P[k], attr->P[k-1], attr->n); 

        for(int i = 0; i < attr->n; i++){
            for(int j = 0; j < attr->n; j++){
                if( i != j && i != (k-1) && j != (k-1) ){
                    if( !(attr->D[k-1][k-1][j]->is_infinite) && !(attr->D[k-1][i][k-1]->is_infinite)){
                        if( !(attr->D[k-1][i][j]->is_infinite) ){
                            if( ((attr->D[k-1][k-1][j]->value + attr->D[k-1][i][k-1]->value) < attr->D[k-1][i][j]->value) ){
                                attr->D[k][i][j]->value = attr->D[k-1][k-1][j]->value + attr->D[k-1][i][k-1]->value;
                                attr->D[k][i][j]->is_infinite = 0;
                                attr->P[k][i][j] = k;
                            }
                        }
                        else{
                            attr->D[k][i][j]->value = attr->D[k-1][k-1][j]->value + attr->D[k-1][i][k-1]->value;
                            attr->D[k][i][j]->is_infinite = 0;
                            attr->P[k][i][j] = k;
                        }
                    }
                }
            }
        }
    }
    print_floyd(attr);
}


int main(){
    int n = 3;
    
    Value ***d_0 = (Value***)calloc(n, sizeof(Value*));
    for(int i = 0; i < n; i++){
        d_0[i] = (Value**)calloc(n, sizeof(Value));
        for(int j = 0; j < n; j++){
            Value *temp = (Value*)calloc(1, sizeof(Value));
            temp->is_infinite = 0;
            temp->value = 0;
            d_0[i][j] = temp;
        }
    }

    d_0[0][1]->value = 2;
    d_0[0][2]->is_infinite = 1;
    d_0[1][0]->is_infinite = 1;
    d_0[1][2]->value = 3;
    d_0[2][0]->value = 4;
    d_0[2][1]->is_infinite = 1;

    Floyd *E = (Floyd*)calloc(1,sizeof(Floyd));

    init_floyd(E, d_0, n);

    //print_floyd(E);

    floyd(E);
    return 0;
}