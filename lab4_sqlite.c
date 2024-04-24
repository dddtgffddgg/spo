#include <sqlite3.h>
#include <stdio.h>
#include "lab4_sqlite.h"

int callback(void *, int, char **, char **);
void sqlite_get_data()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql = "SELECT \
                  components.component_id, \
                  components.name, \
                  price, \
                  shops.shop_id, \
                  shops.name AS shop \
              FROM \
                  components \
              INNER JOIN shops ON components.shop_id = shops.shop_id;";
    rc = sqlite3_exec(db, sql, callback, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}

void sqlite_update(int compid, char *compname, float price)
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql_comp_temp = "UPDATE components\
                              SET \
                              name = '%s', \
                              price = %d.%d \
                          WHERE \
                              component_id = %d;";
    char sql_comp[255];
    sprintf(sql_comp, sql_comp_temp, compname, (int)price, ((int)(price * 100)) % 100, compid);
    rc = sqlite3_exec(db, sql_comp, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to update data in components\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}
