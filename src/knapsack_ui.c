#include <gtk/gtk.h>
#include "knapsack.c"
#include "util/gtk-utility.c"

enum Algorithm {BOUNDED, UNBOUNDED, BINARY};
GtkWidget *combo;
GtkWidget *spinnerTask;
GtkWidget *spinnerCapacity;
GtkWidget *spinnerMax;
GtkWidget *max_label;
GtkWidget *tasks;
GtkWidget *solution;
GtkWidget *knapsack_grid;
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *knapsackType;
GtkWidget *z_grid;
GtkWidget *subjects_grid;
GtkWidget *x_sub_i;

guint32 task_number = 1;
int previous_task_number = 1;
guint32 capacity_number = 1;
int previous_capacity_number = 1;
guint32 max_number = 0;
enum Algorithm type = BINARY;

void change_x_sub_i(char *val);

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
    int index = gtk_combo_box_get_active(GTK_COMBO_BOX(knapsackType));
    if(index == 1){
        type = BOUNDED;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinnerMax), 1);
        gtk_widget_set_visible(spinnerMax, 1);
        gtk_widget_set_visible(max_label, 1);   
        change_x_sub_i("... 1");
    } else if (index == 2) {
        type = UNBOUNDED;
        gtk_widget_set_visible(spinnerMax, 0);
        gtk_widget_set_visible(max_label, 0);
        change_x_sub_i("... ∞");
    } else {
        type = BINARY;
        gtk_widget_set_visible(spinnerMax, 0);
        gtk_widget_set_visible(max_label, 0);
        change_x_sub_i("o 1");
    } 
}

void change_on_z(GtkWidget *grid,const char *lab, const char *val, int j){
    GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(grid), j, 0);
    int num_val = atoi(val);
    char markup[2024];
    if(num_val == 1){
        sprintf(markup, "<span foreground=\"orange1\">x</span><span foreground=\"#7dfffb\" size=\"smaller\"><sub>%s</sub></span>", lab);
    }
    else{
        sprintf(markup, "%s<span foreground=\"orange1\">x</span><span foreground=\"#7dfffb\" size=\"smaller\"><sub>%s</sub></span>", val, lab);
    }
    gtk_label_set_markup(GTK_LABEL(label), markup);
}

void value_change(GtkEditable *editable, gpointer data){
    GValue temp = G_VALUE_INIT;
    g_value_init (&temp, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(tasks), GTK_WIDGET(editable), "left-attach", &temp);

    int j = g_value_get_int(&temp);
    const char *val = gtk_entry_get_text(GTK_ENTRY(editable));
    GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(tasks), j, 0);
    const char *lab = gtk_entry_get_placeholder_text(GTK_ENTRY(label));
    if(j == 1){
        change_on_z(z_grid, lab, val, j);
    }
    else{
        change_on_z(z_grid, lab, val, j*2);
    }
}

void weigth_change(GtkEditable *editable, gpointer data){
    GValue temp = G_VALUE_INIT;
    g_value_init (&temp, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(tasks), GTK_WIDGET(editable), "left-attach", &temp);

    int j = g_value_get_int(&temp);
    const char *val = gtk_entry_get_text(GTK_ENTRY(editable));
    GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(tasks), j, 0);
    const char *lab = gtk_entry_get_placeholder_text(GTK_ENTRY(label));
    change_on_z(subjects_grid, lab, val, (j-1)*2);
}

void change_x_sub_i(char *val){
    char markup[2024];
    gtk_label_set_text(GTK_LABEL(x_sub_i), "");
    sprintf(markup, "<span foreground=\"orange1\">x</span><span foreground=\"#7dfffb\" size=\"smaller\"><sub> i</sub></span> = 0 %s", val);
    gtk_label_set_markup(GTK_LABEL(x_sub_i), markup);
}

void on_task_changed(){ 
    task_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerTask));
    if(task_number > previous_task_number){
        for(int i = previous_task_number+1; i <= task_number; i++){
            char label[2048];

            int letter = 64+i;
            sprintf(label, "%c", letter);
            insertEntryLabel(tasks, i, 0, NULL, label, 0, 1);

            insertLabel(z_grid, i*2-1, 0, "+", NULL);
            insertLabel(z_grid, i*2, 0, label, "<span foreground=\"orange1\">x</span><span foreground=\"#7dfffb\" size=\"smaller\"><sub>%s</sub></span>");

            gtk_grid_insert_column(GTK_GRID(subjects_grid), (i-1)*2-1);
            gtk_grid_insert_column(GTK_GRID(subjects_grid), (i-1)*2-1);

            insertLabel(subjects_grid, (i-1)*2-1, 0, "+", NULL);
            insertLabel(subjects_grid, (i-1)*2, 0, label, "<span foreground=\"orange1\">x</span><span foreground=\"#7dfffb\" size=\"smaller\"><sub>%s</sub></span>");

            insertLabel(knapsack_grid, i*2-1, 0, label, NULL);
            for(int j = 1; j <= capacity_number+1; j++){
                insertLabel(knapsack_grid, i*2-1, j, "0", NULL);
                insertLabel(knapsack_grid, i*2, j, "", NULL);
            }

            int val = 49;
            sprintf(label, "%c", val);
            insertEntry(tasks, i, 1, NULL, label, 1, 0, 1);

            int weigth = 49;
            sprintf(label, "%c", weigth);
            insertEntry(tasks, i, 2, NULL, label, 1, 0, 0);
        }
    }
    else{ 
        for(int i = previous_task_number; i > task_number; i--){
            gtk_grid_remove_column(GTK_GRID(tasks), i);
            gtk_grid_remove_column(GTK_GRID(z_grid), i*2);
            gtk_grid_remove_column(GTK_GRID(z_grid), i*2-1);
            gtk_grid_remove_column(GTK_GRID(knapsack_grid), i*2);
            gtk_grid_remove_column(GTK_GRID(knapsack_grid), i*2-1);
            gtk_grid_remove_column(GTK_GRID(subjects_grid), i*2-2);
            gtk_grid_remove_column(GTK_GRID(subjects_grid), i*2-3);
        }
    } 
    previous_task_number = task_number;
}

void on_capacity_changed(){
    capacity_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerCapacity));
    char str[10];
    sprintf(str, "≤ %d", capacity_number);
    GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(subjects_grid), task_number*2-1, 0);
    gtk_label_set_text(GTK_LABEL(label), str);

    if(capacity_number > previous_capacity_number){
        for(int i = previous_capacity_number+2; i <= capacity_number+1; i++){
            char lab[10];
            sprintf(lab, "%d", i-1);
            insertLabel(knapsack_grid, 0, i, lab, NULL);
            for(int j = 1; j <= task_number*2; j++){
                if(j%2 == 0){
                    insertLabel(knapsack_grid, j, i, "", NULL);
                }else{
                    insertLabel(knapsack_grid, j, i, "0", NULL);
                }
            }
        }
    }
    else{
        for(int i = previous_capacity_number+1; i > capacity_number+1; i--){
            gtk_grid_remove_row(GTK_GRID(knapsack_grid), i);
        }
    } 
    previous_capacity_number = capacity_number;
}

void on_max_changed(){
    max_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinnerMax));
    char str[10];
    sprintf(str, "... %d", max_number);
    change_x_sub_i(str);
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
    z_grid = GTK_WIDGET(gtk_builder_get_object(builder, "z_grid"));
    subjects_grid = GTK_WIDGET(gtk_builder_get_object(builder, "subjects_grid"));
    max_label = GTK_WIDGET(gtk_builder_get_object(builder, "max_label"));
    x_sub_i = GTK_WIDGET(gtk_builder_get_object(builder, "x_sub_i"));

    gtk_combo_box_set_active(GTK_COMBO_BOX(knapsackType), 1);
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}