#include <gtk/gtk.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/menu.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_menu"));
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

void on_floyd_clicked(){
    system("./pending");
}

void on_knapsack_clicked(){
    system("./pending");
}

void on_replacement_clicked(){
    system("./pending");
}

void on_series_clicked(){
    system("./pending");
}