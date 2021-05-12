#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct value_struct {
    int is_infinite;
    int value;
} Value;

typedef struct floyd_struct {
    Value ****D;
    int ***P;
    char **L;
    int n;
} Floyd;

typedef struct position {
    double x;
    double y;
} Position;

int save_floyd(char *file_name, Floyd *F){
    char *fn = malloc((strlen(file_name) + 5));
    strcpy(fn, file_name);
    strcat(fn, ".fd");
    FILE *fp = fopen(fn, "w");

    if (fp == NULL) {
        printf("Error saving file!\n");
        return -1;
    }
    fprintf(fp, "%d;\n", F->n);
    for(int i = 0; i < F->n; i++){
        for(int j = 0; j < F->n; j++){
            if(F->D[0][i][j]->is_infinite)
                fprintf(fp, "-1;");
            else
                fprintf(fp, "%d;", F->D[0][i][j]->value);
        }
        fprintf(fp, "\n");
    }

    for(int i = 0; i < F->n; i++){
        fprintf(fp, "%s ", F->L[i]);
    }

    fclose(fp);
    return 0;
}

int init_floyd(Floyd *attr, Value ***d_0, int n);

Floyd* load_floyd(char *file_name){
    char *fn = malloc((strlen(file_name) + 5));
    strcpy(fn, file_name);
    strcat(fn, ".fd");
    FILE *fp = fopen(fn, "r");
    if (fp == NULL){
        printf("File doesn't exits\n");
        exit(1);
    }
    int res;
    int n;
    
    res = fscanf(fp, "%d;\n", &n);
    if(res < 1){
        printf("Incorrect file format\n");
        exit(1);
    }

    Value ***d_0 = (Value***)calloc(n, sizeof(Value*));
    for(int i = 0; i < n; i++){
        d_0[i] = (Value**)calloc(n, sizeof(Value));
        for(int j = 0; j < n; j++){
            int val = 0;
            res = fscanf(fp, "%d;\n", &val);
            if(res < 1){
                printf("Incorrect file format\n");
                exit(1);
            }
            Value *temp = (Value*)calloc(1, sizeof(Value));
            if(val >= 0){
                temp->is_infinite = 0;
                temp->value = val;
            } else {
                temp->is_infinite = 1;
                temp->value = 0;
            } 
            d_0[i][j] = temp;       
        }
    }

    Floyd *E = (Floyd*)calloc(1,sizeof(Floyd));
    E->n = n;
    E->L = malloc(n * sizeof(char*));
    
    char s[30];
    for(int i = 0; i < n; i++){
        E->L[i] = malloc((sizeof(s) + 1) * sizeof(char));
        res = fscanf(fp, "%s", s);
        strcpy(E->L[i], s);
    }

    init_floyd(E, d_0, n);

    free(d_0);

    fclose(fp);
    return E;
}