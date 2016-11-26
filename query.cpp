#include <iostream>
#include <boost/lexical_cast.hpp>
#include <list>
#include <iterator>
#include <sqlite3.h>
using namespace std;

struct ship_status
{
    int shipid;
    char outport;
    char location;
    float speed;
};

/*struct position
{
    float lng;
    float lat;
};
*/
    

list<char> QueryAllShips()
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
    string strSql="select ShipID from ships";

    nResult = sqlite3_get_table(db,strSql.c_str(),&pResult,&nRow,&nCol,&errmsg);
    cout<<pResult[1]<<endl;
    cout<<pResult[6]<<endl;

    list<char> shipid;
    for(int i=0;i<nRow;i++)
    {
            shipid.push_back(pResult[(i+1)]);
    }
    sqlite3_free_table(pResult);
    sqlite3_close(db);
    return shipid;
}


ship_status QueryShipStatus(char shipid)
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



int main(){
    list<char> a;
    a=QueryAllShips();
    cout << "shipid:";
    ostream_iterator<char,char>out(cout," ");
    copy(a.begin(),a.end(),out);
    cout << endl;

    //list<char>::iterator i;
    //i=a.begin();

    ship_status ship;
    char id='1';
    ship = QueryShipStatus(id);
    cout<<ship.shipid<<endl;
    cout<<ship.outport<<endl;
    cout<<ship.location<<endl;
    cout<<ship.speed<<endl;

    return 0;
}
