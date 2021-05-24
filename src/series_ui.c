#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "series.c"

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *games_spinner;
GtkWidget *s_grid;
GtkWidget *format;
GtkWidget *p_h_spin;
GtkWidget *p_r_spin;
GtkWidget *pr_celtics;
GtkWidget *ph_celtics;
GtkWidget *pr_lakers;
GtkWidget *ph_lakers;

char *celtics_image = "src/util/celtics.png";
char *lakers_image = "src/util/lakers.png";

int game_number = 1;
int previous_game_number = 1;

void load_series_on_table(Series *series){
    char markup [1024];
    for(int i = 2; i <= series->n; i++){
        for(int j = 2; j <= series->n; j++){
            GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(s_grid), j, i);
            if(i == 2 && j == 2){
                sprintf(markup, "<span foreground=\"#007A33\">%0.4f</span>", series->prob[i][j]);
            }
            else{
                sprintf(markup, "<span foreground=\"#007A33\">%0.4f</span>", series->prob[i][j]);
            }
        }
    }
}

Series *init_series_from_ui(){
    Series *series = (Series*)calloc(1, sizeof(Series));
    double p_h = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p_h_spin));
    double p_r = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p_r_spin));
    int *format_array = (int *)calloc(game_number, sizeof(int));
    for(int i = 0; i < game_number; i++){
        GtkWidget *toggle_button = gtk_grid_get_child_at(GTK_GRID(format), i, 1);
        format_array[i] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button));
        printf("%d -> %d\n", i, format_array[i]);
    }
    init_series(series, p_h, p_r, format_array, game_number);
    printf("Si\n");
    return series;
}

void solve_series_ui(GtkButton *button, gpointer user_data){
    Series *series = init_series_from_ui();
    solve_series(series);
    load_series_on_table(series);
}

void on_close_clicked(){
    gtk_main_quit();
}

void toggle_serie(GtkToggleButton *togglebutton, gpointer user_data){
    int is_celtics = gtk_toggle_button_get_active(togglebutton);
    GtkWidget *image = gtk_bin_get_child(GTK_BIN(togglebutton));
    if(is_celtics){
        gtk_image_set_from_file(GTK_IMAGE(image), celtics_image);
    }else{
        gtk_image_set_from_file(GTK_IMAGE(image), lakers_image);
    }
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

void insert_toggle(GtkWidget *grid, int i, int j){
    GtkWidget *toggle = gtk_toggle_button_new();
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), 1);
    GtkWidget *image = gtk_image_new_from_file(celtics_image);
    g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(toggle_serie), NULL);
    gtk_container_add(GTK_CONTAINER(toggle), image);
    gtk_widget_show_all (GTK_WIDGET(toggle));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(toggle), i, j, 1 ,1);
}

void on_max_games_value_changed(GtkSpinButton *spin_button, gpointer user_data){
    game_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(games_spinner));
    if(game_number%2 == 0){
        game_number--;
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(games_spinner), game_number);
    }
    else{
        if(game_number > previous_game_number){
            char label[100];
            if(previous_game_number == 1){
                previous_game_number = 0;
            }
            for(int i = previous_game_number/2+1; i < game_number/2+1; i++){
                sprintf(label, "%d", i + 1);
                insert_label(s_grid, i+2, 0, 8, label , NULL);
                insert_label(s_grid, 0, i+2, 4, label , NULL);
                insert_label(s_grid, i+2, 1, 8, "1.0000" , NULL);
                insert_label(s_grid, 1, i+2, 8, "0.0000", NULL);
                for(int j = 0; j < i; j++){
                    insert_label(s_grid, i+2, j+2, 8, "0.0000", NULL);
                    insert_label(s_grid, j+2, i+2, 8, "0.0000", NULL);
                }
                insert_label(s_grid, i+2, i+2, 8, "0.0000", NULL);
            }
            if(previous_game_number == 0){
                previous_game_number = 1;
            }
            for(int i = previous_game_number+1; i <= game_number; i++){
                sprintf(label, "%d", i);
                insert_label(format, i, 0, 4, label, NULL);
                insert_toggle(format, i, 1);
            }
        } else {
            for(int i = previous_game_number/2+1; i >= game_number/2+1; i--){
                gtk_grid_remove_column(GTK_GRID(s_grid), i+2);
                gtk_grid_remove_row(GTK_GRID(s_grid), i+2);
            }
            for(int i = previous_game_number; i > game_number; i--){
                gtk_grid_remove_column(GTK_GRID(format), i);
            }
        }
        previous_game_number = game_number;
    }
}

void on_p_h_spin_changed(GtkSpinButton *spin_button, gpointer user_data){
    double p_h = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p_h_spin));
    char label[100];
    sprintf(label, "<i><span foreground=\"#00ff00\">p</span><span foreground=\"#00ff00\"><sub>h</sub></span></i> = %.4lf", p_h);
    gtk_label_set_markup(GTK_LABEL(ph_celtics), label);
    sprintf(label, "<i><span foreground=\"#fdb927\">p</span><span foreground=\"#fdb927\"><sub>r</sub></span></i> = %.4lf", 1 - p_h);
    gtk_label_set_markup(GTK_LABEL(pr_lakers), label);
}

void on_p_r_spin_changed(GtkSpinButton *spin_button, gpointer user_data){
    double p_r = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p_r_spin));
    char label[100];
    sprintf(label, "<i><span foreground=\"#00ff00\">p</span><span foreground=\"#00ff00\"><sub>r</sub></span></i> = %.4lf", p_r);
    gtk_label_set_markup(GTK_LABEL(pr_celtics), label);
    sprintf(label, "<i><span foreground=\"#fdb927\">p</span><span foreground=\"#fdb927\"><sub>h</sub></span></i> = %.4lf", 1 - p_r);
    gtk_label_set_markup(GTK_LABEL(ph_lakers), label);
}

void on_load_clicked(){
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window), action, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
    filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.sr");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    chooser = GTK_FILE_CHOOSER(dialog);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT){
        char *fn;
        fn = gtk_file_chooser_get_filename(chooser);
        Series *S = load_series(fn);
        init_series_from_file(S);

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
    builder = gtk_builder_new_from_file("glade/series.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "series"));
    games_spinner = GTK_WIDGET(gtk_builder_get_object(builder, "max_games"));
    s_grid = GTK_WIDGET(gtk_builder_get_object(builder, "s_grid"));
    format = GTK_WIDGET(gtk_builder_get_object(builder, "format"));
    p_h_spin = GTK_WIDGET(gtk_builder_get_object(builder, "p_h_spin"));
    p_r_spin = GTK_WIDGET(gtk_builder_get_object(builder, "p_r_spin"));
    pr_celtics = GTK_WIDGET(gtk_builder_get_object(builder, "pr_celtics"));
    ph_celtics = GTK_WIDGET(gtk_builder_get_object(builder, "ph_celtics"));
    pr_lakers = GTK_WIDGET(gtk_builder_get_object(builder, "pr_lakers"));
    ph_lakers = GTK_WIDGET(gtk_builder_get_object(builder, "ph_lakers"));
     
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

