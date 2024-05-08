#include <sqlite3.h>
#include <stdio.h>
#include "lab41_sqlite.h"

#define DB_FILE "mydb.db"

int main() {
    sqlite_open_db();
    sqlite_get_data();
    sqlite_close_db();
    sqlite_exit();
    return 0;
}
