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
GtkWidget *s_scroll;
GtkWidget *format_scroll;
GtkWidget *first_toggle;
GtkWidget *toolbar;
GtkWidget *format_label;

char *celtics_image = "src/util/celtics.png";
char *lakers_image = "src/util/lakers.png";

int game_number = 1;
int previous_game_number = 1;

void load_css(char *file, GtkWidget *widget);

void update_format_label(){
    char label[1024];
    memset(label,0,strlen(label));
    char markup[2048];

    GtkWidget *toggle = gtk_grid_get_child_at(GTK_GRID(format), 0, 1);
    int current_team = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));
    int previous_team = current_team;

    int i;
    int games_count = 1;

    for(i = 1; i < game_number; i++){
        previous_team = current_team;
        toggle = gtk_grid_get_child_at(GTK_GRID(format), i, 1);
        current_team = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));
        if(current_team != previous_team)
            break;
        games_count++;
    }

    if(previous_team)
        sprintf(markup, "<span foreground=\"#007A33\" size=\"large\">%d</span>", games_count);
    else 
        sprintf(markup, "<span foreground=\"#552583\" size=\"large\">%d</span>", games_count);
    strcat(label, markup);

    games_count = 0;
    for(i; i < game_number; i++){
        previous_team = current_team;
        toggle = gtk_grid_get_child_at(GTK_GRID(format), i, 1);
        current_team = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle));
        if(current_team != previous_team){
            if(previous_team)
                sprintf(markup, "<span foreground=\"black\"> - </span><span foreground=\"#007A33\" size=\"large\">%d</span>", games_count);
            else 
                sprintf(markup, "<span foreground=\"black\"> - </span><span foreground=\"#552583\" size=\"large\">%d</span>", games_count);
            strcat(label, markup);
            games_count = 0;
        }
        games_count++;
        if(i+1 == game_number){
            if(current_team)
                sprintf(markup, "<span foreground=\"black\"> - </span><span foreground=\"#007A33\" size=\"large\">%d</span>", games_count);
            else 
                sprintf(markup, "<span foreground=\"black\"> - </span><span foreground=\"#552583\" size=\"large\">%d</span>", games_count);
            strcat(label, markup);
        }
    }
    sprintf(markup, "<span background=\"lightgrey\">%s</span>", label);
    gtk_label_set_markup(GTK_LABEL(format_label), markup);
}

void load_series_on_table(Series *series){
    char markup [1024];
    for(int i = 1; i <= series->n; i++){
        for(int j = 1; j <= series->n; j++){
            GtkWidget *label = gtk_grid_get_child_at(GTK_GRID(s_grid), j+1, i+1);
            if(i == series->n && j == series->n){
                sprintf(markup, "<span foreground=\"#007A33\">%0.4f</span>", series->prob[i][j]);
            }
            else{
                sprintf(markup, "%0.4f", series->prob[i][j]);
            }
            gtk_label_set_markup(GTK_LABEL(label), markup);
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
    }
    init_series(series, p_h, p_r, format_array, game_number);
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
    update_format_label();
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
    load_css("css/toggle.css", toggle);
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
                insert_label(format, i-1, 0, 4, label, NULL);
                insert_toggle(format, i-1, 1);
            }
        } else {
            for(int i = previous_game_number/2+1; i >= game_number/2+1; i--){
                gtk_grid_remove_column(GTK_GRID(s_grid), i+2);
                gtk_grid_remove_row(GTK_GRID(s_grid), i+2);
            }
            for(int i = previous_game_number; i >= game_number; i--){
                gtk_grid_remove_column(GTK_GRID(format), i);
            }
        }
        previous_game_number = game_number;
    }
    update_format_label();
}

void on_p_h_spin_changed(GtkSpinButton *spin_button, gpointer user_data){
    double p_h = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p_h_spin));
    char label[120];
    sprintf(label, "<i><span size=\"larger\" foreground=\"#007A33\">p</span><span foreground=\"#007A33\"><sub>h</sub></span></i> = %.4lf", p_h);
    gtk_label_set_markup(GTK_LABEL(ph_celtics), label);
    sprintf(label, "<i><span size=\"larger\" foreground=\"#fdb927\">p</span><span foreground=\"#fdb927\"><sub>r</sub></span></i>  = %.4lf", 1 - p_h);
    gtk_label_set_markup(GTK_LABEL(pr_lakers), label);
}

void on_p_r_spin_changed(GtkSpinButton *spin_button, gpointer user_data){
    double p_r = gtk_spin_button_get_value(GTK_SPIN_BUTTON(p_r_spin));
    char label[120];
    sprintf(label, "<i><span size=\"larger\" foreground=\"#007A33\">p</span><span foreground=\"#007A33\"><sub>r</sub></span></i>  = %.4lf", p_r);
    gtk_label_set_markup(GTK_LABEL(pr_celtics), label);
    sprintf(label, "<i><span size=\"larger\" foreground=\"#fdb927\">p</span><span foreground=\"#fdb927\"><sub>h</sub></span></i> = %.4lf", 1 - p_r);
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
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(p_h_spin), S->p_h);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(p_r_spin), S->p_r);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(games_spinner), S->n * 2 - 1);
        for(int i = 0; i < S->n * 2 - 1; i++){
            GtkWidget *toggle_button = gtk_grid_get_child_at(GTK_GRID(format), i, 1);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle_button), S->format[i]);
        }
        solve_series(S);
        load_series_on_table(S);
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
        Series *S = init_series_from_ui();
        save_series(fn, S);   
        free(fn);
    }
    gtk_widget_destroy(dialog);
}

void load_css(char *file, GtkWidget *widget){
    GtkCssProvider *provider = gtk_css_provider_new ();
    GtkStyleContext * context = gtk_widget_get_style_context(widget);
    gtk_css_provider_load_from_path (provider, file, NULL);
    gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
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
    format_label = GTK_WIDGET(gtk_builder_get_object(builder, "format_label"));
     
    s_scroll = GTK_WIDGET(gtk_builder_get_object(builder, "s_scroll"));
    format_scroll = GTK_WIDGET(gtk_builder_get_object(builder, "format_scroll"));
    first_toggle = GTK_WIDGET(gtk_builder_get_object(builder, "first_toggle"));
    toolbar = GTK_WIDGET(gtk_builder_get_object(builder, "toolbar"));
    load_css("css/toggle.css", first_toggle);
    load_css("css/background.css", s_scroll);
    load_css("css/background.css", format_scroll);
    load_css("css/background.css", window);
    load_css("css/background.css", toolbar);

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

