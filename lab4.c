#include <gtk/gtk.h>
#include <sqlite3.h>

sqlite3 *db;

// Функция для отображения данных
void display_data(GtkWidget *treeview) {
    // Очистка списка
    gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    GtkListStore *store = gtk_list_store_new(5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING);
    
    // Выполнение запроса к базе данных
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT id, title, author, pages, isbn FROM books", -1, &stmt, NULL);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *title = sqlite3_column_text(stmt, 1);
        const unsigned char *author = sqlite3_column_text(stmt, 2);
        int pages = sqlite3_column_int(stmt, 3);
        const unsigned char *isbn = sqlite3_column_text(stmt, 4);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, id, 1, title, 2, author, 3, pages, 4, isbn, -1);
    }
    sqlite3_finalize(stmt);
    
    // Установка модели для дерева
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

// Функция для инициализации базы данных
void init_database() {
    int rc = sqlite3_open("books.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *treeview;

    gtk_init(&argc, &argv);
    init_database();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    treeview = gtk_tree_view_new();
    display_data(treeview);

    gtk_container_add(GTK_CONTAINER(window), treeview);
    gtk_widget_show_all(window);

    gtk_main();

    sqlite3_close(db);
    return 0;
}
