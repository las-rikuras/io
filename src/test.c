#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <cairo.h>

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
   // g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_nodes), NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void on_spinner_changed(){ 
    spinner_num = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinner));
    gtk_widget_queue_draw(drawing_area);
}

void on_drawing_area_draw(GtkWidget *widget, cairo_t *cr, gpointer data){
    guint width, height;
    GdkRGBA color;
    GtkStyleContext *context;
    double size = spinner_num / 1.0;

    context = gtk_widget_get_style_context (widget);
    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);
    gtk_render_background(context, cr, 0, 0, width, height);
    
    for(int i = 1; i <= spinner_num; i++){  
        cairo_arc (cr, i * 50.0, 40, MIN (width, height) / (3.0 * size), 0, 2 * G_PI);
    }

    gtk_style_context_get_color (context, gtk_style_context_get_state (context), &color);
    gdk_cairo_set_source_rgba (cr, &color);
    gdk_cairo_set_source_rgba (cr, &color);
    cairo_fill (cr);
}
void on_window_destroy(){
    gtk_main_quit();
}