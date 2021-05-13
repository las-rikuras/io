#include <gtk/gtk.h>
#include "knapsack.c"

enum Algorithm {BOUNDED, UNBOUNDED, BINARY};
GtkWidget *combo;
GtkWidget *spinnerTask;
GtkWidget *spinnerCapacity;
GtkWidget *spinnerMax;
GtkWidget *tasks;
GtkWidget *knapsack;
GtkBuilder *builder;
GtkWidget *window;

guint32 task_number = 0;
guint32 capacity_number = 0;
guint32 max_number = 0;
enum Algorithm type = BINARY;

int main(int argc, char *argv[]){
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
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window), action, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.kn");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    chooser = GTK_FILE_CHOOSER(dialog);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT){
        char *fn;
        fn = gtk_file_chooser_get_filename(chooser);
        // load_knapsack
        free(fn);
    }
    gtk_widget_destroy(dialog);  
}

void on_save_clicked(){
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), action, "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);
    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_current_name(chooser, "Untitled");
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT){
        char *fn;
        fn = gtk_file_chooser_get_filename(chooser);
        // save_knapsack
        free(fn);
    }
    gtk_widget_destroy(dialog);
}

void on_combo_changed(GtkEntry *e){
    const gchar *txt = gtk_entry_get_text(e);
    if(strcmp(txt,"Bounded")){
        type = BOUNDED;
        printf("1 ");
    } else if (strcmp(txt,"Ubounded")) {
        type = UNBOUNDED;
        printf("2 ");
    } else {
        type = BINARY;
        printf("3 ");
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
