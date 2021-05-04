#include <gtk/gtk.h>

int main(int argc, char *argv[]){
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/knapsack.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

void on_window_destroy(){
    gtk_main_quit();
}

void on_load_clicked(){
    printf("LOAD\n");
}

void on_save_clicked(){
    printf("SAVE\n");   
}

void on_text_box_changed(){
    printf("Text box\n");   
}

void on_combo_changed(GtkEntry *e){
    printf("Combo %s\n", gtk_entry_get_text(e));   
}

void on_spinner_changed(){
    printf("Spinner\n");   
}