#include <sqlite3.h>
#include <stdio.h>
#include "lab4_sqlite.h"

static sqlite3 *db = NULL;

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
    // Ваш код для запроса данных из базы данных
}

void sqlite_update(int compid, const char *compname, float price) {
    if (db == NULL) {
        fprintf(stderr, "Database is not open.\n");
        return;
    }
    // Ваш код для обновления данных в базе данных
}

void sqlite_close_connection() {
    sqlite_close_db();
    printf("Connection closed.\n");
}
