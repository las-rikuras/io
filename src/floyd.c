#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "floyd_util.c"


GtkBuilder *builder;
GtkWidget *grid;
GtkWidget *D_label;
int current_d = 0;
int max_d = 1;
Floyd *F;
Value ***d_0;

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

/* void free_floyd(Floyd *f){
    for(int i = 0; i <= f->n; i++){
        for(int j = 0; j < n; j++){
            attr->D[i][j] = (Value**)calloc(n, sizeof(Value*));
            for(int k = 0; k < n; k++){
                Value *temp = (Value*)calloc(1, sizeof(Value));
                temp->is_infinite = 0;
                temp->value = 0;
                attr->D[i][j][k] = temp;
            }
        } 
        free(f->D[i]);
    }
    free(f->D);
} */

/* Inits the strutc used for the main program */
int init_floyd(Floyd *attr, Value ***d_0, int n){
    attr->n = n;
    attr->D = (Value****)calloc(n+1, sizeof(Value***));
    for(int i = 0; i <= n; i++){
        attr->D[i] = (Value***)calloc(n, sizeof(Value**));
        for(int j = 0; j < n; j++){
            attr->D[i][j] = (Value**)calloc(n, sizeof(Value*));
            for(int k = 0; k < n; k++){
                Value *temp = (Value*)calloc(1, sizeof(Value));
                temp->is_infinite = 0;
                temp->value = 0;
                attr->D[i][j][k] = temp;
            }
        } 
    }
    
    copy_D_table(attr->D[0], d_0, n);

    int ***p = (int***)calloc(n+1, sizeof(int**));
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
void floyd_stepper(Floyd *attr, int k){

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

void load_d(Floyd *f, GtkWidget *grid, int k){
    for(int i = 0; i < f->n; i++){
        for(int j = 0; j < f->n; j++){
            if(i != j){
                GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), j+1, i+1);
                if(f->D[k][i][j]->is_infinite){
                    gtk_entry_set_text(GTK_ENTRY(entry), "");
                }
                else{
                    char str[10];
                    sprintf(str, "%d", f->D[k][i][j]->value);
                    gtk_entry_set_text(GTK_ENTRY(entry), str);
                }
            }
        }
    }
}

void next (GtkButton *button, gpointer user_data){
    if(!current_d){
        GtkWidget *previous = user_data;
        gtk_widget_show_all(previous);
        F = (Floyd*)calloc(1,sizeof(Floyd));
        d_0 = (Value***)calloc(max_d, sizeof(Value*));
        
        for(int i = 0; i < max_d; i++){
            d_0[i] = (Value**)calloc(max_d, sizeof(Value));
            for(int j = 0; j < max_d; j++){
                Value *temp = (Value*)calloc(1, sizeof(Value));
                GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), j+1, i+1);
                temp->is_infinite = 0;
                temp->value = 0;
                if(GTK_IS_ENTRY(entry)){
                    const char *val = gtk_entry_get_text(GTK_ENTRY(entry));
                    if(val[0] == 0){
                        temp->is_infinite = 1;
                    }
                    else{
                        int number = atoi(val);
                        temp->value = number;
                    }
                }
                d_0[i][j] = temp;
            }
        }
        init_floyd(F,d_0, max_d);
    }
    current_d++;
    char str[5];
    sprintf(str, "D(%d)", current_d);
    gtk_label_set_text(GTK_LABEL(D_label), str);

    floyd_stepper(F, current_d);
    load_d(F,grid,current_d);
    //print_D(F, current_d);

    if(current_d == max_d){
        gtk_widget_hide(GTK_WIDGET(button));
    }
}

void previous (GtkButton *button, gpointer user_data){
    if(current_d == max_d){
        GtkWidget *next= user_data;
        gtk_widget_show_all(next);
    }
    current_d--;
    char str[5];
    sprintf(str, "D(%d)", current_d);
    load_d(F, grid, current_d);
    gtk_label_set_text(GTK_LABEL(D_label), str);
    if(!current_d){
        gtk_widget_hide(GTK_WIDGET(button));
        free(F);
        free(d_0);
    }
}


void insert_text_event(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data){
    for (int i = 0; i < length; i++) {
        if (!isdigit(text[i])) {
            g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
            return;
        }
    }
}

void change_label(GtkEditable *editable, gpointer data){
    GtkWidget* grid = data;

    GValue i = G_VALUE_INIT;
    GValue j = G_VALUE_INIT;
    g_value_init (&i, G_TYPE_INT);
    g_value_init (&j, G_TYPE_INT);

    gtk_container_child_get_property(GTK_CONTAINER(grid), GTK_WIDGET(editable), "left-attach", &i);
    gtk_container_child_get_property(GTK_CONTAINER(grid), GTK_WIDGET(editable), "top-attach", &j);

    GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), g_value_get_int(&j), g_value_get_int(&i));

    const char *text = gtk_entry_get_text(GTK_ENTRY(editable));

    gtk_entry_set_text(GTK_ENTRY(entry), text);
}

void insertEntry(GtkWidget *grid, int i, int j, int val){
    GtkWidget *entry;

    if(val){
        entry = gtk_entry_new();
        gtk_entry_set_width_chars (GTK_ENTRY(entry), 3);
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "∞");
        gtk_entry_set_has_frame(GTK_ENTRY(entry), 0);
        gtk_entry_set_alignment (GTK_ENTRY(entry), 0.5);
        g_signal_connect(G_OBJECT(entry), "insert_text", G_CALLBACK(insert_text_event), NULL);
    }
    else if(val == 0){
        entry = gtk_label_new("0");
        gtk_label_set_width_chars(GTK_LABEL(entry), 3);

    }
    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}

void insertLabel(GtkWidget *grid, int i, int j){
    GtkEntry*entry;
    entry = (GtkEntry*)gtk_entry_new();
    char *val = (char*)calloc(1,sizeof(char));
    GtkWidget* previousEntry;

    if(!i){
        previousEntry = gtk_grid_get_child_at(GTK_GRID(grid), i, j-1);
    }
    else{
        previousEntry = gtk_grid_get_child_at(GTK_GRID(grid), i-1, j);
    }

    const char *previousVal = gtk_entry_get_placeholder_text(GTK_ENTRY(previousEntry));

    val[0] = previousVal[0]+1;

    gtk_entry_set_width_chars (entry, 3);

    gtk_entry_set_alignment (entry, 0.5);
    
    g_signal_connect_after (entry, "changed", G_CALLBACK (change_label), grid);

    gtk_entry_set_placeholder_text(entry, val);
    gtk_widget_show (GTK_WIDGET(entry));

    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}

void spin_clicked (GtkSpinButton *spinbutton, gpointer data){

    struct spin_data *spin_data = data;
    gint value = gtk_spin_button_get_value_as_int (spinbutton);

    if(spin_data->last_value < (int)value){

        int difference = (int)value - spin_data->last_value;
        for(int i = (spin_data->last_value) + 1; i <= (int)value; i++){
            insertLabel(spin_data->grid, i, 0);
            insertLabel(spin_data->grid, 0, i);
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
    max_d = value;
}

void on_window_destroy(){
    gtk_main_quit();
}

void on_exit_clicked(){
    gtk_main_quit();
}

int main(int argc, char *argv[]){
    GtkWidget *window;
    GtkWidget *nodes_number;
    GtkWidget *next_button;
    GtkWidget *previous_button;
    GtkWidget *A1;
    GtkWidget *A2;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/floyd.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    nodes_number = GTK_WIDGET(gtk_builder_get_object(builder, "nodes_number"));
    next_button = GTK_WIDGET(gtk_builder_get_object(builder, "next"));
    previous_button = GTK_WIDGET(gtk_builder_get_object(builder, "previous"));
    grid = GTK_WIDGET(gtk_builder_get_object(builder, "grid"));
    A1 = GTK_WIDGET(gtk_builder_get_object(builder, "A1"));
    A2 = GTK_WIDGET(gtk_builder_get_object(builder, "A2"));
    D_label = GTK_WIDGET(gtk_builder_get_object(builder, "D_label"));
    
    struct spin_data spin_data;
    spin_data.grid = grid;
    spin_data.last_value = 1;


    g_signal_connect(nodes_number, "value-changed", G_CALLBACK (spin_clicked), &spin_data);
    g_signal_connect(next_button, "clicked", G_CALLBACK (next), previous_button);
    g_signal_connect(previous_button, "clicked", G_CALLBACK (previous), next_button);
    
    g_signal_connect_after (A1, "changed", G_CALLBACK (change_label), grid);
    g_signal_connect_after (A2, "changed", G_CALLBACK (change_label), grid);

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);
    gtk_main();

    
    g_object_unref(builder);

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

    //Floyd *E = load_floyd("floyd");
    //floyd(E);
    //Floyd *E = (Floyd*)calloc(1,sizeof(Floyd));
    //save_floyd("floyd", E);
    //floyd(E); 

    return 0;
}