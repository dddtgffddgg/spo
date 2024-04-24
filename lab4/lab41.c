#include "lab4_sqlite.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#define UI_FILE "lab04.glade"

enum
{
    ID = 0,
    NAME,
    ANNOTATION,
    PAGES,
    ISBN
};

struct MainWindowObjects
{
    GtkWindow *main_window;
    GtkTreeView *treeview;
    GtkListStore *liststore;
} mainWindowObjects;

int callback(void *not_used, int argc, char **argv, char **col_names)
{
    GtkTreeIter iter;
    if (argc == 5)
    {
        gtk_list_store_append(GTK_LIST_STORE(mainWindowObjects.liststore), &iter);
        gtk_list_store_set(GTK_LIST_STORE(mainWindowObjects.liststore), &iter, ID,
                           atoi(argv[ID]), NAME, argv[NAME], ANNOTATION, argv[ANNOTATION],
                           PAGES, atoi(argv[PAGES]), ISBN, argv[ISBN], -1);
    }
    return 0;
}

int main(int argc, char **argv)
{
    GtkBuilder *builder;
    GError *error = NULL;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
    {
        g_warning("%s\n", error->message);
        g_free(error);
        return (1);
    }

    mainWindowObjects.main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    mainWindowObjects.treeview =
        GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview_components"));
    mainWindowObjects.liststore =
        GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore_components"));

    gtk_builder_connect_signals(builder, &mainWindowObjects);

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(GTK_WIDGET(mainWindowObjects.main_window));

    sqlite_get_data();

    gtk_main();
}

// Остальные функции остаются без изменений
