/*
 *Author:daixin
 *Date:2016-11-23
 */
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

/*�洬״̬��ʾ*/
typedef struct {
    int shipid;
    position location;
    float speed;
} ship_status;

typedef struct {
    float lng /*����*/
    float lat /*γ��*/
} position;

/*
 * ��ѯ�����洬
 * �����������
 * ����ֵ����ֻID�б�
 */
(int [])QueryAllShips();

/*
 * ��ѯ��ֻ״̬��Ϣ
 * �����������ֻID
 * ����ֵ����ǰ��ֻ״̬
 */
(ship_status)QueryShipStatus(int shipid);

/*
 * �豸״̬��ʾ
 */
typedef struct {
    int devid;
    int devnum;
    char devname;
} dev_status;

/*
 * ��ѯ�����豸
 * �����������
 * ����ֵ���豸��Ϣ�б�
 */
(dev_status[])QueryAllDevices();

/*
 * ��ѯ��ǰ��ֻ�������豸
 * �����������ֻID
 * ����ֵ���豸��Ϣ�б�
 */
(dev_status[])QueryShipDevices(int shipid);

/*
 * ��ʷ̽�����ݲ�ѯ
 * �����������ֻID����ʼʱ�䡢����ʱ��
 * ����ֵ���ļ��б�
 */
(*file[])QueryData(int shipid,char starttime,char endtime);

/*
 * ��Ϣɸѡ-��ֻ��Ϣ
 * �����������γ������
 * ����ֵ����ֻID�б�
 * ����������豸����ID
 * ����ֵ����ֻID�б�
 */
(int [])QueryShipByLocation(struct position);
(int [])QueryShipByDevice(int devid);

/*
 * ��Ϣɸѡ-�豸��Ϣ
 * ����������豸״̬
 * ����ֵ���豸�б�
 * ����������豸����
 * ����ֵ���豸�б�
 */
(struct dev_status [])QueryDevByStatus(char devstatus);
(struct dev_status [])QueryDevByType(int devid);


