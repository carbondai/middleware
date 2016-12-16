/**
 * Date: Dec 2 2016
 * Author: Dai Xin
 */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include <string.h>

typedef struct gps_info
{
    double lng;
    double lat;
}GPS_INFO;

typedef struct ships_info
{
    int shipid;
    time_t time;
    struct gps_info* gpsinfo;
    char* shipstatus;
    double speed;
    double course;
    int totaldev;
}SHIPS_INFO;

typedef struct ship_dev
{
    int devid;
    int shipid;
    time_t time;
    char* devstatus;
}SHIP_DEV;

typedef struct data_info
{
    int shipid;
    time_t time;
    struct gps_info* gpsinfo;
    int devid;
    int datalength;
}DATA_INFO;

void CreateTable(char* sql)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    int rc;

    rc = sqlite3_open("nhship.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
    if(rc != SQLITE_OK)
    {
        if(zErr != NULL)
        {
            fprintf(stderr, "SQL error:%s\n", zErr);
            sqlite3_free(zErr);
        }
    }

    sqlite3_close(db);
}

void DeleteRecord(char* sql)
{
    CreateTable(sql);
}

/*
 * 收到状态数据后就插入此表，记录增加
 */
void InsertShips(struct ships_info* ship_info)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    int rc;
    char* sql;
    const char* tail;

    sql = "INSERT INTO ships_info VALUES(?,?,?,?,?,?,?,?);";

    rc = sqlite3_open("nhship.db", &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);

    sqlite3_bind_int(stmt, 1, ship_info->shipid);
    sqlite3_bind_int(stmt, 2, ship_info->time);
    sqlite3_bind_double(stmt, 3, ship_info->gpsinfo->lng);
    sqlite3_bind_double(stmt, 4, ship_info->gpsinfo->lat);
    sqlite3_bind_double(stmt, 5, ship_info->speed);
    sqlite3_bind_double(stmt, 6, ship_info->course);
    sqlite3_bind_text(stmt, 7, ship_info->shipstatus, strlen(ship_info->shipstatus), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, ship_info->totaldev);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

/*
 * 更新状态数据时，同时写入此表，包含设备和船的映射及设备状态
 */
void InsertShipDev(struct ship_dev* ship_device)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    int rc;
    char* sql;
    const char* tail;

    sql = "INSERT INTO ship_device VALUES(?,?,?,?);";

    rc = sqlite3_open("nhship.db", &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);

    sqlite3_bind_int(stmt, 1, ship_device->devid);
    sqlite3_bind_int(stmt, 2, ship_device->shipid);
    sqlite3_bind_int(stmt, 3, ship_device->time);
    sqlite3_bind_text(stmt, 4, ship_device->devstatus, strlen(ship_device->devstatus), SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);
} 

void InsertData(struct data_info* data_infos, char buffer[])
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    int rc;
    char* sql;
    const char* tail;

    sql = "INSERT INTO data_info VALUES(?,?,?,?,?,?,?);";

    rc = sqlite3_open("nhship.db", &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);

    sqlite3_bind_int(stmt, 1, data_infos->shipid);
    sqlite3_bind_int(stmt, 2, data_infos->time);
    sqlite3_bind_double(stmt, 3, data_infos->gpsinfo->lng);
    sqlite3_bind_double(stmt, 4, data_infos->gpsinfo->lat);
    sqlite3_bind_int(stmt, 5, data_infos->devid);
    sqlite3_bind_int(stmt, 6, data_infos->datalength);
    sqlite3_bind_blob(stmt, 7, buffer, data_infos->datalength, NULL);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);

}

int main()
{
    struct ships_info * ship_info;
    struct gps_info* gps_infos;
    struct ship_dev* ship_device;
    struct data_info* data_infos;
    time_t timep;
    int i;
    char* sql1, *sql2, *sql3;
    char buffer[16]={'1','2','3','4','5','6','7','8','9','0','a','b','c','d','e','f'};

    sql1 = "CREATE TABLE IF NOT EXISTS ships_info(shipid integer, \
            time integer,\
            lng real, \
            lat real, \
            speed real, \
            course real, \
            shipstatus text, \
            totaldev int);";

    sql2 = "CREATE TABLE IF NOT EXISTS ship_device(shipid integer, \
            devid integer, \
            time integer, \
            devstatus text);"; 

    sql3 = "CREATE TABLE IF NOT EXISTS data_info(shipid integer, \
            time integer, \
            lng real, \
            lat real, \
            devid integer, \
            datalength integer, \
            data blob);";

    CreateTable(sql1);
    CreateTable(sql2);
    CreateTable(sql3);

    ship_info = malloc(sizeof(struct ships_info));
    gps_infos = malloc(sizeof(struct gps_info));
    ship_device = malloc(sizeof(struct ship_dev));
    data_infos = malloc(sizeof(struct data_info));

    time(&timep);

    gps_infos->lng = 110.52;
    gps_infos->lat = 12.31;

    ship_info->shipid = 129;
    ship_info->time= timep;
    ship_info->gpsinfo = gps_infos;
    ship_info->shipstatus = "on the sea";
    ship_info->speed = 22.53;
    ship_info->course = 150.45;
    ship_info->totaldev = 20;

    ship_device->shipid = ship_info->shipid;
    ship_device->time = ship_info->time;
    for(i=0;i<ship_info->totaldev;i++)
    {
        ship_device->devid = 123+i;
        if((i%2) == 0)
        {
            ship_device->devstatus = "normal";
        }
        else ship_device->devstatus = "abnormal";

        InsertShipDev(ship_device);
    }

    data_infos->shipid = ship_info->shipid;
    data_infos->time = timep;
    data_infos->gpsinfo = gps_infos;
    data_infos->devid = ship_device->devid;
    data_infos->datalength = 16;

    InsertShips(ship_info);
    InsertData(data_infos, buffer);

    return 0;
}

