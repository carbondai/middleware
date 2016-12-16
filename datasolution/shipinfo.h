#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

/*
 * ϵͳ��Ϣ�ֶ�
 */
typedef struct sys_msg
{
    char s_id;
    char msg_type;
    int payload_len;
    char* payload;
}SYS_MSG;

/*
 * gps������Ϣ��lngΪ���ȣ�latΪγ�ȣ���Χ-180��+180
 */
typedef struct gps_info
{
    double lng;
    double lat;
}GPS_INFO;

/*�豸������Ϣ
 */
typedef struct dev_config
{
    char header;
    int devid;
    int iterbytes;
    char* iterinfo;
    int otherbytes;
    char* otherinfo;
}DEV_CONFIG;

/*��ֻ���ɼ���Ԫ�̶���Ϣ
 */
typedef struct ships_info
{
    char shiptype;
    int shipid;
    int phonenumber;
    char ip;
    char version;
    char storageinfo;
    int devnum;
}SHIPS_INFO;

/*
 * ��ֻ��̬��Ϣ
 */
typedef struct ships_status
{
    int shipid;
    time_t time;
    struct gps_info* gpsinfo;
    char* shipstatus;
    double speed;
    double course;
    int totaldev;
}SHIPS_INFO;

/*
 * ��ֻ�豸��Ӧ״̬��Ϣ
 */
typedef struct ship_dev
{
    int devid;
    int shipid;
    time_t time;
    char* devstatus;
}SHIP_DEV;

/*
 * ����״̬��Ϣ
 */
typedef struct data_info
{
    int shipid;
    time_t time;
    struct gps_info* gpsinfo;
    int devid;
    int datalength;
}DATA_INFO;


/*----------------------function----------------------*/
 
/*��ȡ�豸��ϸ��Ϣ
 */
struct dev_config* querydevinfo(int devid)
{
    sqlite3* db;
    char* zErr;
    char* sql;
    const char* tail;
    int rc;
    struct dev_config* devinfo;

    devinfo = malloc(sizeof(struct dev_config));

    sql = "SELECT * FROM dev_config WHERE devid=?;";

    rc = sqlite3_open("nhship.db", &db);
    if(rc){
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_prepare(db, sql, strlen(sql), &stmt, &tail);
    sqlite3_bind_int(stmt, 1, devid);
    sqlite3_step(stmt);
    ncols = sqlite3_column_count(stmt);

    devinfo->header = sqlite3_column_text(stmt, 0);
    devinfo->devid = sqlite3_column_int(stmt, 1);
    devinfo->iterbytes = sqlite3_column_int(stmt, 2);
    devinfo->iterinfo = malloc(devinfo->iterbytes);
    memcpy(devinfo->iterinfo, sqlite3_column_blob(stmt, 3), devinfo->iterbytes);
    devinfo->otherbytes = sqlite3_column_int(stmt, 4);
    devinfo->otherinfo = malloc(devinfo->otherbytes);
    memcpy(devinfo->otherinfo, sqlite3_column_blob(stmt, 5), devinfo->otherbytes);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return devinfo;
}

void localstatus()
{

}
