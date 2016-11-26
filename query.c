#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdint.h>

typedef struct 
{
    int shipid;
    char outport;
    char location;
    float speed;
}ship_status;

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
    char** pResult;
    int nRow;
    int nCol;
    int rLen = 0;
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


/*ship_status QueryShipStatus(char shipid)
{
    sqlite3* db;
    int nResult = sqlite3_open("test.db",&db);
    if (nResult == SQLITE_OK)
    {
        cout<<"数据库打开成功"<<endl;
    }
    char* errmsg;
    char** pResult;
    int nRow;
    int nCol;

    cout<<shipid<<endl;

    string strSql("select * from ships where shipid=");

    strSql += shipid;

    cout<< strSql <<endl;

    nResult = sqlite3_get_table(db,strSql.c_str(),&pResult,&nRow,&nCol,&errmsg);

    ship_status ship;

    for(int i=0;i<nRow;i++)
    {
            ship.shipid = boost::lexical_cast<int>(pResult[(i+1)*nCol]);
            ship.outport = (pResult[(i+1)*nCol+1]);
            ship.location = (pResult[(i+1)*nCol+2]);
            ship.speed = boost::lexical_cast<float>(pResult[(i+1)*nCol+3]);
    }
    sqlite3_free_table(pResult);
    sqlite3_close(db);
    return ship;
    
}
*/

void ship_free_recordes(struct ship_record *r)
{
    int i = 0;
#if 0
    char *p = NULL;

    for (i = 0; i < r->nRow; i++) {
        p = r->recordes[i];
        printf("p = %p: %s\n", p, p);
        free(p);
    }
#endif
    sqlite3_free_table(r->recordes);
    free(r);
}

int main(){
    SHIP_RECORD* a;
    a=QueryAllShips();

    int i;
    for(i=1;i<a->nRow;i++)
    {
        printf("%s\n", a->recordes[i]);
    }
    //list<char>::iterator i;
    //i=a.begin();
    ship_free_recordes(a);

    return 0;
}
