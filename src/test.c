#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <cairo.h>

GtkBuilder *builder;
GtkWidget *drawing_area;
cairo_t *cr;
GtkWidget *spinner;
guint spinner_num = 1;
double **position;

double** matrix(int n){
    double **matrix = calloc(n, sizeof(double*));
    for(int i = 0; i < n; i++){
        matrix[i] = calloc(2, sizeof(double));
    }
    return matrix;
}

void free_matrix(int n){
    for(int i = 0; i < n; i++){
        free(position[i]);
    }
    free(position);
}

int main(int argc, char *argv[]){  
    // gtk
    GtkWidget *window;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/test.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    spinner = GTK_WIDGET(gtk_builder_get_object(builder, "spinner"));
    gtk_builder_connect_signals(builder, NULL);
   // g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_nodes), NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void on_spinner_changed(){ 
    int old = spinner_num;
    spinner_num = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinner));
    if(position != NULL){
        free_matrix(old);
    }       
    position = matrix(spinner_num);
    gtk_widget_queue_draw(drawing_area);
}

void on_drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer data){
    guint width, height;
    GdkRGBA color;
    GtkStyleContext *context;
    int offset = 50;
    double x, y = 0;
    float size = 0;

    context = gtk_widget_get_style_context (widget);
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    gtk_render_background(context, cr, 0, 0, width, height);

    cairo_select_font_face(cr, "arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 10);
/*
    for(int i = 1; i <= spinner_num; i++){  
        size = MIN (width, height) / (3.0 * (spinner_num / 1.0));
        x = i * 50.0;
        y = 40;
        cairo_arc (cr, x, y, size, 0, 2 * G_PI);
        cairo_move_to(cr, x - size/2, y + size + offset);
        cairo_show_text(cr, "Label");
    } */

    int j = 0;
    size =  MIN (width, height) / 25.0;
    y = size * 2;
    x = size * 2;
    for(int i = 0; i < spinner_num; i++){
        if(j == 6){
            y += size * 2 + offset;
            x = size * 2;
            j = 0;
        }
        position[0][i] = x;
        position[1][i] = y;
        cairo_arc (cr, x, y, size, 0, 2 * G_PI);
        cairo_move_to(cr, x - size/2, y + size + 10);
        cairo_show_text(cr, "Label");
        x += size * 2 + offset;
        j++;
    }

    gtk_style_context_get_color (context, gtk_style_context_get_state (context), &color);
    gdk_cairo_set_source_rgba (cr, &color);
    gdk_cairo_set_source_rgba (cr, &color);
    cairo_fill (cr);
}

void add_edge(double x_1, double y_1, double x_2, double x_3){

}

void on_window_destroy(){
    gtk_main_quit();
}