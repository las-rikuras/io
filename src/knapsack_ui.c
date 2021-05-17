#include <gtk/gtk.h>
#include "knapsack.c"
#include "util/gtk-utility.c"

enum Algorithm {BOUNDED, UNBOUNDED, BINARY};
GtkWidget *combo;
GtkWidget *spinnerTask;
GtkWidget *spinnerCapacity;
GtkWidget *spinnerMax;
GtkWidget *tasks;
GtkWidget *solution;
GtkWidget *knapsack_grid;
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *knapsackType;

guint32 task_number = 1;
int previous_task_number = 1;
guint32 capacity_number = 1;
guint32 max_number = 0;
enum Algorithm type = BINARY;

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
}

void on_task_changed(){ 
    task_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerTask));
    if(task_number > previous_task_number){
        for(int i = previous_task_number+1; i <= task_number; i++){
            char label[2];

            int letter = 64+i;
            sprintf(label, "%c", letter);
            insertEntryLabel(tasks, i, 0, NULL, label, 0, 1);
            for(int j = 0; j <= capacity_number; j++){
                insertLabel(knapsack_grid, i*2-1, j, label);
                if(j != 0){
                    insertLabel(knapsack_grid, i*2, j, "label");
                }
                sprintf(label, "%s", "");
            }

            int val = 48;
            sprintf(label, "%c", val);
            insertEntry(tasks, i, 1, NULL, label, 1, 0);

            int weigth = 49;
            sprintf(label, "%c", weigth);
            insertEntry(tasks, i, 2, NULL, label, 1, 0);
        }
        previous_task_number = task_number;
    }
    else{
        for(int i = previous_task_number+1; i > task_number; i--){
            gtk_grid_remove_column(GTK_GRID(tasks), i);
            gtk_grid_remove_column(GTK_GRID(knapsack_grid), i*2);
            gtk_grid_remove_column(GTK_GRID(knapsack_grid), i*2-1);
        }
        previous_task_number = task_number;
    }
}

void on_capacity_changed(){
    capacity_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerCapacity));
}

void on_max_changed(){
    max_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerMax));
    printf("%d\n", max_number);  
}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/knapsack.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    spinnerTask = GTK_WIDGET(gtk_builder_get_object(builder, "task"));
    spinnerCapacity = GTK_WIDGET(gtk_builder_get_object(builder, "capacity"));
    spinnerMax = GTK_WIDGET(gtk_builder_get_object(builder, "max"));
    tasks = GTK_WIDGET(gtk_builder_get_object(builder, "tasks"));
    solution = GTK_WIDGET(gtk_builder_get_object(builder, "solution"));
    knapsack_grid = GTK_WIDGET(gtk_builder_get_object(builder, "knapsack"));
    knapsackType = GTK_WIDGET(gtk_builder_get_object(builder, "knapsackType"));
    gtk_combo_box_set_active(GTK_COMBO_BOX(knapsackType), 0);
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}