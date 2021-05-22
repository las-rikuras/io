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

