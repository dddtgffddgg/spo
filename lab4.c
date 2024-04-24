#include "lab04_sqlite.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#define UI_FILE "lab04.glade"

enum
{
    COMPID = 0,
    COMPNAME,
    PRICE,
    SHOPID,
    SHOPNAME
};

struct MainWindowObjects
{
    GtkWindow *main_window;
    GtkTreeView *treeview;
    GtkListStore *liststore;
    GtkAdjustment *adjustment;
    GtkTreeViewColumn *column_comp_id;
    GtkTreeViewColumn *column_price;
    GtkTreeViewColumn *column_shop_id;
} mainWindowObjects;

int callback(void *not_used, int argc, char **argv, char **col_names)
{
    GtkTreeIter iter;
    if (argc == 5)
    {
        gtk_list_store_append(GTK_LIST_STORE(mainWindowObjects.liststore), &iter);
        gtk_list_store_set(GTK_LIST_STORE(mainWindowObjects.liststore), &iter, COMPID,
                           atoi(argv[COMPID]), COMPNAME, argv[COMPNAME], PRICE, atof(argv[PRICE]),
                           SHOPID, atoi(argv[SHOPID]), SHOPNAME, argv[SHOPNAME], -1);
    }
    return 0;
}

void price_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model,
                          GtkTreeIter *iter, gpointer user_data)
{
    gfloat price;
    gchar buf[30];
    gtk_tree_model_get(model, iter, PRICE, &price, -1);
    g_snprintf(buf, sizeof(buf), "%.2f", price);
    g_object_set(renderer, "text", buf, NULL);
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
    mainWindowObjects.adjustment =
        GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment_price"));

    mainWindowObjects.column_price =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_price"));
    GtkCellRenderer *cell = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "clnrender_price"));
    gtk_tree_view_column_set_cell_data_func(mainWindowObjects.column_price, cell,
                                            price_cell_data_func, NULL, NULL);

    mainWindowObjects.column_comp_id =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_comp_id"));
    mainWindowObjects.column_shop_id =
        GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "cln_shop_id"));

    gtk_builder_connect_signals(builder, &mainWindowObjects);

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(GTK_WIDGET(mainWindowObjects.main_window));

    sqlite_get_data();

    gtk_main();
}

G_MODULE_EXPORT void on_btnsave_clicked(GtkWidget *button, gpointer data)
{
    GtkTreeIter iter;
    gboolean reader =
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    while (reader)
    {
        gint compid;
        gchar *compname;
        gfloat price;
        gtk_tree_model_get(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter, COMPID, &compid,
                           COMPNAME, &compname, PRICE, &price, -1);
        sqlite_update(compid, compname, price);
        reader = gtk_tree_model_iter_next(GTK_TREE_MODEL(mainWindowObjects.liststore), &iter);
    }
    gtk_list_store_clear(mainWindowObjects.liststore);
    sqlite_get_data();
}

G_MODULE_EXPORT void on_cellrenderertext_compname_edited(GtkCellRendererText *renderer, gchar *path,
                                                         gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, COMPNAME, new_text, -1);
    }
}

G_MODULE_EXPORT void on_cellrendererspin_price_edited(GtkCellRendererText *renderer, gchar *path,
                                                      gchar *new_text, gpointer data)
{
    if (g_ascii_strcasecmp(new_text, "") != 0)
    {
        GtkTreeIter iter;
        GtkTreeModel *model;
        model = gtk_tree_view_get_model(mainWindowObjects.treeview);
        if (gtk_tree_model_get_iter_from_string(model, &iter, path))
            gtk_list_store_set(GTK_LIST_STORE(model), &iter, PRICE, atof(new_text), -1);
    }
}

G_MODULE_EXPORT void on_show_hidden_toggled(GtkToggleButton *button, gpointer data)
{
    gboolean visible = gtk_toggle_button_get_active(button);
    gtk_tree_view_column_set_visible(mainWindowObjects.column_comp_id, visible);
    gtk_tree_view_column_set_visible(mainWindowObjects.column_shop_id, visible);
}

G_MODULE_EXPORT void on_btnabout_clicked(GtkButton *button, gpointer data)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "О программе", mainWindowObjects.main_window,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "_OK", GTK_RESPONSE_NONE, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(content_area), 15);
    GtkWidget *label = gtk_label_new("\nЭто как ваша лабораторная работа №4, только меньше\n");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show(label);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

G_MODULE_EXPORT void on_window_destroy(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}

G_MODULE_EXPORT void on_btnexit_clicked(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}
