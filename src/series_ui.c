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

int game_number;
int previous_game_number = 1;

void on_close_clicked(){
    gtk_main_quit();
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

void insert_check_box(GtkWidget *grid, int i, int j){
    GtkWidget *check_box = gtk_check_button_new();
    gtk_widget_show (GTK_WIDGET(check_box));
    gtk_widget_set_halign (GTK_WIDGET(check_box), GTK_ALIGN_CENTER);
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(check_box), i, j, 1 ,1);
}

void on_max_games_value_changed(){
    game_number = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(games_spinner));
    if(game_number > previous_game_number){
        for(int i = previous_game_number; i < game_number; i++){
            char label[100];
            sprintf(label, "%d", i + 1);
            insert_label(s_grid, i+3, 0, 8, label , NULL);
            insert_label(s_grid, 0, i+3, 4, label , NULL);
            insert_label(s_grid, i+3, 1, 8, "1.000" , NULL);
            insert_label(s_grid, 1, i+3, 8, "0.000", NULL);

            // Format
            insert_label(format, i+1, 0, 4, label, NULL);
            insert_check_box(format, i+1, 1);
        }
    } else {
        for(int i = previous_game_number; i > game_number; i--){
            gtk_grid_remove_column(GTK_GRID(s_grid), i+2);
            gtk_grid_remove_row(GTK_GRID(s_grid), i+2);

            gtk_grid_remove_column(GTK_GRID(format), i);
        }
    }
    previous_game_number = game_number;
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
    
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

