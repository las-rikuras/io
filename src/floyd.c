#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "floyd_util.c"

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


/* Gtk code */

struct spin_data{
    GtkWidget *grid;
    int last_value;
};

void insertEntry(GtkWidget *grid, int i, int j, int val){
    GtkEntry*entry;
    entry = (GtkEntry*)gtk_entry_new();

    gtk_entry_set_width_chars (entry, 2);

    if(val == -1){
        gtk_entry_set_placeholder_text(entry, "∞");
    }
    else if(val == 0){
        gtk_editable_set_editable(GTK_EDITABLE(entry), 0);
        gtk_entry_set_text (entry, "0");
    }

    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}

void insertLabel(GtkWidget *grid, int i, int j, char *val){
    GtkEntry*entry;
    entry = (GtkEntry*)gtk_entry_new();

    gtk_entry_set_width_chars (entry, 2);

    gtk_entry_set_placeholder_text(entry, val);
    gtk_entry_set_text (entry, val);

    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}

void spin_clicked (GtkSpinButton *spinbutton, gpointer data){

    struct spin_data *spin_data = data;
    gint value = gtk_spin_button_get_value_as_int (spinbutton);

    if(spin_data->last_value < (int)value){

        int difference = (int)value - spin_data->last_value;
        for(int i = (spin_data->last_value) + 1; i <= (int)value; i++){
            insertLabel(spin_data->grid, i, 0, "A");
            insertLabel(spin_data->grid, 0, i, "A");
            for(int j = 1; j < i; j++){
                insertEntry(spin_data->grid, i, j, -1);
                insertEntry(spin_data->grid, j, i, -1);
            }
            insertEntry(spin_data->grid, i, i, 0);
        }
    }
    else{
        for(int i = spin_data->last_value; i > value; i--){
            gtk_grid_remove_column (GTK_GRID(spin_data->grid), i);
            gtk_grid_remove_row (GTK_GRID(spin_data->grid), i);
        }
    }
    spin_data->last_value = (int)value;
}

void on_window_destroy(){
    gtk_main_quit();
}

void on_exit_clicked(){
    gtk_main_quit();
}

int main(int argc, char *argv[]){
   /* GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *nodes_number;
    GtkWidget *next_button;
    GtkWidget *previous_button;
    GtkWidget *grid;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/floyd.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    nodes_number = GTK_WIDGET(gtk_builder_get_object(builder, "nodes_number"));
    next_button = GTK_WIDGET(gtk_builder_get_object(builder, "next"));
    previous_button = GTK_WIDGET(gtk_builder_get_object(builder, "previous"));
    grid = GTK_WIDGET(gtk_builder_get_object(builder, "grid"));
    struct spin_data spin_data;
    spin_data.grid = grid;
    spin_data.last_value = 1;

    g_signal_connect(nodes_number, "value-changed", G_CALLBACK (spin_clicked), &spin_data);

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main(); */

    /*
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

    init_floyd(E, d_0, n); */

    Floyd *E = load_floyd("floyd");
    floyd(E);
    //Floyd *E = (Floyd*)calloc(1,sizeof(Floyd));
    //save_floyd("floyd", E);
    //floyd(E); 

    return 0;
}