#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "replacement.c"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *cost_spin;
GtkWidget *project_spin;
GtkWidget *equipment_spin;
GtkWidget *solve_button;
GtkWidget *time_units_g;
GtkWidget *analysis_g;
GtkWidget *drawing_area;
GtkWidget *profit_g;

int equipment_lifetime = 1;
int previous_equipment_lifetime = 1;
int project_lifetime = 1;
int analysis_size = 0;
int cost = 400;

int draw_all_solutions = 0;
int draw_determinate_solution = 0;

int size = 541;
double scale = 1;

Solutions *routes;
int current_lifetime = 1;

void load_css(char *file, GtkWidget *widget);

void load_css(char *file, GtkWidget *widget){
    GtkCssProvider *provider = gtk_css_provider_new ();
    GtkStyleContext * context = gtk_widget_get_style_context(widget);
    gtk_css_provider_load_from_path (provider, file, NULL);
    gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void draw_area(GtkWidget *widget, cairo_t *cr, gpointer data){
    if(draw_all_solutions){

        GdkRGBA *color;
        GtkStyleContext *context;
        context = gtk_widget_get_style_context (widget);

        gtk_render_background(context, cr, 0, 0, size, size);
        gtk_widget_set_size_request(widget, size, size);
        if((current_lifetime*100+41)*scale > size){
            gtk_render_background(context, cr, 0, 0, (current_lifetime*100+41)*scale , (510+20)*scale);
            gtk_widget_set_size_request(widget, (current_lifetime*100+41)*scale , (510+20)*scale);
        }
        if((510+20)*scale > size){
            gtk_render_background(context, cr, 0, 0, (current_lifetime*100+41)*scale , (510+20)*scale);
            gtk_widget_set_size_request(widget, (current_lifetime*100+41)*scale , (510+20)*scale);
        }

        cairo_save(cr);
        cairo_scale(cr, 1*scale, 1*scale);
        double scale_factor = 1;

        cairo_set_line_width(cr, 2.0);
        for(int i = 0; i < routes->size; i++){
            cairo_set_source_rgb (cr, routes->routes[i]->R, routes->routes[i]->G, routes->routes[i]->B);
            for(int j = 1; j < routes->routes[i]->size; j++){
                cairo_save(cr);
                cairo_scale(cr, 1, 1*scale_factor);
                cairo_arc_negative(cr, ((routes->routes[i]->elemets[j]+routes->routes[i]->elemets[j-1])*100+40)/2, 510/scale_factor, 
                                        (routes->routes[i]->elemets[j]-routes->routes[i]->elemets[j-1])*100/2, 0, G_PI);
                cairo_stroke(cr);
                cairo_restore(cr);
            }
            scale_factor *= 0.90;
        }

        for(int i = 0; i <= current_lifetime; i++){
            cairo_set_source_rgb (cr, 0.8, 0.2, 0.2);
            cairo_stroke(cr);
            cairo_save(cr);
            cairo_arc(cr, i*100+20, 510, 20, 0, 2*G_PI);
            cairo_set_source_rgba(cr, 1, 1, 1, 1); 
            cairo_fill_preserve(cr);
            cairo_restore(cr); 
            cairo_stroke(cr);
        }
        cairo_restore(cr); 

    }
    else if (draw_determinate_solution){
        
    }
}
  
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

    if(width)
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

void clear_analysis(){
    for(int i = analysis_size; i > 0; i--){
        gtk_grid_remove_row(GTK_GRID(analysis_g), i);
    }
    analysis_size = 0;
}

char* get_next_nodes_string(Replacement *R, int col){
   int *arr = get_next_nodes(R, col);
   int size = get_next_nodes_size(R, col);
   if(size > 0){
        char *label = malloc(sizeof(char) * 1000);
        char number[1000];
        if(size > 1)
            sprintf(number, "%d, ", arr[0]);
        else
            sprintf(number, "%d", arr[0]);
        strcpy(label, number);
        for(int i = 1; i < size; i++){        
            sprintf(number, "%d", arr[i]);
            strcat(label, number);
            if(i < size-1){
                strcat(label, ", ");
            }
        }  
        return label;
   } else {
       return "-";
   }   
}

void load_analysis(Replacement *R){
    clear_analysis();
    char label[100];
    for(int i = 0; i <= R->project_lifetime; i++){
        sprintf(label, "%d", i);
        insert_label(analysis_g, 0, i + 1, 4, label, NULL);

        sprintf(label, "%d", get_min(R, R->g, i));
        insert_label(analysis_g, 1, i + 1, 4, label, NULL);

        insert_label(analysis_g, 2, i + 1, 4, get_next_nodes_string(R, i), NULL);
        analysis_size++;
    }
    routes = get_solutions(R);
    draw_all_solutions = 1;
    current_lifetime = R->project_lifetime;
    gtk_widget_queue_draw(drawing_area);
}

void clear_profit(Replacement *R){
    GList *rows, *it;
    rows = gtk_container_get_children(GTK_CONTAINER(profit_g));
    for(it = rows; it != NULL; it = g_list_next(it))
        gtk_widget_destroy(GTK_WIDGET(it->data));
    g_list_free(rows);
}

void fill_profit_per_time_unit(Replacement *R){
    clear_profit(R);
    char val[500 * R->project_lifetime];
    int i = 1;
    int j = 0;
    while(i <= R->equipment_lifetime && i <= R->project_lifetime){
        while(i + j <= R->project_lifetime){ 
            sprintf(val, "<span foreground=\"#3da4ab\" size=\"larger\">C</span><span foreground=\"#f6cd61\"><sub>%d</sub></span><span foreground=\"#fe8a71\"><sub>%d</sub></span> =", j, j+i);
            insert_label(profit_g, j, i, 0, "", val); 
            j++;       
        }
        sprintf(val, "%d + %d - %d = %d", R->initial_cost, R->time_units[i-1]->maintenance, R->time_units[i-1]->resale_price, c(R, j-1, j-1+i));
        insert_label(profit_g, R->project_lifetime, i, 0, "", val); 
        i++;
        j = 0;
    }   
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
    load_analysis(R);
    fill_profit_per_time_unit(R);
    free(R);
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
        init_replacement_from_file(R);
        load_time_units(R);
        load_analysis(R);
        fill_profit_per_time_unit(R);
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
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(window), action, "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);
    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.rp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
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

void mouse_scroll(GtkWidget *widget, GdkEventScroll *event, gpointer user_data){
    /*accel_mask = Gtk.accelerator_get_default_mod_mask()
   if event.state & accel_mask == Gdk.ModifierType.CONTROL_MASK:
     direction = event.get_scroll_deltas()[2]
     if direction > 0:  # scrolling down -> zoom out
        self.set_zoom_level(self.get_zoom_level() - 0.1)
     else:
        self.set_zoom_level(self.get_zoom_level() + 0.1)*/
    
    if(event->direction == GDK_SCROLL_UP && event->state & GDK_CONTROL_MASK){
        scale += 0.10; 
    }
    else if(event->direction == GDK_SCROLL_DOWN && event->state & GDK_CONTROL_MASK){
        scale -= 0.10; 
    }
    gtk_widget_queue_draw(drawing_area);
}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/replacement.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "replacement"));
    drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    project_spin = GTK_WIDGET(gtk_builder_get_object(builder, "project_lifetime_spin"));
    equipment_spin = GTK_WIDGET(gtk_builder_get_object(builder, "equipment_lifetime_spin"));
    cost_spin = GTK_WIDGET(gtk_builder_get_object(builder, "initial_cost_spin"));
    solve_button = GTK_WIDGET(gtk_builder_get_object(builder, "solve"));
    time_units_g = GTK_WIDGET(gtk_builder_get_object(builder, "time_units"));
    analysis_g = GTK_WIDGET(gtk_builder_get_object(builder, "anal_g"));

    gtk_widget_add_events(drawing_area, GDK_SCROLL_MASK);

    g_signal_connect(drawing_area, "scroll-event", G_CALLBACK(mouse_scroll), NULL);
    g_signal_connect(drawing_area, "draw", G_CALLBACK (draw_area), NULL);
    
    
    profit_g = GTK_WIDGET(gtk_builder_get_object(builder, "profit_g"));
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}