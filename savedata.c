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
    time_t date;
    struct gps_info* gpsinfo;
    char* devstatus;
    float speed;
} STATUS_INFO;


int main()
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    char* zErr;
    int rc;
    char* sql;
    const char* tail;

    struct status_info * stat_info;
    struct gps_info* gps_infos;

    stat_info = malloc(sizeof(struct status_info));
    gps_infos = malloc(sizeof(struct gps_info));

    gps_infos->lng = 110.52;
    gps_infos->lat = 12.31;

    stat_info->shipid = 119;
    stat_info->devid = 13;
    stat_info->date = time(NULL);
    stat_info->gpsinfo = gps_infos;
    stat_info->devstatus = "normal";
    stat_info->speed = 12.53;

    printf("%c", stat_info->date);
    printf("%f", gps_infos->lng);
    printf("%f", gps_infos->lat);

//#if 0
    rc = sqlite3_open("nhship.db", &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
//#if 0 
    sql = "CREATE TABLE IF NOT EXISTS ship_device(shipid integer, \
        devid integer, \
        date timestamp,\
        gpsinfo blob, \
        devstatus text,\
        speed fload(10,3));"
//#endif
    sql = "INSERT INTO ship_device VALUES(?,?,?,?,?,?);";

    sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);

    sqlite3_bind_int(stmt, 1, stat_info->shipid);
    sqlite3_bind_int(stmt, 2, stat_info->devid);
    sqlite3_bind_text(stmt, 3, stat_info->date, strlen(stat_info->devstatus), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 4, stat_info->gpsinfo,sizeof(struct gps_info), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, stat_info->devstatus, strlen(stat_info->devstatus), SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, stat_info->speed);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    sqlite3_close(db);
//#endif

    return 0;
}

