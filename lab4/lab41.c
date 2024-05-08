#include <sqlite3.h>
#include <stdio.h>
#include "lab4_sqlite.h"

static sqlite3 *db = NULL;

int callback(void *, int, char **, char **);

void sqlite_open_db() {
    int rc;
    rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
}

void sqlite_close_db() {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}

void sqlite_get_data() {
    if (db == NULL) {
        fprintf(stderr, "Database is not open.\n");
        return;
    }
    
    char *sql = "SELECT \
                  id, \
                  name \
              FROM \
                  your_table;";
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, callback, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void sqlite_update(int compid, const char *compname, float price) {
    if (db == NULL) {
        fprintf(stderr, "Database is not open.\n");
        return;
    }
    //код для обновления данных в базе данных
}

void sqlite_close_connection() {
    sqlite_close_db();
    printf("Connection closed.\n");
}
