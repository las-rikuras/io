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
int project_lifetime = 1;
int cost = 400;

void load_time_units(Replacement *R){
    char markup [1024];
    for(int i = 0; i < R->equipment_lifetime; i++){
        GtkWidget *maint = gtk_grid_get_child_at(GTK_GRID(time_units_g), 1, i + 1);
        GtkWidget *resale = gtk_grid_get_child_at(GTK_GRID(time_units_g), 2, i + 1);
        sprintf(markup, "%d", R->time_units[i]->maintenance);
        gtk_entry_set_text(GTK_ENTRY(maint), markup);
        sprintf(markup, "%d", R->time_units[i]->resale_price);
        gtk_entry_set_text(GTK_ENTRY(resale), markup);
    }
}

Unit** get_time_units(){
    Unit **time_units = calloc(equipment_lifetime, sizeof(Unit));
    for(int i = 0; i < equipment_lifetime; i++){
        time_units[i] = malloc(sizeof(Unit)); // 0 = maint. 1 = resale
        GtkWidget *maint = gtk_grid_get_child_at(GTK_GRID(time_units_g), 1, i + 1);
        GtkWidget *resale = gtk_grid_get_child_at(GTK_GRID(time_units_g), 2, i + 1);

        const char *val = gtk_entry_get_text(GTK_ENTRY(maint));
        if(strlen(val) == 0)
            val = gtk_entry_get_placeholder_text(GTK_ENTRY(maint));
        time_units[i]->maintenance = atoi(val);

        val = gtk_entry_get_text(GTK_ENTRY(resale));
        if(strlen(val) == 0)
            val = gtk_entry_get_placeholder_text(GTK_ENTRY(resale));   
        time_units[i]->resale_price = atoi(val);
    }  
    return time_units;
} 

void entry_number(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data){
    GtkEntry*entry = GTK_ENTRY(editable);
    const char *current_text = gtk_entry_get_text(entry);
    if(strlen(current_text) != 0){
        for (int i = 0; i < length; i++) {
            if (!isdigit(text[i])) {
                g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
                return;
            }
        }
    }
    else{
        if (!isdigit(text[0]) || text[0] == '0') {
            g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
            return;
        } 
    }
}

void insert_entry(GtkWidget *grid, int i, int j, int width, char *placeholder){
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_width_chars (GTK_ENTRY(entry), width);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder);
    gtk_entry_set_alignment (GTK_ENTRY(entry), 0.5);
    g_signal_connect(G_OBJECT(entry), "insert_text", G_CALLBACK(entry_number), NULL);
    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}

void insert_label(GtkWidget *grid, int i, int j, int width, char *val, char *markup){
    GtkWidget *label;
    GtkWidget *event_box = gtk_event_box_new();

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

    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(event_box), i, j, 1 ,1);
    gtk_widget_show (event_box);

    gtk_container_add (GTK_CONTAINER(event_box), label);
    gtk_widget_show (GTK_WIDGET(label));
}

void on_equipment_lifetime_spin_value_changed(GtkSpinButton *spin_button, gpointer user_data){
    equipment_lifetime = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(equipment_spin));
    if(equipment_lifetime > previous_equipment_lifetime){
        char label[100];
        for(int i = previous_equipment_lifetime; i < equipment_lifetime; i++){     
            sprintf(label, "%d", i + 1);
            insert_label(time_units_g, 0, i + 1, 4, label, NULL);
            insert_entry(time_units_g, 1, i + 1, 5, "10");
            sprintf(label, "%d", cost/4 * 3);
            insert_entry(time_units_g, 2, i + 1, 5, label);
        }
    } else if (equipment_lifetime < previous_equipment_lifetime) {
        for(int i = previous_equipment_lifetime; i > equipment_lifetime; i--){
            gtk_grid_remove_row(GTK_GRID(time_units_g), i);
        }
    }
    previous_equipment_lifetime = equipment_lifetime;
}

void on_initial_cost_spin_value_changed(GtkSpinButton *spin_button, gpointer user_data){
    cost = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cost_spin));
}

void on_project_lifetime_spin_value_changed(GtkSpinButton *spin_button, gpointer user_data){
    project_lifetime = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(project_spin));
}

void load_next_nodes(Replacement *R){
    /*
        Carga el array para los saltos que puede dar
    */
}

void load_analysis(Replacement *R){
    /*
        Carga lo del analisis
        Son tres columnas
        Ver como acomodar el array de load_next_nodes que va en
        la ultima columna
        los otros dos se cargan facil
    */
}

Replacement *init_from_ui(){
    Replacement *R = malloc(sizeof(Replacement));
    R->initial_cost = cost;
    R->project_lifetime = project_lifetime;
    R->equipment_lifetime = equipment_lifetime;
    R->time_units = get_time_units();
    init_replacement_from_file(R);
    return R;
}

void solve_replacement(GtkButton *button, gpointer user_data){
    Replacement *R = init_from_ui();
    print_replacement(R);
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
        // clear analysis
        char *fn;
        fn = gtk_file_chooser_get_filename(chooser);
        Replacement *R = load_replacement(fn);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(project_spin), R->project_lifetime);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(equipment_spin), R->equipment_lifetime);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(cost_spin), R->initial_cost);
        load_time_units(R);

        free(fn);
        free(R);
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
        Replacement *R = init_from_ui();
        save_replacement(fn, R);
        free(fn);
        free(R);
    }
    gtk_widget_destroy(dialog);
}

void kill_process(){
    gtk_main_quit();
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