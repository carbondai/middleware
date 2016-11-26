#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

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
    

char* QueryAllShips()
{
    sqlite3* db;
    int nResult = sqlite3_open("test.db",&db);
    if (nResult == SQLITE_OK)
    {
        printf("数据库打开成功\n");
    }
    char* errmsg;
    char** pResult;
    int nRow;
    int nCol;
    char *strSql="select ShipID from ships";

    nResult = sqlite3_get_table(db,strSql,&pResult,&nRow,&nCol,&errmsg);

    printf("%s", pResult[2]);

    static char* shipid;
    int i;
    for(i=0;i<nRow;i++)
    {
            &shipid = pResult[(i+1)];
    }
    printf("ship\n");
    sqlite3_free_table(pResult);
    sqlite3_close(db);
    return shipid;
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


int main(){
    char* a;
    a=QueryAllShips();
    printf("shipid:");

    printf("%s", a[1]);
    //list<char>::iterator i;
    //i=a.begin();

    return 0;
}
