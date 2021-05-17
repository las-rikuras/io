#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <math.h>
#include "floyd_util.c"

GtkBuilder *builder;
GtkWidget *grid;
GtkWidget *D_label;
GtkWidget *drawing_area;
int current_d = 0;
int max_d = 1;
Floyd *F;
Value ***d_0;
double scale = 1;
GtkWidget *p_grid;
Position **positions;
GtkWidget *dialog;
GtkWidget *start;
GtkWidget *final;
GtkWidget *route_lable;
GtkWidget *nodes_number;
int pauseDraw;

void route (GtkButton*widget, gpointer user_data);

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

void reset_css(GtkWidget *grid){
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_path (provider, "css/entry_reset.css", NULL);
    for(int i = 0; i < max_d; i++){
        for(int j = 0; j < max_d; j++){
            GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), j+1, i+1);
            GtkStyleContext * context = gtk_widget_get_style_context(entry);
            gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        }
    }                     
}

void load_d(Floyd *f, GtkWidget *grid, int k){
    reset_css(grid);
    for(int i = 0; i < f->n; i++){
        for(int j = 0; j < f->n; j++){
            if(i != j){
                GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), j+1, i+1);
                if(f->D[k][i][j]->is_infinite){
                    gtk_entry_set_text(GTK_ENTRY(entry), "");
                }
                else{
                    char str[10];
                    if(k!=0){
                        if(f->D[k][i][j]->value != f->D[k-1][i][j]->value){
                            GtkCssProvider *provider = gtk_css_provider_new ();
                            GtkStyleContext * context = gtk_widget_get_style_context(entry);
                            gtk_css_provider_load_from_path (provider, "css/entry_change.css", NULL);
                            gtk_style_context_add_provider (context,
                                                                GTK_STYLE_PROVIDER(provider),
                                                                GTK_STYLE_PROVIDER_PRIORITY_USER);

                        }
                    }

                    sprintf(str, "%d", f->D[k][i][j]->value);
                    gtk_entry_set_text(GTK_ENTRY(entry), str);
                }
            }
        }
    }
}

void load_p(Floyd *f, GtkWidget *grid, int k){
    reset_css(p_grid);
    for(int i = 0; i < f->n; i++){
        for(int j = 0; j < f->n; j++){
            if(i != j){
                GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(p_grid), j+1, i+1);
                GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), f->P[k][i][j], 0);
                char const *text = gtk_entry_get_text(GTK_ENTRY(entry));
                if(text[0] == 0){
                    char const *place = gtk_entry_get_placeholder_text(GTK_ENTRY(entry));
                    gtk_label_set_text(GTK_LABEL(label), place);
                    if(k!=0){
                        if(f->D[k][i][j]->value != f->D[k-1][i][j]->value){
                            GtkCssProvider *provider = gtk_css_provider_new ();
                            GtkStyleContext * context = gtk_widget_get_style_context(label);
                            gtk_css_provider_load_from_path (provider, "css/entry_change.css", NULL);
                            gtk_style_context_add_provider (context,
                                                                GTK_STYLE_PROVIDER(provider),
                                                                GTK_STYLE_PROVIDER_PRIORITY_USER);

                        }
                    }
                }
                else{
                    gtk_label_set_text(GTK_LABEL(label), text);
                }
            }
        }
    }
}

void next(GtkButton *button, gpointer user_data){
    if(!current_d){
        gtk_widget_set_sensitive(nodes_number, 0);
        GtkWidget *previous = user_data;
        pauseDraw = 1;
        gtk_widget_set_sensitive(previous, 1);
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
        for(int i = 0; i <= max_d; i++){
            for(int j = 0; j <= max_d; j++){
                if(i!=j){
                    GtkWidget *w = gtk_grid_get_child_at(GTK_GRID(grid), j, i);
                    gtk_widget_set_sensitive(w, 0);
                }
            }
        }
    }
    current_d++;
    char str[5];
    sprintf(str, "D(%d)", current_d);
    gtk_label_set_text(GTK_LABEL(D_label), str);

    floyd_stepper(F, current_d);
    load_d(F,grid,current_d);
    load_p(F,grid,current_d);
    //print_D(F, current_d);

    if(current_d == max_d){
        gtk_widget_set_sensitive(GTK_WIDGET(button), 0);
        for(int i = 1; i <= max_d; i++){
            GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(p_grid), 0, i);
            const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
            char id = i; 
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(start), &id, text);
            gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(final), &id, text);
            gtk_combo_box_set_active(GTK_COMBO_BOX(start), 0);
            gtk_combo_box_set_active(GTK_COMBO_BOX(final), 0);
        }
        gtk_dialog_run(GTK_DIALOG(dialog));
    }
}

void previous (GtkButton *button, gpointer user_data){
    reset_css(grid);
    reset_css(p_grid);
    if(current_d == max_d){
        GtkWidget *next= user_data;
        gtk_widget_set_sensitive(next, 1);
    }
    current_d--;
    char str[5];
    sprintf(str, "D(%d)", current_d);
    load_d(F, grid, current_d);
    load_p(F, grid, current_d);
    gtk_label_set_text(GTK_LABEL(D_label), str);
    if(!current_d){
        for(int i = 0; i <= max_d; i++){
            for(int j = 0; j <= max_d; j++){
                if(i!=j){
                    GtkWidget *w = gtk_grid_get_child_at(GTK_GRID(grid), j, i);
                    gtk_widget_set_sensitive(w, 1);
                }
            }
        }
        pauseDraw = 0;
        gtk_widget_set_sensitive(nodes_number, 1);
        gtk_widget_set_sensitive(GTK_WIDGET(button), 0);
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
    GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(p_grid), g_value_get_int(&j), g_value_get_int(&i));
    const char *text = gtk_entry_get_text(GTK_ENTRY(editable));
    gtk_entry_set_text(GTK_ENTRY(entry), text);
    gtk_entry_set_text(GTK_ENTRY(label), text);
    gtk_widget_queue_draw(drawing_area);
}

void entry_changed(GtkEditable *editable, gpointer data){
    gtk_widget_queue_draw(drawing_area);
}

void insertEntry(GtkWidget *grid, int i, int j, int val){
    GtkWidget *entry;
    GtkWidget *p;

    if(val){
        entry = gtk_entry_new();
        gtk_entry_set_width_chars (GTK_ENTRY(entry), 3);
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "∞");
        gtk_entry_set_has_frame(GTK_ENTRY(entry), 0);
        gtk_entry_set_alignment (GTK_ENTRY(entry), 0.5);
        g_signal_connect(G_OBJECT(entry), "insert_text", G_CALLBACK(insert_text_event), NULL);
        g_signal_connect(G_OBJECT(entry), "changed", G_CALLBACK(entry_changed), NULL);
    }
    else if(val == 0){
        entry = gtk_label_new("0");
        gtk_label_set_width_chars(GTK_LABEL(entry), 3);

    }
    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);

    p = gtk_label_new("0");
    gtk_label_set_width_chars(GTK_LABEL(p), 3);
    gtk_widget_show (p);
    gtk_grid_attach(GTK_GRID(p_grid), p, i, j, 1 ,1);
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

    GtkWidget *p;
    p = gtk_entry_new();

    gtk_widget_set_sensitive(p, 0);
    gtk_entry_set_text(GTK_ENTRY(p), val);
    gtk_entry_set_placeholder_text(GTK_ENTRY(p), val);
    gtk_entry_set_width_chars (GTK_ENTRY(p), 3);
    gtk_entry_set_alignment (GTK_ENTRY(p), 0.5);
    gtk_widget_show (p);
    gtk_grid_attach(GTK_GRID(p_grid), p, i, j, 1 ,1);
}

void on_drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer data){
    if(!pauseDraw){
        GtkWidget* spinner = data;
        int spinner_num = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinner));
        guint width, height;
        GdkRGBA color;
        GtkStyleContext *context;

        context = gtk_widget_get_style_context (widget);
        width = 543;
        height = 543;
        
        double x, y;
        double ang = 0;
        double radius = 50*spinner_num;
        double node_r = 20;

        if(((2*radius + 2*node_r)*scale) > width){
            int new_width = ((2*radius + 2*node_r)+20)*scale;
            width = height = new_width;
            gtk_widget_set_size_request(widget, new_width, new_width);
        }
        else{
            gtk_widget_set_size_request(widget, 543, 543);
        }

        gtk_render_background(context, cr, 0, 0, width-1, width-1);
        
        x = y = width/2;
        positions = (Position**)calloc(spinner_num, sizeof(Position*));
        if(spinner_num == 1){
            Position *pos = (Position*)calloc(1, sizeof(Position*));
            pos->x = width/2;
            pos->y = width/2;
            cairo_arc (cr, width/2, height/2, node_r*scale, 0, 2 * G_PI);
            positions[0] = pos;
        }
        else{
            for(int i = 0; i < spinner_num; i++){
                Position *pos = (Position*)calloc(1, sizeof(Position*));
                y = (radius * sin(ang))*scale + width/2;
                x = (radius * cos(ang))*scale + height/2;
                pos->y = y;
                pos->x = x;
                positions[i] = pos;
                ang += 2*G_PI/spinner_num;
            } 
            
            cairo_set_line_width(cr, 2*scale);
            for(int i = 1; i <= max_d; i++){
                for(int j = 1; j <= max_d; j++){
                    if(i != j){
                        GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), j, i);
                        const char *val = gtk_entry_get_text(GTK_ENTRY(entry));
                            
                        if(val[0] != 0){
                            double fas = node_r*scale/2;
                            if(i < j){
                                cairo_move_to(cr, positions[i-1]->y-fas, positions[i-1]->x-fas);
                                cairo_line_to(cr, positions[j-1]->y-fas, positions[j-1]->x-fas);
                            }
                            else{
                                cairo_move_to(cr, positions[i-1]->y+fas, positions[i-1]->x+fas);
                                cairo_line_to(cr, positions[j-1]->y+fas, positions[j-1]->x+fas);
                            }
                            cairo_close_path(cr);
                            cairo_stroke(cr);
                            cairo_save(cr);
                            cairo_restore(cr);
                        }
                    }
                }
            }
            ang = 0;
            for(int i = 0; i < spinner_num; i++){
                y = (radius * sin(ang))*scale + width/2;
                x = (radius * cos(ang))*scale + height/2;

                cairo_save(cr);
                cairo_arc(cr, y, x, node_r*scale, 0, 2*G_PI);
                cairo_set_source_rgba(cr, 1, 1, 1, 1); 
                cairo_fill_preserve(cr);
                cairo_restore(cr); 

                cairo_save(cr);
                GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), 0, i+1);
                const char *label = gtk_entry_get_text(GTK_ENTRY(entry));
                cairo_move_to(cr, y-node_r*scale/4, x+node_r*scale/4);
                cairo_set_font_size (cr, 20*scale);
                cairo_set_source_rgb (cr, 1, 0.27, 0);
                if(label[0] != 0){
                    cairo_show_text(cr, label);
                }
                else{
                    const char * placeholder = gtk_entry_get_placeholder_text(GTK_ENTRY(entry));
                    cairo_show_text(cr, placeholder);
                }
                ang += 2*G_PI/spinner_num;
                cairo_restore(cr);
                cairo_stroke(cr);
            } 
        }

        gtk_style_context_get_color (context, gtk_style_context_get_state (context), &color);
        gdk_cairo_set_source_rgba (cr, &color);
        gdk_cairo_set_source_rgba (cr, &color);
        cairo_fill (cr);
    }
    else{
        GtkWidget* spinner = data;
        int spinner_num = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinner));
        guint width, height;
        GdkRGBA color;
        GtkStyleContext *context;

        context = gtk_widget_get_style_context (widget);
        width = 543;
        height = 543;
        
        double x, y;
        double ang = 0;
        double radius = 50*spinner_num;
        double node_r = 20;

        if(((2*radius + 2*node_r)*scale) > width){
            int new_width = ((2*radius + 2*node_r)+20)*scale;
            width = height = new_width;
            gtk_widget_set_size_request(widget, new_width, new_width);
        }
        else{
            gtk_widget_set_size_request(widget, 543, 543);
        }

        gtk_render_background(context, cr, 0, 0, width-1, width-1);
        
        x = y = width/2;
        positions = (Position**)calloc(spinner_num, sizeof(Position*));
        if(spinner_num == 1){
            Position *pos = (Position*)calloc(1, sizeof(Position*));
            pos->x = width/2;
            pos->y = width/2;
            cairo_arc (cr, width/2, height/2, node_r*scale, 0, 2 * G_PI);
            positions[0] = pos;
        }
        else{
            for(int i = 0; i < spinner_num; i++){
                Position *pos = (Position*)calloc(1, sizeof(Position*));
                y = (radius * sin(ang))*scale + width/2;
                x = (radius * cos(ang))*scale + height/2;
                pos->y = y;
                pos->x = x;
                positions[i] = pos;
                ang += 2*G_PI/spinner_num;
            } 
            
            cairo_set_line_width(cr, 2*scale);
            for(int i = 1; i <= max_d; i++){
                for(int j = 1; j <= max_d; j++){
                    if(i != j){                           
                        if(!(F->D[0][i-1][j-1]->is_infinite)){
                            double fas = node_r*scale/2;
                            if(i < j){
                                cairo_move_to(cr, positions[i-1]->y-fas, positions[i-1]->x-fas);
                                cairo_line_to(cr, positions[j-1]->y-fas, positions[j-1]->x-fas);
                            }
                            else{
                                cairo_move_to(cr, positions[i-1]->y+fas, positions[i-1]->x+fas);
                                cairo_line_to(cr, positions[j-1]->y+fas, positions[j-1]->x+fas);
                            }
                            cairo_close_path(cr);
                            cairo_stroke(cr);
                            cairo_save(cr);
                            cairo_restore(cr);
                        }
                    }
                }
            }
            ang = 0;
            for(int i = 0; i < spinner_num; i++){
                y = (radius * sin(ang))*scale + width/2;
                x = (radius * cos(ang))*scale + height/2;

                cairo_save(cr);
                cairo_arc(cr, y, x, node_r*scale, 0, 2*G_PI);
                cairo_set_source_rgba(cr, 1, 1, 1, 1); 
                cairo_fill_preserve(cr);
                cairo_restore(cr); 

                cairo_save(cr);
                GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), 0, i+1);
                const char *label = gtk_entry_get_text(GTK_ENTRY(entry));
                cairo_move_to(cr, y-node_r*scale/4, x+node_r*scale/4);
                cairo_set_font_size (cr, 20*scale);
                cairo_set_source_rgb (cr, 1, 0.27, 0);
                if(label[0] != 0){
                    cairo_show_text(cr, label);
                }
                else{
                    const char * placeholder = gtk_entry_get_placeholder_text(GTK_ENTRY(entry));
                    cairo_show_text(cr, placeholder);
                }
                ang += 2*G_PI/spinner_num;
                cairo_restore(cr);
                cairo_stroke(cr);
            } 
        }

        gtk_style_context_get_color (context, gtk_style_context_get_state (context), &color);
        gdk_cairo_set_source_rgba (cr, &color);
        gdk_cairo_set_source_rgba (cr, &color);
        cairo_fill (cr);
    }
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
            gtk_grid_remove_column (GTK_GRID(p_grid), i);
            gtk_grid_remove_row (GTK_GRID(p_grid), i);
        }
    }
    spin_data->last_value = (int)value;
    max_d = value;
    int width = gtk_widget_get_allocated_width(drawing_area);
    gtk_widget_queue_draw(drawing_area);
}

void zoomIn(GtkButton *button, gpointer user_data){
    scale += 0.1; 
    gtk_widget_queue_draw(drawing_area);
}

void zoomOut(GtkButton *button, gpointer user_data){
    if(scale > 0.2){
        scale -= 0.1; 
        gtk_widget_queue_draw(drawing_area);
    }
}

void on_window_destroy(){
    gtk_main_quit();
}

void on_exit_clicked(){
    gtk_main_quit();
}

void close_dialog(GtkButton *button, gpointer user_data){
    gtk_dialog_response(GTK_DIALOG(dialog), 0);
}

void on_response (GtkDialog *di, gint esponse_id,gpointer user_data){
    gtk_widget_hide(GTK_WIDGET(di));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(start));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(final));
}

void route (GtkButton *button, gpointer user_data){
    const char *first = gtk_combo_box_get_active_id(GTK_COMBO_BOX(start));
    int id1 = first[0];
    const char *end = gtk_combo_box_get_active_id(GTK_COMBO_BOX(final));
    int id2 = end[0];
    char buf[5000];

    if(id1 == id2){
        GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(p_grid), 0, id1);
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        strcat(buf, text);
    }
    else{
        while(1){
            GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(p_grid), 0, id1);
            const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
            if(!F->P[max_d][id1-1][id2-1]){
                entry = gtk_grid_get_child_at(GTK_GRID(p_grid), 0, id2);
                const char *last = gtk_entry_get_text(GTK_ENTRY(entry));
                strcat(buf, text);
                strcat(buf, "->");
                strcat(buf, last);
                gtk_label_set_text(GTK_LABEL(route_lable), buf);
                break;
            }
            strcat(buf, text);
            strcat(buf, "->");
            id1 = F->P[max_d][id1-1][id2-1];
        }
    }
}

int main(int argc, char *argv[]){
    GtkWidget *window;
    GtkWidget *next_button;
    GtkWidget *previous_button;
    GtkWidget *A1;
    GtkWidget *A2;
    GtkWidget *zoomin;
    GtkWidget *zoomout;
    GtkWidget *close_di;

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
    drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    zoomin = GTK_WIDGET(gtk_builder_get_object(builder, "zoomin"));
    zoomout = GTK_WIDGET(gtk_builder_get_object(builder, "zoomout"));
    p_grid = GTK_WIDGET(gtk_builder_get_object(builder, "p_grid"));
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog"));
    close_di = GTK_WIDGET(gtk_builder_get_object(builder, "close_dialog"));
    start = GTK_WIDGET(gtk_builder_get_object(builder, "start"));
    final = GTK_WIDGET(gtk_builder_get_object(builder, "end"));
    route_lable = GTK_WIDGET(gtk_builder_get_object(builder, "route"));

    struct spin_data spin_data;
    spin_data.grid = grid;
    spin_data.last_value = 1;
    pauseDraw = 0;

    g_signal_connect(nodes_number, "value-changed", G_CALLBACK (spin_clicked), &spin_data);
    g_signal_connect(next_button, "clicked", G_CALLBACK (next), previous_button);
    g_signal_connect(previous_button, "clicked", G_CALLBACK (previous), next_button);
    g_signal_connect(drawing_area, "draw", G_CALLBACK (on_drawing_area_draw), nodes_number);
    g_signal_connect(zoomin, "clicked", G_CALLBACK (zoomIn), drawing_area);
    g_signal_connect(zoomout, "clicked", G_CALLBACK (zoomOut), drawing_area);
    g_signal_connect(close_di, "clicked", G_CALLBACK (close_dialog), NULL);
    
    g_signal_connect_after (A1, "changed", G_CALLBACK (change_label), grid);
    g_signal_connect_after (A2, "changed", G_CALLBACK (change_label), grid);

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);
    gtk_main();

    
    g_object_unref(builder);

    return 0;
}