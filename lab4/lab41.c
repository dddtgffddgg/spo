#include <sqlite3.h>
#include <stdio.h>
#include "lab4_sqlite.h"

int main() {
    sqlite_open_db();
    sqlite_get_data();
    sqlite_close_connection();
    return 0;
}
