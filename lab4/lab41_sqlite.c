#include <sqlite3.h>
#include <stdio.h>
#include "lab4_sqlite.h"

int callback(void *, int, char **, char **);

void sqlite_get_data() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db))) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql = "SELECT \
                    authors.id AS author_id, authors.name AS author_name, \
                    books.id AS book_id, books.title AS book_title, books.annotation AS book_description, books.pages AS book_pages, \
                    publishers.id AS publisher_id, publishers.name AS publisher_name, publishers.city AS publisher_city \
                FROM \
                    books \
                INNER JOIN authors ON books.author_id = authors.id \
                INNER JOIN publishers ON books.publisher_id = publishers.id;";
    rc = sqlite3_exec(db, sql, callback, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}

void sqlite_update(int book_id, const char *book_title, const char *book_description, int book_pages) {
    // обновление данных в базе данных
}

