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
    float lng;
    float lat;
}GPS_INFO;

typedef struct status_info
{
    int shipid;
    int devid;
    time_t time;
    struct gps_info* gpsinfo;
    char* devstatus;
    float speed;
} STATUS_INFO;

void InsertData(struct status_info* stat_info)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    int rc;
    char* sql;
    const char* tail;

    rc = sqlite3_open("nhship.db", &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sql = "CREATE TABLE IF NOT EXISTS ship_device(shipid integer, \
        devid integer, \
        time text,\
        lng float(10,3), \
        lat float(10,3), \
        devstatus text,\
        speed fload(10,3));"
        "INSERT INTO ship_device VALUES(?,?,?,?,?,?,?);";

    while(sqlite3_complete(sql))
    {
        sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);

        sqlite3_bind_int(stmt, 1, stat_info->shipid);
        sqlite3_bind_int(stmt, 2, stat_info->devid);
        sqlite3_bind_text(stmt, 3, stat_info->time, strlen(stat_info->time), SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 4, stat_info->gpsinfo->lng);
        sqlite3_bind_double(stmt, 5, stat_info->gpsinfo->lat);
        sqlite3_bind_text(stmt, 6, stat_info->devstatus, strlen(stat_info->devstatus), SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 7, stat_info->speed);

        sql = tail;
    }
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);

}

int main()
{

    struct status_info * stat_info;
    struct gps_info* gps_infos;

    stat_info = malloc(sizeof(struct status_info));
    gps_infos = malloc(sizeof(struct gps_info));

    gps_infos->lng = 110.52;
    gps_infos->lat = 12.31;

    stat_info->shipid = 119;
    stat_info->devid = 13;
    stat_info->time= time(NULL);
    stat_info->gpsinfo = gps_infos;
    stat_info->devstatus = "normal";
    stat_info->speed = 12.53;

    InsertData(stat_info);

    return 0;
}

