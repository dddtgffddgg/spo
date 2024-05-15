#ifndef LAB4_SQLITE_H
#define LAB4_SQLITE_H

#define DB_FILE "mydb.db"

#include <sqlite3.h>

void sqlite_open_db();
void sqlite_close_db();
void sqlite_get_data();
void sqlite_insert_author(const char *name);
void sqlite_insert_publisher(const char *name, const char *city);
void sqlite_insert_book(const char *title, const char *annotation, int pages, const char *isbn, int author_id, int publisher_id);
void sqlite_exit();

#endif
