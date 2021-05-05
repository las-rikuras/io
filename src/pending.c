#include <gtk/gtk.h>

int main(int argc, char *argv[]){
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/pending.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "pending"));
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void on_pending_destroy(){
    gtk_main_quit();
}

void on_close_clicked(){
    gtk_main_quit();
}