#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void main()
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    int rc, ncols, i;
    int devid=142;
    const char* tail;
    char* sql1;
    sql1 = "select * from data_info where devid=?;";

    sqlite3_open("nhship.db", &db);
    sqlite3_prepare(db, sql1, strlen(sql1), &stmt, &tail);
    sqlite3_bind_int(stmt, 1, devid);
    rc = sqlite3_step(stmt);

    ncols = sqlite3_column_count(stmt);
    while(rc == SQLITE_ROW){
        for(i=0;i<ncols;i++){
            fprintf(stderr, "'%s' ", sqlite3_column_text(stmt, i));
        }
        rc = sqlite3_step(stmt);
    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);


}
