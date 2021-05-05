#include <gtk/gtk.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/menu.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_menu"));

    // set tooltips
    button = GTK_WIDGET(gtk_builder_get_object(builder, "floyd"));
    gtk_widget_set_tooltip_text(button, "Algorithm to find the shortest distance between any pair of nodes");

    button = GTK_WIDGET(gtk_builder_get_object(builder, "series"));
    gtk_widget_set_tooltip_text(button, "Algorithm to find the probability for a team to win a determined game in a series");

    button = GTK_WIDGET(gtk_builder_get_object(builder, "knapsack"));
    gtk_widget_set_tooltip_text(button, "This algorithm allows to optimally allocate a budget between various activities");

    button = GTK_WIDGET(gtk_builder_get_object(builder, "replacement"));
    gtk_widget_set_tooltip_text(button, "Algorithm to determine when it is best to make an equipment change within a defined period of time");
    
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void on_window_menu_destroy(){
    gtk_main_quit();
}

void on_exit_clicked(){
    gtk_main_quit();
}

void* callpending(void *arg);

void on_floyd_clicked(){
    pthread_t thread;
    pthread_create(&thread, NULL, &callpending, NULL);
}

void on_knapsack_clicked(){
    pthread_t thread;
    pthread_create(&thread, NULL, &callpending, NULL);
}

void on_replacement_clicked(){
    pthread_t thread;
    pthread_create(&thread, NULL, &callpending, NULL);
}

void on_series_clicked(){
    pthread_t thread; //
    pthread_create(&thread, NULL, &callpending, NULL);
}

void* callpending(void *arg){
    system("./pending");
}