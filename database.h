/*
 *Author:daixin
 *Date:2016-11-23
 */
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

/*渔船状态显示*/
typedef struct {
    int shipid;
    position location;
    float speed;
} ship_status;

typedef struct {
    float lng /*经度*/
    float lat /*纬度*/
} position;

/*
 * 查询所有渔船
 * 输入参数：无
 * 返回值：船只ID列表
 */
(int [])QueryAllShips();

/*
 * 查询船只状态信息
 * 输入参数：船只ID
 * 返回值：当前船只状态
 */
(ship_status)QueryShipStatus(int shipid);

/*
 * 设备状态显示
 */
typedef struct {
    int devid;
    int devnum;
    char devname;
} dev_status;

/*
 * 查询所有设备
 * 输入参数：无
 * 返回值：设备信息列表
 */
(dev_status[])QueryAllDevices();

/*
 * 查询当前船只上所有设备
 * 输入参数：船只ID
 * 返回值：设备信息列表
 */
(dev_status[])QueryShipDevices(int shipid);

/*
 * 历史探测数据查询
 * 输入参数：船只ID、开始时间、结束时间
 * 返回值：文件列表
 */
(*file[])QueryData(int shipid,char starttime,char endtime);

/*
 * 信息筛选-船只信息
 * 输入参数：经纬度坐标
 * 返回值：船只ID列表
 * 输入参数：设备种类ID
 * 返回值：船只ID列表
 */
(int [])QueryShipByLocation(struct position);
(int [])QueryShipByDevice(int devid);

/*
 * 信息筛选-设备信息
 * 输入参数：设备状态
 * 返回值：设备列表
 * 输入参数：设备种类
 * 返回值：设备列表
 */
(struct dev_status [])QueryDevByStatus(char devstatus);
(struct dev_status [])QueryDevByType(int devid);


