#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

void value_change(GtkEditable *editable, gpointer data);
void weigth_change(GtkEditable *editable, gpointer data);

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

void insertEntry(GtkWidget *grid, int i, int j, char *val, char *placeholder, int is_editable, int has_frame, int is_value){
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 3);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder);
    gtk_entry_set_alignment (GTK_ENTRY(entry), 0.5);
    gtk_widget_set_sensitive(entry, is_editable);
    gtk_entry_set_has_frame(GTK_ENTRY(entry), has_frame);
    g_signal_connect(G_OBJECT(entry), "insert_text", G_CALLBACK(entry_number), NULL);

    if(is_value == 1){
        g_signal_connect(G_OBJECT(entry), "changed", G_CALLBACK(value_change), NULL);
    }
    else if (is_value == 0){
        g_signal_connect(G_OBJECT(entry), "changed", G_CALLBACK(weigth_change), NULL);
    }
    else{
        
    }

    if(val){
        gtk_entry_set_text(GTK_ENTRY(entry), val);
    }

    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}

void insertLabel(GtkWidget *grid, int i, int j, char *val, char *markup){
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
    gtk_label_set_width_chars (GTK_LABEL(label), 3);

    gtk_widget_show (GTK_WIDGET(label));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(label), i, j, 1 ,1);
}

void insertEntryLabel(GtkWidget *grid, int i, int j, char *val, char *placeholder, int is_editable, int has_frame){
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 3);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder);
    gtk_entry_set_alignment (GTK_ENTRY(entry), 0.5);
    gtk_widget_set_sensitive(entry, is_editable);
    gtk_entry_set_has_frame(GTK_ENTRY(entry), has_frame);

    if(val){
        gtk_entry_set_text(GTK_ENTRY(entry), val);
    }

    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}