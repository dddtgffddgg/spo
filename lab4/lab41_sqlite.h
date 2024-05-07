#ifndef LAB4_SQLITE_H
#define LAB4_SQLITE_H

#define DB_FILE "mydb.db"

#include <sqlite3.h>

void sqlite_open_db();
void sqlite_close_db();
void sqlite_get_data();
void sqlite_exit();

#endif
