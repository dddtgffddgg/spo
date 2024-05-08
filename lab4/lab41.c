#include <sqlite3.h>
#include <stdio.h>
#include "lab4_sqlite.h"

static sqlite3 *db = NULL;

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void sqlite_open_db() {
    if (db != NULL) {
        fprintf(stderr, "Database is already open.\n");
        return;
    }
    int rc;
    rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
}

void sqlite_close_db() {
    if (db == NULL) {
        fprintf(stderr, "Database is not open.\n");
        return;
    }
    sqlite3_close(db);
    db = NULL;
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
