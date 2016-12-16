#include <stdio.h>
#include <shipinfo.h>

/*
 * 从系统消息中提取对应的字段，返回结构化数据
 */
struct sys_msg* MsgFormat(char *buffer)
{
    char* pos = 0;
    struct sys_msg* sys_msgs;
    sys_msgs = malloc(sizeof(struct sys_msg));

    memcpy(sys_msgs->s_id, buffer, 1);
    pos = pos + 1;
    memcpy(sys_msgs->msg_type, buffer+pos, 1);
    pos = pos + 1;
    memcpy(sys_msgs->payload_len, buffer+pos, 2);
    pos = pos + 2;
    memcpy(sys_msgs->payload, buffer+pos, payload_len);

    return sys_msgs;
}

/*
 * 判断系统消息类型
 */
void CmdMsg(struct sys_msg* sys_msgs)
{
    char* pos = 0;
    char subcmd;
    char devid;
    int datalength;
    char *cmddata;

    memcpy(subcmd, sys_msgs->payload, 1);
    pos = pos + 1;

    switch(sys_msgs->msg_type){
        case "0x00": 
            if(subcmd == '0x00'){
                memcpy(devid, sys_msgs->payload+pos, 1);
                pos = pos + 1;
                memcpy(datalength, sys_msgs->payload+pos, 2);
                pos = pos + 1;
                memcpy(cmddata, sys_msgs->payload+pos, datalength);
            }else if(subcmd == '0x01'){
                querydevdata(devid);
            }else if(subcmd == '0x02'){
                querydevinfo(devid);
            }
            break;
        case "0x01":
            localstatus();
        case "0x02":
            memcpy(time1, sys_msgs->payload+pos, 4);
            pos = pos + 4;
            memcpy(time2, sys_msgs->payload+pos, 4);
            pos = pos + 4;
            memcpy(lng, sys_msgs->payload+pos, 8);
            pos = pos + 8;
            memcpy(lat, sys_msgs->payload+pos, 8);
            pos = pos + 8;
            memcpy(radius, sys_msgs->payload+pos, 4);
            pos = pos + 4;
            if(subcmd == '0x00'){
                queryrecords(time1,time2,lng,lat);
            }else if(subcmd == '0x01'){
                memcpy(devid, sys_msgs->payload+pos, 1);
                querydata();
            }
            break;
        case "0x03":
            if(subcmd == '0x00'){
                queryship();
            }else if(subcmd == '0x01'){
                queryalldev();
            }
            break;
        case "0x04":
            if(subcmd == '0x00'){
                updateship();
            }else if(subcmd == '0x01'){
                saveship();
            }
            break;
        case "0x05":
            if(subcmd == '0x00'){
                updatedata();
            }else if (subcmd == '0x01'){
                savedata();
            }
            break;
        case "0x06":
            if(subcmd == '0x00'){
                saveshipinfos();
            }else if(subcmd == '0x01'){
                savedevinfos();
            }
            break;
    }
}
