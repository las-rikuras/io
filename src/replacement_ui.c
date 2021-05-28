#include <gtk/gtk.h>
#include "replacement.c"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *cost_spin;
GtkWidget *project_spin;
GtkWidget *equipment_spin;
GtkWidget *solve_button;
GtkWidget *time_units_g;
GtkWidget *analysis_g;

int equipment_lifetime = 1;
int previous_equipment_lifetime = 1;

Replacement *init_from_ui(){
    Replacement *R = malloc(sizeof(Replacement));

    return R;
}

void solve_replacement(GtkButton *button, gpointer user_data){

}

void load_on_time_units(){

}


void insert_label(GtkWidget *grid, int i, int j, int width, char *val, char *markup){
    GtkWidget *label;
    if(markup){
        label = gtk_label_new(NULL);
        char str [2048];
        sprintf(str, markup, val);
        gtk_label_set_markup(GTK_LABEL(label), str);
    }
    else {
        label = gtk_label_new(val);
    }
    gtk_label_set_width_chars (GTK_LABEL(label), width);

    gtk_widget_show (GTK_WIDGET(label));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(label), i, j, 1 ,1);
}

void on_equipment_lifetime_spin_value_changed(GtkSpinButton *spin_button, gpointer user_data){
    equipment_lifetime = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(equipment_spin));
    if(equipment_lifetime > previous_equipment_lifetime){

    } else if (equipment_lifetime < previous_equipment_lifetime) {

    }
}


void on_load_clicked(){
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window), action, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.rp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    chooser = GTK_FILE_CHOOSER(dialog);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT){
        clear_kn();
        char *fn;
        fn = gtk_file_chooser_get_filename(chooser);

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

        free(fn);
    }
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/replacement.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "replacement"));
    
    project_spin = GTK_WIDGET(gtk_builder_get_object(builder, "project_lifetime_spin"));
    equipment_spin = GTK_WIDGET(gtk_builder_get_object(builder, "equipment_lifetime_spin"));
    cost_spin = GTK_WIDGET(gtk_builder_get_object(builder, "initial_cost_spin"));
    solve_button = GTK_WIDGET(gtk_builder_get_object(builder, "solve"));
    time_units_g = GTK_WIDGET(gtk_builder_get_object(builder, "time_units"));
    analysis_g = GTK_WIDGET(gtk_builder_get_object(builder, "anal_g"));
    
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
