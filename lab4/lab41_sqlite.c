#include <sqlite3.h>
#include <stdio.h>
#include "lab41_sqlite.h"

static sqlite3 *db = NULL;

void sqlite_open_db() {
    int rc;
    rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
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
                    books.book_id, \
                    books.title, \
                    books.annotation, \
                    books.pages, \
                    books.isbn, \
                    authors.author_id, \
                    authors.name AS author_name, \
                    publishers.publisher_id, \
                    publishers.name AS publisher_name, \
                    publishers.city \
                 FROM \
                    books \
                 INNER JOIN authors ON books.author_id = authors.author_id \
                 INNER JOIN publishers ON books.publisher_id = publishers.publisher_id;";
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, callback, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void sqlite_insert_author(const char *name) {
    if (db == NULL) {
        fprintf(stderr, "Database is not open.\n");
        return;
    }
    
    char *sql = sqlite3_mprintf("INSERT INTO authors (name) VALUES (%Q);", name);
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    sqlite3_free(sql);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to insert data into authors\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void sqlite_insert_publisher(const char *name, const char *city) {
    if (db == NULL) {
        fprintf(stderr, "Database is not open.\n");
        return;
    }
    
    char *sql = sqlite3_mprintf("INSERT INTO publishers (name, city) VALUES (%Q, %Q);", name, city);
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    sqlite3_free(sql);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to insert data into publishers\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void sqlite_insert_book(const char *title, const char *annotation, int pages, const char *isbn, int author_id, int publisher_id) {
    if (db == NULL) {
        fprintf(stderr, "Database is not open.\n");
        return;
    }
    
    char *sql = sqlite3_mprintf("INSERT INTO books (title, annotation, pages, isbn, author_id, publisher_id) VALUES (%Q, %Q, %d, %Q, %d, %d);", 
                                title, annotation, pages, isbn, author_id, publisher_id);
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    sqlite3_free(sql);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to insert data into books\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void sqlite_exit() {
    sqlite_close_db();
    printf("Exiting...\n");
}
