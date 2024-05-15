#include <sqlite3.h>
#include <stdio.h>
#include "lab41_sqlite.h"

#define DB_FILE "mydb.db"

int main() {
    sqlite_open_db();
    
    // Вставка данных
    sqlite_insert_author("New Author");
    sqlite_insert_publisher("New Publisher", "New City");
    sqlite_insert_book("New Book", "New Annotation", 123, "ISBN123", 1, 1);
    
    // Получение данных
    sqlite_get_data();
    
    sqlite_close_db();
    sqlite_exit();
    return 0;
}
