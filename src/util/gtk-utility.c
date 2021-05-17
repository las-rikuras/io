#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

void entry_number(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data){
    for (int i = 0; i < length; i++) {
        if (!isdigit(text[i])) {
            g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
            return;
        }
    }
}

void insertEntry(GtkWidget *grid, int i, int j, char *val, char *placeholder, int is_editable, int has_frame){
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_width_chars (GTK_ENTRY(entry), 3);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), placeholder);
    gtk_entry_set_alignment (GTK_ENTRY(entry), 0.5);
    gtk_widget_set_sensitive(entry, is_editable);
    gtk_entry_set_has_frame(GTK_ENTRY(entry), has_frame);
    g_signal_connect(G_OBJECT(entry), "insert_text", G_CALLBACK(entry_number), NULL);

    if(val){
        gtk_entry_set_text(GTK_ENTRY(entry), val);
    }

    gtk_widget_show (GTK_WIDGET(entry));
    gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(entry), i, j, 1 ,1);
}

void insertLabel(GtkWidget *grid, int i, int j, char *val){
    GtkWidget *label = gtk_label_new(val);
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