#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

GtkBuilder *builder;
GtkWidget *drawing_area;
cairo_t *cr;
GtkWidget *spinner;
guint spinner_num = 1;

int main(int argc, char *argv[]){  
    // gtk
    GtkWidget *window;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/test.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    spinner = GTK_WIDGET(gtk_builder_get_object(builder, "spinner"));
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void on_spinner_changed(){ 
    spinner_num = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinner));
    gtk_widget_queue_draw(drawing_area);
}

void add_edge(double x_1, double y_1, double x_2, double y_2){
    cairo_set_line_width(cr, 1);
    cairo_move_to(cr, x_1, y_1);
    cairo_line_to(cr, x_2, y_2);
    cairo_stroke(cr);
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

    double ang = 0;
    for(int i = 1; i < 10; i++){
        cairo_set_line_width(cr, 1);
        cairo_move_to(cr, width/2, height/2);
        cairo_line_to(cr, i * sin(ang), i * cos(ang));
        cairo_stroke(cr);
    }

    gtk_style_context_get_color (context, gtk_style_context_get_state (context), &color);
    gdk_cairo_set_source_rgba (cr, &color);
    gdk_cairo_set_source_rgba (cr, &color);
    cairo_fill (cr);
}

void on_window_destroy(){
    gtk_main_quit();
}