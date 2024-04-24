#ifndef LAB4_SQLITE_H
#define LAB4_SQLITE_H

#define DB_FILE "mydb.db"

void sqlite_get_data();
void sqlite_update(int book_id, const char *book_title, const char *book_description, int book_pages);

#endif

