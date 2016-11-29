#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
//#include <stdint.h>

typedef struct data_record{
    int nRow;
    int nCol;
    char **records;
} DATA_RECORD;
    
DATA_RECORD* QueryRecord(char *strSql)
{
    sqlite3* db;
    char* errmsg;
    int nRow;
    int nCol;
    struct data_record *record;

    int nResult = sqlite3_open("test.db",&db);
    if (nResult == SQLITE_OK)
    {
        printf("数据库打开成功\n");
    }

    record = malloc(sizeof(struct data_record));
    nResult = sqlite3_get_table(db,strSql,&record->records, &record->nRow, &record->nCol, &errmsg);

    sqlite3_close(db);
    return record;
}

void free_records(struct data_record *r)
{
    sqlite3_free_table(r->records);
    free(r);
}

int main(){
    DATA_RECORD* a;
    char *strSql;

    strSql = "select * from devices where devid=3;"; 
        
    a=QueryRecord(strSql);

    int i,j;
    printf("%d %d\n", a->nRow, a->nCol);

    for (i=0;i<=a->nRow;i++)
    {
        for(j=0;j<a->nCol;j++)
        {
            printf("%s\t",a->records[i*(a->nCol)+j]);
        }
        printf("\n");
    }

    free_records(a);

    return 0;
}
