#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void main()
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    const char* tail;
    char* sql1, * sql2;
    sql1 = "select data from data_info;";

    sqlite3_open("nhship.db", &db);
    sqlite3_prepare(db, sql1, strlen(sql1), &stmt, &tail);

    int len = sqlite3_column_bytes(stmt, 0);
    char* data = malloc(len);
    memcpy(data, sqlite3_column_blob(stmt, 0), len);

    int i;
    for(i = 0; i<16; i++)
    {
        printf("%d\n", data[i]);
    }    

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    sqlite3_close(db);
}
