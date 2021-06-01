#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
//#include "replacement.c"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *drawing_area;

int size = 541;
double scale = 1;

void load_css(char *file, GtkWidget *widget);

void load_css(char *file, GtkWidget *widget){
    GtkCssProvider *provider = gtk_css_provider_new ();
    GtkStyleContext * context = gtk_widget_get_style_context(widget);
    gtk_css_provider_load_from_path (provider, file, NULL);
    gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void draw_area(GtkWidget *widget, cairo_t *cr, gpointer data){
    GtkWidget* spinner = data;
    GdkRGBA *color;
    GtkStyleContext *context;
    context = gtk_widget_get_style_context (widget);
    gtk_render_background(context, cr, 0, 0, size, size);
    gtk_widget_set_size_request(widget, size, size);


}

/* void draw_area(GtkWidget *widget, cairo_t *cr, gpointer data){
    GtkWidget* spinner = data;
    int size = 541;
    GdkRGBA *color;
    GtkStyleContext *context;
    context = gtk_widget_get_style_context (widget);
    gtk_render_background(context, cr, 0, 0, size+200, size+200);
    gtk_widget_set_size_request(widget, size+200, size+200);

    int scale = 1;

    cairo_set_line_width(cr, 2);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 5; j++){
            if(matrix[i][j]){  

                cairo_save(cr);
                cairo_scale(cr, 1, 1/scale);
                cairo_set_source_rgb (cr, 0, 0.2, 0.8);
                cairo_arc_negative(cr, (coordinates[matrix[i][j]]+coordinates[i+j+1])/2, 300*scale, (coordinates[matrix[i][j]]-coordinates[i+j+1])/2, 0, G_PI);
                cairo_stroke(cr);
                cairo_restore(cr);
            }
        }  
    }   


    for(int i = 0; i < 6; i++){
        cairo_set_source_rgb (cr, 0.8, 0.2, 0.2);
        cairo_stroke(cr);
        cairo_save(cr);
        cairo_arc(cr, coordinates[i], 300, 20, 0, 2*G_PI);
        cairo_set_source_rgba(cr, 1, 1, 1, 1); 
        cairo_fill_preserve(cr);
        cairo_restore(cr); 
        cairo_stroke(cr);
    } 

}
 */

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/replacement.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "replacement_ui"));
    drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));

    g_signal_connect(drawing_area, "draw", G_CALLBACK (draw_area), NULL);

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}