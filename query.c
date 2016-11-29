#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
//#include <stdint.h>

typedef struct ship_status
{
    char shipid;
    char outport;
    char location;
    char speed;
} SHIP_STATUS;

/*struct position
{
    float lng;
    float lat;
};
*/
typedef struct ship_record {
    int nRow;
    int nCol;
    char **recordes;
} SHIP_RECORD;
    
SHIP_RECORD* QueryAllShips()
{
    int i;
    sqlite3* db;
    char* errmsg;
    int nRow;
    int nCol;
    char *strSql="select ShipID from ships";
    struct ship_record *record;

    int nResult = sqlite3_open("test.db",&db);
    if (nResult == SQLITE_OK)
    {
        printf("数据库打开成功\n");
    }

    //record = malloc(sizeof(struct ship_record) + sizeof(char *) * nRow);
    //nResult = sqlite3_get_table(db,strSql,&pResult,&nRow,&nCol,&errmsg);
    record = malloc(sizeof(struct ship_record));
    nResult = sqlite3_get_table(db,strSql,&record->recordes, &record->nRow, &record->nCol, &errmsg);

#if 0
    printf("dx: %s\n", pResult[2]);

    for(i=1;i<nRow;i++)
    {
            rLen = strlen(pResult[i]);
            record->record[i] = malloc(rLen + 1);
            if (record->record[i]) {
                memset(record->record[i], 0, rLen+1);
                memcpy(record->record[i], pResult[i], rLen);
            }
    }
    sqlite3_free_table(pResult);
#endif
    sqlite3_close(db);
    return record;
}


SHIP_RECORD* QueryShipStatus()
{
    sqlite3* db;
    int nResult = sqlite3_open("test.db",&db);
    if (nResult == SQLITE_OK)
    {
        printf("数据库打开成功\n");
    }
    char* errmsg;
    int nRow;
    int nCol;
    struct ship_record *record;
    char* strSql = "select * from ships where shipid=1";

    //strSql += shipid;

    record = malloc(sizeof(struct ship_record));
    nResult = sqlite3_get_table(db,strSql,&record->recordes,&record->nRow,&record->nCol,&errmsg);

    sqlite3_close(db);
    return record;
    
}


void ship_free_recordes(struct ship_record *r)
{
    int i = 0;
    sqlite3_free_table(r->recordes);
    free(r);
}

int main(){
    SHIP_RECORD* a;
    a=QueryShipStatus();
    int i=0;
    for (i;i<(a->nRow > a->nCol ? a->nRow : a->nCol);i++)
    {
        printf("%s\n",a->recordes[i+a->nCol]);
    }

    ship_free_recordes(a);

    return 0;
}
