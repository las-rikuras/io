#include <gtk/gtk.h>

enum Algorithm {BOUNDED, UNBOUNDED, BINARY};
GtkWidget *combo;
GtkWidget *spinnerTask;
GtkWidget *spinnerCapacity;
GtkWidget *spinnerMax;
GtkWidget *tasks;
GtkWidget *knapsack;

guint32 task_number = 0;
guint32 capacity_number = 0;
guint32 max_number = 0;
enum Algorithm type = BINARY;

int main(int argc, char *argv[]){
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/knapsack.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    spinnerTask = GTK_WIDGET(gtk_builder_get_object(builder, "task"));
    spinnerCapacity = GTK_WIDGET(gtk_builder_get_object(builder, "capacity"));
    spinnerMax = GTK_WIDGET(gtk_builder_get_object(builder, "max"));
    tasks = GTK_WIDGET(gtk_builder_get_object(builder, "taskValues"));
    knapsack = GTK_WIDGET(gtk_builder_get_object(builder, "knappsackSolve"));
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

void on_about_clicked(){
    printf("ABOUT\n");   
}

void on_combo_changed(GtkEntry *e){
    gchar txt = gtk_entry_get_text(e);
    if(txt == "Bounded"){
        type = BOUNDED;
    } else if (txt == "Unbounded") {
        type = UNBOUNDED;
    } else {
        type = BINARY;
    }
    printf("%s\n", txt);   
}

void on_task_changed(){ 
    task_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerTask));
    printf("%d\n", task_number);
}

void on_capacity_changed(){
    capacity_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerCapacity));
    printf("%d\n", capacity_number); 
}

void on_max_changed(){
    max_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerMax));
    printf("%d\n", max_number);  
}
